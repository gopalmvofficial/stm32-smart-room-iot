/**
  ******************************************************************************
  * @file           : blynk.h
  * @brief          : Blynk 2.0 MQTT Protocol & Wi-Fi Configuration
  * @author         : Gopal M V
  ******************************************************************************
  */

#ifndef __BLYNK_H
#define __BLYNK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "usart.h"

/* Blynk 2.0 Configuration Credentials */
#define BLYNK_TEMPLATE_ID   "TMPL3bjGdLblN"
#define BLYNK_TEMPLATE_NAME "Smart Room System"
#define BLYNK_AUTH_TOKEN    "aAh-BPN6uDPH_IVzDTJmbUbvVhVkppsi"

/* Primary Home Wi-Fi */
#define WIFI_SSID_HOME      "FTTH-C52C"
#define WIFI_PASS_HOME      "12345678"

/* Secondary Mobile Hotspot (College / On-The-Go) */
#define WIFI_SSID_HOTSPOT   "iphone"
#define WIFI_PASS_HOTSPOT   "1234567890"

/* Blynk Virtual Pin Mapping */
#define VIRTUAL_PIN_TEMP        0  /* V0: Temperature Readout (°C) */
#define VIRTUAL_PIN_PIR         1  /* V1: Motion Status (0=Clear, 1=Motion) */
#define VIRTUAL_PIN_RELAY       2  /* V2: Relay State (0=OFF, 1=ON) */
#define VIRTUAL_PIN_OVERRIDE    3  /* V3: Remote Fan Switch (0=Auto, 1=Manual ON) */
#define VIRTUAL_PIN_STATUS      4  /* V4: System Status Message */

/* Function Prototypes */
void Blynk_Init(void);
void Blynk_Run(void);
void Blynk_VirtualWriteFloat(uint8_t pin, float value);
void Blynk_VirtualWriteInt(uint8_t pin, int value);
void Blynk_VirtualWriteString(uint8_t pin, const char *str);
uint8_t Blynk_GetManualOverride(void);
uint8_t Blynk_IsConnected(void);
const char* Blynk_GetStateStr(void);

#ifdef __cplusplus
}
#endif

#endif /* __BLYNK_H */
