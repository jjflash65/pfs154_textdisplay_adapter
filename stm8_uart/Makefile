############################################################
#
#                         Makefile
#
############################################################

PROJECT       = stxlcd_demo_uart

MCU           = stm8s103f3
#MCU           = stm8s105k4


# hier alle zusaetzlichen Softwaremodule angegeben
SRCS          = ./src/stm8_init.rel
SRCS         += ./src/stxlcd_uart.rel
SRCS         += ./src/uart.rel


INC_DIR       = -I./ -I./include


# FLASHERPROG Auswahl fuer STM8:
# 0 : stm8flash (ueber ST-Link Adapter)  1 : stm8_bootlash (Bootloader)

PROGPORT      = /dev/ttyUSB0
CH340RESET    = 0
FLASHERPROG   = 0


include ./makefile.mk
