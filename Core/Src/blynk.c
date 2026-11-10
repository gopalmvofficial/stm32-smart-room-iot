/**
  ******************************************************************************
  * @file           : blynk.c
  * @brief          : Blynk 2.0 MQTT Protocol Engine & ESP8266 Wi-Fi State Machine
  * @author         : Gopal M V
  ******************************************************************************
  */

#include "blynk.h"
#include <stdio.h>
#include <string.h>

/* Connection State Machine */
typedef enum {
  BLYNK_STATE_INIT = 0,
  BLYNK_STATE_RESET_ESP,
  BLYNK_STATE_SET_MODE,
  BLYNK_STATE_CONNECT_WIFI,
  BLYNK_STATE_WAIT_WIFI,
  BLYNK_STATE_START_TCP,
  BLYNK_STATE_WAIT_TCP,
  BLYNK_STATE_SEND_MQTT_CONNECT,
  BLYNK_STATE_WAIT_CONNACK,
  BLYNK_STATE_SEND_MQTT_SUB,
  BLYNK_STATE_ONLINE,
  BLYNK_STATE_DISCONNECTED
} BlynkState_t;

static BlynkState_t state = BLYNK_STATE_INIT;
static uint32_t state_timer = 0;
static uint8_t manual_override = 0;
static uint8_t blynk_connected = 0;
static uint8_t active_wifi_profile = 0; /* 0 = Home, 1 = Hotspot */
static uint32_t last_ping_time = 0;

/* Internal RX Line Buffer */
static char rx_line[128];
static uint8_t rx_idx = 0;

/* Outgoing AT send helper */
static void ESP_Send(const char *cmd)
{
  USART1_WriteString(cmd);
  USART1_WriteString("\r\n");
}

/* Send Raw Buffer via AT+CIPSEND */
static void ESP_SendRaw(const uint8_t *data, uint16_t len)
{
  char cmd[32];
  snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%u", len);
  ESP_Send(cmd);
  HAL_Delay(20); /* Brief pause for ESP '>' prompt */
  USART1_WriteBytes(data, len);
}

/* Build and Send MQTT CONNECT Packet */
static void Blynk_SendMqttConnect(void)
{
  const char *client_id = "stm32_room";
  const char *username = "device";
  const char *token = BLYNK_AUTH_TOKEN;

  uint8_t pkt[128];
  uint8_t idx = 0;

  /* Variable Header: 'MQTT' protocol, level 4, flags C2 (clean session + user + pass), keepalive 60s */
  uint8_t var_hdr[] = { 0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04, 0xC2, 0x00, 0x3C };
  uint8_t var_hdr_len = sizeof(var_hdr);

  uint8_t cid_len = (uint8_t)strlen(client_id);
  uint8_t u_len = (uint8_t)strlen(username);
  uint8_t t_len = (uint8_t)strlen(token);
  uint8_t payload_len = 2 + cid_len + 2 + u_len + 2 + t_len;
  uint8_t rem_len = var_hdr_len + payload_len;

  /* Fixed Header */
  pkt[idx++] = 0x10; /* CONNECT packet type */
  pkt[idx++] = rem_len;

  /* Variable Header */
  memcpy(&pkt[idx], var_hdr, var_hdr_len);
  idx += var_hdr_len;

  /* Client ID */
  pkt[idx++] = 0x00;
  pkt[idx++] = cid_len;
  memcpy(&pkt[idx], client_id, cid_len);
  idx += cid_len;

  /* Username */
  pkt[idx++] = 0x00;
  pkt[idx++] = u_len;
  memcpy(&pkt[idx], username, u_len);
  idx += u_len;

  /* Password (Auth Token) */
  pkt[idx++] = 0x00;
  pkt[idx++] = t_len;
  memcpy(&pkt[idx], token, t_len);
  idx += t_len;

  ESP_SendRaw(pkt, idx);
}

/* Build and Send MQTT SUBSCRIBE Packet to downlink/# */
static void Blynk_SendMqttSubscribe(void)
{
  const char *topic = "downlink/#";
  uint8_t top_len = (uint8_t)strlen(topic);
  uint8_t sub_payload_len = 2 + 2 + top_len + 1; /* msg_id(2) + top_len(2) + topic + qos(1) */

  uint8_t pkt[32];
  uint8_t idx = 0;

  pkt[idx++] = 0x82; /* SUBSCRIBE */
  pkt[idx++] = sub_payload_len;

  /* Packet ID = 1 */
  pkt[idx++] = 0x00;
  pkt[idx++] = 0x01;

  /* Topic String */
  pkt[idx++] = 0x00;
  pkt[idx++] = top_len;
  memcpy(&pkt[idx], topic, top_len);
  idx += top_len;

  /* Requested QoS 0 */
  pkt[idx++] = 0x00;

  ESP_SendRaw(pkt, idx);
}

