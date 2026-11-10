# STM32 Smart Room IoT Controller

An automated room environment and occupancy controller powered by the **STM32F103C6T6 (ARM Cortex-M3)** microcontroller. 

The system continuously monitors ambient room temperature and human occupancy, displays real-time status on a 16x2 I2C LCD, automatically activates an appliance relay (fan/light), and transmits live telemetry to both a local real-time Web Dashboard and the **Blynk 2.0 Cloud** platform via an ESP8266 Wi-Fi coprocessor.

---

## Key Features

- **Non-Blocking 1-Wire FSM**: Accurately samples DS18B20 12-bit temperature without halting the CPU during the 750 ms conversion window.
- **Bit-Bang I2C LCD Driver**: Custom open-drain bit-bang driver for PCF8574 backpacks on PB6/PB7 with periodic register sanitization (prevents display corruption).
- **Debounced Occupancy Detection**: HC-SR501 PIR digital sensor with multi-cycle software debounce to filter electrical noise.
- **Smart Relay Automation**: Active-LOW 5V relay control with dual trigger logic:
  - **Automatic Mode**: Turns ON when human motion is detected.
  - **Manual Mode**: Remote app override via Blynk virtual pin `V3` or local dashboard.
- **Dual Wi-Fi Auto-Failover**: Automatically attempts primary network (Home Wi-Fi) and fails over to secondary network (Mobile Hotspot).
- **Blynk 2.0 IoT Cloud Integration**: Native MQTT packet engine sending telemetry (`V0`: Temp, `V1`: PIR, `V2`: Relay, `V3`: Override).
- **Local Web Dashboard & SWD Gateway**: Python gateway that reads live microcontroller RAM registers via ST-Link SWD in real time and serves an interactive web control panel.

---

## Hardware Pinout & Wiring

| STM32 Pin | Connected Component | Function | Description |
|-----------|--------------------|----------|-------------|
| **PA1**   | DS18B20            | 1-Wire Data | Ambient temperature sensing (Requires 4.7kΩ pull-up to 3.3V) |
| **PA0**   | HC-SR501 PIR       | Digital In  | Occupancy / Motion detection (Internal pull-down) |
| **PB6**   | 16x2 LCD (PCF8574) | I2C SCL     | Clock line (Open-drain, external/internal pull-up) |
| **PB7**   | 16x2 LCD (PCF8574) | I2C SDA     | Data line (Open-drain, external/internal pull-up) |
| **PB12**  | 5V Relay Module    | Relay IN1   | Active-LOW relay trigger (LOW = ON, HIGH = OFF) |
| **PC13**  | Onboard Blue Pill  | Status LED  | Fast blink on motion detected, OFF on standby |
| **PA9**   | ESP8266 (ESP-12F)  | USART1 TX   | Connects to ESP8266 RX pin |
| **PA10**  | ESP8266 (ESP-12F)  | USART1 RX   | Connects to ESP8266 TX pin |
| **3.3V**  | VCC                | Power       | Power supply for STM32, DS18B20, and ESP8266 |
| **5.0V**  | VCC                | Power       | Power supply for Relay Module, LCD backlight, PIR |
| **GND**   | GND                | Ground      | Common ground across all modules |

---

## Project Structure

```
├── Core/
│   ├── Inc/
│   │   ├── blynk.h             # Blynk 2.0 MQTT protocol & Wi-Fi configuration
│   │   ├── ds18b20.h           # 1-Wire temperature driver header
│   │   ├── i2c_lcd.h           # HD44780 PCF8574 LCD backpack header
│   │   ├── main.h              # System clock and hardware definitions
│   │   ├── pir.h               # HC-SR501 PIR sensor header
│   │   ├── relay.h             # Active-LOW relay control header
│   │   └── usart.h             # USART1 register-level driver header
│   ├── Src/
│   │   ├── blynk.c             # MQTT state machine & ESP AT communication
│   │   ├── ds18b20.c           # Microsecond 1-Wire bit-bang implementation
│   │   ├── i2c_lcd.c           # Software I2C master & multi-screen display loop
│   │   ├── main.c              # Main superloop & peripheral integration
│   │   ├── pir.c               # GPIO input reader & debouncer
│   │   ├── relay.c             # GPIO output driver for relay coil
│   │   ├── usart.c             # Ring buffer buffered serial driver
│   │   └── system_stm32f1xx.c  # CMSIS clock tree setup
│   └── Startup/
│       └── startup_stm32f103c6tx.s
├── Drivers/                    # STM32 HAL & CMSIS core drivers
├── web/
│   └── index.html              # Modern dark-mode real-time web dashboard
├── server.py                   # Python SWD RAM reader & Web Dashboard server
├── Makefile                    # GNU ARM Embedded Toolchain build file
├── STM32F103C6TX_FLASH.ld      # Linker script for STM32F103C6 (32KB Flash, 10KB RAM)
└── README.md
```

---

## Build & Flash

### Prerequisites
- **ARM GNU Toolchain**: `arm-none-eabi-gcc`, `arm-none-eabi-size`, `arm-none-eabi-objcopy`
- **Make**: GNU Make
- **ST-Link Programmer**: STM32CubeProgrammer or OpenOCD

### Compile Firmware
```bash
make -j4 all
```
The build produces `build/led_blonking.elf`, `build/led_blonking.hex`, and `build/led_blonking.bin`.

### Flash to Blue Pill (via ST-Link)
```bash
STM32_Programmer_CLI -c port=SWD mode=UR reset=HWRST -w build/led_blonking.bin 0x08000000 -v -rst
```

---

## Local Web Dashboard & Hardware Gateway

The repository includes a Python gateway (`server.py`) that communicates directly with the STM32 over the ST-Link SWD port, polls internal memory registers, and serves an interactive control panel:

```bash
python server.py
```

Then open your browser at:
```
http://localhost:8080
```

### Dashboard Features:
- Real-time live temperature display (°C and °F).
- Dynamic PIR occupancy radar with visual motion indicator.
- Interactive fan status with SVG spinning animation and manual override toggle.
- Live system event log.

---

## Author

**Gopal M V**  
GitHub: [@gopalmvofficial](https://github.com/gopalmvofficial)

---

## License

MIT License. Free to use for educational and commercial embedded systems development.
