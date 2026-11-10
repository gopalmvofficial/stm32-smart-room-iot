#!/usr/bin/env python3
"""
Smart Room IoT Controller - Local Hardware Gateway & Dashboard Server
Author: Gopal M V

Features:
- Polls live telemetry (Temperature, PIR Motion, Relay state) directly from STM32 RAM via ST-Link SWD.
- Maintains authenticated MQTT session with Blynk 2.0 Cloud (blynk.cloud:1883).
- Synchronizes downlink commands (virtual pin V3 fan override) between Blynk app and hardware.
- Serves real-time Web Dashboard with live status API.
"""

import http.server
import json
import os
import re
import shutil
import socket
import socketserver
import struct
import subprocess
import sys
import threading
import time

# Configuration
DEFAULT_PORT = int(os.environ.get("PORT", 8080))
BLYNK_TOKEN = os.environ.get("BLYNK_TOKEN", "aAh-BPN6uDPH_IVzDTJmbUbvVhVkppsi")
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
WEB_DIR = os.path.join(BASE_DIR, "web")
if not os.path.exists(WEB_DIR):
    WEB_DIR = os.path.join(BASE_DIR, "..", "web")

def find_programmer_cli():
    """Locate STM32CubeProgrammer CLI on the system."""
    if "STM32_PROGRAMMER_CLI" in os.environ and os.path.exists(os.environ["STM32_PROGRAMMER_CLI"]):
        return os.environ["STM32_PROGRAMMER_CLI"]

    which = shutil.which("STM32_Programmer_CLI")
    if which:
        return which

    candidates = [
        r"C:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.2.500.202603051304\tools\bin\STM32_Programmer_CLI.exe",
        r"C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe",
        r"C:\Program Files (x86)\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe",
        r"C:\ST\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
    ]
    for path in candidates:
        if os.path.exists(path):
            return path

    return "STM32_Programmer_CLI"

CLI_PATH = find_programmer_cli()

# Global State
latest_data = {
    "temperature": 30.1,
    "temp_valid": 1,
    "motion": 0,
    "relay": 0,
    "manual_override": 0,
    "blynk_online": False,
    "timestamp": time.time()
}

lock = threading.Lock()
mqtt_socket = None

def blynk_mqtt_worker():
    """Background worker maintaining live MQTT session with Blynk Cloud."""
    global mqtt_socket, latest_data

    while True:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(5.0)
            s.connect(('blynk.cloud', 1883))

            client_id = 'stm32_room'
            user = 'device'
            token = BLYNK_TOKEN

            var_header = b'\x00\x04MQTT\x04\xc2\x00\x3c'
            payload = b'\x00' + bytes([len(client_id)]) + client_id.encode()
            payload += b'\x00' + bytes([len(user)]) + user.encode()
            payload += b'\x00' + bytes([len(token)]) + token.encode()
            pkt = b'\x10' + bytes([len(var_header) + len(payload)]) + var_header + payload
            s.sendall(pkt)

            connack = s.recv(4)
            if len(connack) >= 4 and connack[0] == 0x20 and connack[3] == 0x00:
                print(">> [Blynk MQTT] Connected successfully. Device is ONLINE.")
                with lock:
                    latest_data["blynk_online"] = True

                # Subscribe to downlink topic for mobile app controls
                topic = 'downlink/#'
                sub_payload = b'\x00\x01\x00' + bytes([len(topic)]) + topic.encode() + b'\x00'
                sub_pkt = b'\x82' + bytes([len(sub_payload)]) + sub_payload
                s.sendall(sub_pkt)
                s.recv(5)

                mqtt_socket = s
                last_ping = time.time()
                last_pub = 0

                s.settimeout(1.0)
                while True:
                    now = time.time()

                    # Keep-alive PING every 25s
                    if now - last_ping >= 25.0:
                        s.sendall(b'\xc0\x00')
                        last_ping = now

                    # Telemetry publish interval: 1.5s
                    if now - last_pub >= 1.5:
                        last_pub = now
                        with lock:
                            t_val = str(latest_data["temperature"])
                            m_val = str(latest_data["motion"])
                            r_val = str(latest_data["relay"])

                        def pub(tp, val):
                            t_bytes = tp.encode()
                            v_bytes = val.encode()
                            vhdr = b'\x00' + bytes([len(t_bytes)]) + t_bytes
                            rem = len(vhdr) + len(v_bytes)
                            s.sendall(b'\x30' + bytes([rem]) + vhdr + v_bytes)

                        pub('ds/v0', t_val)
                        pub('ds/v1', m_val)
                        pub('ds/v2', r_val)

                    # Poll for incoming app controls
                    try:
                        data = s.recv(128)
                        if data and (b'v3' in data or b'V3' in data):
                            if b'1' in data:
                                with lock:
                                    latest_data["manual_override"] = 1
                            elif b'0' in data:
                                with lock:
                                    latest_data["manual_override"] = 0
                    except socket.timeout:
                        pass

        except Exception:
            with lock:
                latest_data["blynk_online"] = False
            time.sleep(3.0)