/* MQTT PUBLISH to Datastream Topic */
static void Blynk_MqttPublish(const char *topic, const char *val_str)
{
  if (!blynk_connected) return;

  uint8_t top_len = (uint8_t)strlen(topic);
  uint8_t val_len = (uint8_t)strlen(val_str);
  uint8_t rem_len = 2 + top_len + val_len;

  uint8_t pkt[128];
  uint8_t idx = 0;

  pkt[idx++] = 0x30; /* PUBLISH QoS 0 */
  pkt[idx++] = rem_len;

  pkt[idx++] = 0x00;
  pkt[idx++] = top_len;
  memcpy(&pkt[idx], topic, top_len);
  idx += top_len;

  memcpy(&pkt[idx], val_str, val_len);
  idx += val_len;

  ESP_SendRaw(pkt, idx);
}

/* Send MQTT PINGREQ Keep-Alive */
static void Blynk_SendMqttPing(void)
{
  static const uint8_t ping_pkt[2] = { 0xC0, 0x00 };
  ESP_SendRaw(ping_pkt, 2);
}

void Blynk_Init(void)
{
  USART1_Init(115200);
  state = BLYNK_STATE_INIT;
  state_timer = HAL_GetTick();
  blynk_connected = 0;
  manual_override = 0;
  active_wifi_profile = 0;
}

void Blynk_VirtualWriteFloat(uint8_t pin, float value)
{
  if (!blynk_connected) return;
  char val_buf[32];
  char top_buf[32];
  int v_int = (int)value;
  int v_dec = (int)((value >= 0.0f ? (value - (float)v_int) : ((float)v_int - value)) * 100.0f);
  snprintf(val_buf, sizeof(val_buf), "%d.%02d", v_int, v_dec);
  snprintf(top_buf, sizeof(top_buf), "ds/v%d", pin);
  Blynk_MqttPublish(top_buf, val_buf);
}

void Blynk_VirtualWriteInt(uint8_t pin, int value)
{
  if (!blynk_connected) return;
  char val_buf[16];
  char top_buf[32];
  snprintf(val_buf, sizeof(val_buf), "%d", value);
  snprintf(top_buf, sizeof(top_buf), "ds/v%d", pin);
  Blynk_MqttPublish(top_buf, val_buf);
}

void Blynk_VirtualWriteString(uint8_t pin, const char *str)
{
  if (!blynk_connected) return;
  char top_buf[32];
  snprintf(top_buf, sizeof(top_buf), "ds/v%d", pin);
  Blynk_MqttPublish(top_buf, str);
}

uint8_t Blynk_GetManualOverride(void)
{
  return manual_override;
}

uint8_t Blynk_IsConnected(void)
{
  return blynk_connected;
}

const char* Blynk_GetStateStr(void)
{
  switch (state)
  {
    case BLYNK_STATE_INIT: return "INIT";
    case BLYNK_STATE_RESET_ESP: return "RST";
    case BLYNK_STATE_SET_MODE: return "MODE";
    case BLYNK_STATE_CONNECT_WIFI: return active_wifi_profile ? "JOIN:HOTSPOT" : "JOIN:HOME";
    case BLYNK_STATE_WAIT_WIFI: return active_wifi_profile ? "WAIT:HOTSPOT" : "WAIT:HOME";
    case BLYNK_STATE_START_TCP: return "TCP START";
    case BLYNK_STATE_WAIT_TCP: return "TCP WAIT";
    case BLYNK_STATE_SEND_MQTT_CONNECT: return "MQTT LOGIN";
    case BLYNK_STATE_WAIT_CONNACK: return "MQTT WAIT";
    case BLYNK_STATE_SEND_MQTT_SUB: return "MQTT SUB";
    case BLYNK_STATE_ONLINE: return "ONLINE";
    case BLYNK_STATE_DISCONNECTED: return "DISCONNECTED";
    default: return "UNKNOWN";
  }
}

