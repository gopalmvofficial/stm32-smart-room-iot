# Target MCU
TARGET = led_blonking
MCU = -mcpu=cortex-m3 -mthumb

# Target device definition
C_DEFS = -DSTM32F103x6 -DUSE_HAL_DRIVER

# Source directories & files
C_SOURCES = \
Core/Src/main.c \
Core/Src/ds18b20.c \
Core/Src/pir.c \
Core/Src/relay.c \
Core/Src/i2c_lcd.c \
Core/Src/usart.c \
Core/Src/blynk.c \
Core/Src/syscalls.c \
Core/Src/stm32f1xx_it.c \
Core/Src/stm32f1xx_hal_msp.c \
Core/Src/system_stm32f1xx.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c

ASM_SOURCES = \
Core/Startup/startup_stm32f103c6tx.s

# Includes
C_INCLUDES = \
-ICore/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IDrivers/CMSIS/Include

# Compiler & Linker flags
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) -O2 -Wall -fdata-sections -ffunction-sections
ASFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) -Wall -fdata-sections -ffunction-sections

LDSCRIPT = STM32F103C6TX_FLASH.ld
LDFLAGS = $(MCU) -specs=nano.specs -u _printf_float -specs=nosys.specs -T$(LDSCRIPT) -Wl,--gc-sections -Wl,-Map=build/$(TARGET).map

# Build Directory
BUILD_DIR = build

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
