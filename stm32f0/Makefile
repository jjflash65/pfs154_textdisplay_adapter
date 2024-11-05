PROJECT = stxlcd_demo_uart

# hier alle zusaetzlichen Softwaremodule angeben
SRCS            = ./src/sysf0xx_init.o
SRCS           += ./src/uart.o
SRCS           += ./src/stxlcd_uart.o

INC_DIR         = -I./ -I./include -I./src

LSCRIPT         = stm32f030x4.ld

# FLASHERPROG Auswahl fuer STM32:
# 0 : STLINK-V2, 1 :  stm32flash, 2 : DFU_UTIL

PROGPORT      = /dev/ttyUSB0
ERASEFLASH    = 0
FLASHERPROG   = 1

include ./lib/libopencm3.mk
