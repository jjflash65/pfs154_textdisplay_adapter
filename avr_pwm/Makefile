###############################################################################
#
#                                 Makefile
#
#   einfaches Makefile zum "builden" von HEX-Dateien fuer Atmel (c) AVR-
#   Mikrocontroller.
#
#
#
#   Mai 2017,  R. Seelig
#
###############################################################################


PROJECT           = stxlcd_demo

SRCS             = ./stxlcd.o

INCLUDE_PATHS     = -I. -I../include


PRINTF_FL         = 0
SCANF_FL          = 0
MATH              = 0

# fuer Compiler / Linker
FREQ              = 16000000ul
MCU               = atmega168

# fuer AVRDUDE
PROGRAMMER        = arduino
PROGPORT           = /dev/ttyUSB0
BRATE             = 115200
DUDEOPTS          = B1

CH340RESET        = 0


include ./makefile.mk