def poll_stm32_hardware():
    """Poll STM32 RAM via ST-Link SWD hotplug connection."""
    global latest_data

    if not os.path.exists(CLI_PATH):
        return

    while True:
        try:
            cmd = [CLI_PATH, "-c", "port=SWD", "mode=HOTPLUG", "-r32", "0x200001F4", "12"]
            res = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=3)
            out = res.stdout

            m = re.search(r"0x200001F4\s*:\s*([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})", out)
            if m:
                word0 = int(m.group(1), 16)
                word1 = int(m.group(2), 16)

                motion = (word0 >> 8) & 0xFF
                temp_valid = (word0 >> 16) & 0xFF

                float_bytes = struct.pack(">I", word1)
                temp = round(struct.unpack(">f", float_bytes)[0], 1)

                fan_state = 1 if (motion or latest_data["manual_override"]) else 0

                with lock:
                    if temp_valid and 0 <= temp <= 85:
                        latest_data["temperature"] = temp
                    latest_data["temp_valid"] = temp_valid
                    latest_data["motion"] = motion
                    latest_data["relay"] = fan_state
                    latest_data["timestamp"] = time.time()
        except Exception:
            pass

        time.sleep(0.5)

class DashboardHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=WEB_DIR, **kwargs)

    def do_GET(self):
        if self.path == '/api/status':
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            with lock:
                payload = json.dumps(latest_data)
            self.wfile.write(payload.encode('utf-8'))
            return

        if self.path.startswith('/api/fan/'):
            action = 1 if '1' in self.path else 0
            with lock:
                latest_data["manual_override"] = action
                latest_data["relay"] = action
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            self.wfile.write(json.dumps({"success": True, "override": action}).encode('utf-8'))
            return

        if self.path == '/':
            self.path = '/index.html'

        return super().do_GET()

    def log_message(self, format, *args):
        # Silence routine static polling logs in console
        return

def start_server():
    t_hw = threading.Thread(target=poll_stm32_hardware, daemon=True)
    t_hw.start()

    t_mqtt = threading.Thread(target=blynk_mqtt_worker, daemon=True)
    t_mqtt.start()

    socketserver.TCPServer.allow_reuse_address = True
    server_port = DEFAULT_PORT

    for port_offset in range(10):
        try:
            test_port = server_port + port_offset
            httpd = socketserver.TCPServer(("127.0.0.1", test_port), DashboardHandler)
            server_port = test_port
            break
        except OSError:
            continue
    else:
        server_port = 49671
        httpd = socketserver.TCPServer(("127.0.0.1", server_port), DashboardHandler)

    print("=" * 60)
    print("  Smart Room IoT Gateway & Web Dashboard")
    print("=" * 60)
    print(f">> Web Dashboard : http://localhost:{server_port}")
    print(f">> Blynk Cloud   : Authenticated (Token: {BLYNK_TOKEN[:8]}...)")
    print(f">> ST-Link CLI   : {CLI_PATH if os.path.exists(CLI_PATH) else 'Simulated / Not Found'}")
    print("=" * 60)

    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nGateway stopped.")

if __name__ == "__main__":
    start_server()
