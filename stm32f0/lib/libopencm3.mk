# ---------------------------------------------------------------
#                          makefile.mk
#     funktionaler Bestandteil eines Makefiles zum Uebersetzen
#     von Programmen fuer einen STM32F030
#
#     Diese Datei sollte vom Makefile des Projektes aufgerufen
#     werden
# ---------------------------------------------------------------

LIBOPENCM3DIR ?= ./lib/libopencm3

SOC ?= STM32F0

CFLAGS  = -std=gnu99 -Wall -Os -g
CFLAGS += -mcpu=cortex-m0 -mthumb
CFLAGS += -nostartfiles -ffreestanding
CFLAGS += -ffunction-sections -fdata-sections -flto

ifeq ($(INC_DIR),)
	INC_DIR    := -I./ -I../include
endif

TOOLPATH = './tools/'

CFLAGS += -I$(LIBOPENCM3DIR)/include
# CFLAGS += -I/usr/local/gcc-arm-none-eabi/arm-none-eabi/include
CFLAGS += $(INC_DIR)

CFLAGS += -L$(LIBOPENCM3DIR)/lib
CFLAGS += -L$(LIBOPENCM3DIR)/../
CFLAGS += -D$(SOC)

AS      = arm-none-eabi-as
CC      = arm-none-eabi-gcc
CPP     = arm-none-eabi-g++
LD      = arm-none-eabi-gcc
SIZE    = arm-none-eabi-size
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

LDFLAGS += -lopencm3_stm32f0 -Wl,--gc-sections -flto -lm -lc -lgcc -lnosys

ifeq ($(PROGPORT),)
	PROGPORT   := /dev/ttyUSB0
endif

ifeq ($(LSCRIPT),)
	LSCRIPT   := stm32f030x6.ld
endif

.PHONY: all lib size flash list clean cleanlib

all: clean $(PROJECT).elf size

lib:
	make -C $(LIBOPENCM3DIR)

$(PROJECT).elf: $(PROJECT).c $(SRCS) | lib
	$(CC) $(CFLAGS)  -o $@ -T $(LSCRIPT) $^ $(LDFLAGS)
	$(OBJCOPY) -O binary $(PROJECT).elf $(PROJECT).bin

$(PROJECT).list: $(PROJECT).elf
	$(OBJDUMP) -D $< > $(PROJECT).list

size: $(PROJECT).elf
	$(SIZE)  $(PROJECT).elf 1>&2

flash: $(PROJECT).bin

####################################################################

# STLINK-V2


ifeq ($(FLASHERPROG), 0)

ifeq ($(ERASEFLASH), 1)
	st-flash erase
endif
	st-flash write $< 0x8000000
endif

####################################################################

# STLINK-V2


ifeq ($(FLASHERPROG), 0b)

	STM32_Programmer_CLI -c port=SWD mode=UR -q -d $< 0x8000000 -g 1>&2
endif

####################################################################

# serieller Bootloader mit Aktivierung ueber an  RTS / DTR ange-
# schlossenen /rst und boot0 Leitungen

ifeq ($(FLASHERPROG), 1)

	$(TOOLPATH)stm32flash -w $< -b 115200 -g 0 $(PROGPORT)
endif

####################################################################

# DFU-UTIL Bootloader

ifeq ($(FLASHERPROG), 2)
	$(TOOLPATH)dfu-util -a0 -s 0x8000000 -D $<
endif

####################################################################

# STM32CubeProgrammer SWD

ifeq ($(FLASHERPROG), 3)
	STM32_Programmer_CLI -c port=SWD -w $< 0x8000000 -s
endif

####################################################################


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f ./src/*.o

cleanlib: clean
	rm -f $(LIBOPENCM3DIR)/lib/stm32/f0/*.d
	rm -f $(LIBOPENCM3DIR)/lib/stm32/f0/*.o
