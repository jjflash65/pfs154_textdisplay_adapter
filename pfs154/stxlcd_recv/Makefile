############################################################
#
#                         Makefile
#
############################################################

PROJECT       = stxlcd_recv
MCU           = PFS154
MEMORG        = pdk14
F_CPU         = 8000000
FACTORYCAL    = 1

# hier alle zusaetzlichen Softwaremodule angegeben
SRCS          = ./src/delay.rel

INC_DIR       = -I./ -I./include

# Pfad zum Programmer und zum Programm "pfsreadhex"

TOOLPATH      = ./tools

# benutzbare Programmer:
#  1 : easypdkprogrammer  ==> serielle Portangabe kann frei bleiben
#  2 : pfsprog            ==> benoetigt serielle Portangabe

PROGRAMMER    = 2
SERPORT       = /dev/ttyUSB0
#SERPORT       = /dev/ttyACM0

include ./makefile.mk