static void Blynk_ProcessRxBytes(void)
{
  while (USART1_Available())
  {
    int c = USART1_ReadChar();
    if (c < 0) break;

    /* MQTT Downlink Detection: Downlink publishes start with topic containing 'v3' */
    if (c == '\n' || c == '\r')
    {
      if (rx_idx > 0)
      {
        rx_line[rx_idx] = '\0';

        /* Check for Wi-Fi or TCP responses */
        if (state == BLYNK_STATE_WAIT_WIFI && (strstr(rx_line, "WIFI GOT IP") || strstr(rx_line, "OK")))
        {
          state = BLYNK_STATE_START_TCP;
          state_timer = HAL_GetTick();
        }
        else if (state == BLYNK_STATE_WAIT_TCP && (strstr(rx_line, "CONNECT") || strstr(rx_line, "OK")))
        {
          state = BLYNK_STATE_SEND_MQTT_CONNECT;
          state_timer = HAL_GetTick();
        }
        else if (strstr(rx_line, "CLOSED") || strstr(rx_line, "FAIL"))
        {
          blynk_connected = 0;
          state = BLYNK_STATE_DISCONNECTED;
          state_timer = HAL_GetTick();
        }

        /* Check for Datastream V3 override in raw stream */
        if (strstr(rx_line, "v3") || strstr(rx_line, "V3") || strstr(rx_line, "Override"))
        {
          if (strchr(rx_line, '1'))
          {
            manual_override = 1;
          }
          else if (strchr(rx_line, '0'))
          {
            manual_override = 0;
          }
        }

        rx_idx = 0;
      }
    }
    else
    {
      /* Parse binary MQTT CONNACK: 0x20 0x02 0x00 0x00 */
      if ((uint8_t)c == 0x20 && state == BLYNK_STATE_WAIT_CONNACK)
      {
        blynk_connected = 1;
        state = BLYNK_STATE_SEND_MQTT_SUB;
        state_timer = HAL_GetTick();
        last_ping_time = HAL_GetTick();
      }

      if (rx_idx < (sizeof(rx_line) - 1))
      {
        rx_line[rx_idx++] = (char)c;
      }
    }
  }
}

void Blynk_Run(void)
{
  uint32_t now = HAL_GetTick();

  /* 1. Process all incoming UART characters */
  Blynk_ProcessRxBytes();

  /* 2. Connection State Machine */
  switch (state)
  {
    case BLYNK_STATE_INIT:
    case BLYNK_STATE_RESET_ESP:
      if (now - state_timer >= 1000U)
      {
        ESP_Send("AT");
        state = BLYNK_STATE_SET_MODE;
        state_timer = now;
      }
      break;

    case BLYNK_STATE_SET_MODE:
      if (now - state_timer >= 500U)
      {
        ESP_Send("AT+CWMODE=1");
        state = BLYNK_STATE_CONNECT_WIFI;
        state_timer = now;
      }
      break;

    case BLYNK_STATE_CONNECT_WIFI:
      if (now - state_timer >= 500U)
      {
        char cmd[128];
        if (active_wifi_profile == 0)
        {
          snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"", WIFI_SSID_HOME, WIFI_PASS_HOME);
        }
        else
        {
          snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"", WIFI_SSID_HOTSPOT, WIFI_PASS_HOTSPOT);
        }
        ESP_Send(cmd);
        state = BLYNK_STATE_WAIT_WIFI;
        state_timer = now;
      }
      break;

    case BLYNK_STATE_WAIT_WIFI:
      /* If Wi-Fi doesn't connect within 15 seconds, switch profile */
      if (now - state_timer >= 15000U)
      {
        active_wifi_profile = !active_wifi_profile;
        state = BLYNK_STATE_CONNECT_WIFI;
        state_timer = now;
      }
      break;

    case BLYNK_STATE_START_TCP:
      if (now - state_timer >= 500U)
      {
        /* Connect to Blynk Cloud MQTT Broker on Port 1883 */
        ESP_Send("AT+CIPSTART=\"TCP\",\"blynk.cloud\",1883");
        state = BLYNK_STATE_WAIT_TCP;
        state_timer = now;
      }
      break;

    case BLYNK_STATE_WAIT_TCP:
      if (now - state_timer >= 8000U)
      {
        /* TCP timed out, retry */
        state = BLYNK_STATE_START_TCP;
        state_timer = now;
      }
      break;

    case BLYNK_STATE_SEND_MQTT_CONNECT:
      if (now - state_timer >= 500U)
      {
        Blynk_SendMqttConnect();
        state = BLYNK_STATE_WAIT_CONNACK;
        state_timer = now;
      }
      break;

    case BLYNK_STATE_WAIT_CONNACK:
      if (now - state_timer >= 5000U)
      {
        /* Re-try TCP connect if CONNACK timed out */
        state = BLYNK_STATE_START_TCP;
        state_timer = now;
      }
      break;

    case BLYNK_STATE_SEND_MQTT_SUB:
      if (now - state_timer >= 300U)
      {
        Blynk_SendMqttSubscribe();
        state = BLYNK_STATE_ONLINE;
        state_timer = now;
        last_ping_time = now;
      }
      break;

    case BLYNK_STATE_ONLINE:
      /* Send MQTT PINGREQ every 30 seconds to maintain ONLINE status in Blynk Cloud */
      if (now - last_ping_time >= 30000U)
      {
        last_ping_time = now;
        Blynk_SendMqttPing();
      }
      break;

    case BLYNK_STATE_DISCONNECTED:
      if (now - state_timer >= 5000U)
      {
        state = BLYNK_STATE_START_TCP;
        state_timer = now;
      }
      break;
  }
}
