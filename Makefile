# Compilers and tools
CC := arm-none-eabi-gcc
AS := arm-none-eabi-as
LD := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy

# Paths
BUILD_DIR := build
TARGET := $(BUILD_DIR)/payload

# Source and object files
SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Flags
CFLAGS := -std=gnu99 -Os -mthumb -mcpu=cortex-a9 -fno-builtin-printf -fno-strict-aliasing
CFLAGS += -fno-builtin-memcpy -mno-unaligned-access -DPRINTF_DISABLE_SUPPORT_FLOAT=1 -fPIE
LDFLAGS := -T src/linker.x -nodefaultlibs -nostdlib

ifeq ($(KAERU_DEBUG),1)
$(warning Debug mode enabled)
CFLAGS += -DDEBUG
endif

# Default target
all: $(TARGET).bin

$(TARGET).bin: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $(TARGET).elf $(OBJECTS)
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

# To compile any .c to .o
$(BUILD_DIR)/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
