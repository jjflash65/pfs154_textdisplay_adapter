# ---------------------------------------------------------------
#                           makefile.mk
#
#    gemeinsames Makefile fuer alle STM8-Projekte, wird von
#    den entsprechenden Makefiles der Projekte includiert
#
#    Die Makefiles der Projekte enthalten lediglich den
#    Quelldateinamen des Programms aus dem ein auf dem
#    STM8 lauffaehiges Programm generiert werden soll
#
#    August 2016    R. Seelig
#
# ---------------------------------------------------------------

# Pfad zu Include Dateien
INCLUDE_PATH = $(INC_DIR)
ifeq ($(INC_DIR),)
	INCLUDE_PATH = -I./ -I../include
endif

# Compilersymbole (defines)
CC_SYMBOLS   =-DF_CPU=16000000

# -----------------------------------------------------------------------------------------------------
#  hier endet das "User-Interface des Makefiles und es sollte  ab hier nur mit Bedacht Aenderungen
#  vorgenommen werden !!!
# -----------------------------------------------------------------------------------------------------

# Typename Flasherprogramm fuer den STLINK/V2 (Default ist stm8s103f3)
DEVICE_NAME = $(MCU)
ifeq ($(MCU),)
	DEVICE_NAME  = stm8s103f3
endif

# Pfad zu Bibliothekn
LIBSPEC      =-lstm8 -mstm8


OBJS         = $(PROJECT).rel $(ADD_LIBS) $(SRCS)

#CC_FLAGS     =--std-c99 --opt-code-size --disable-warning 197 --disable-warning 84 --disable-warning 185
CC_FLAGS     =--std-c99 --opt-code-size --disable-warning 197 --disable-warning 84 --disable-warning 185 --disable-warning 85
CC_FLAGS    +=-D__$(MCU)__

CC = sdcc
LD = sdld

TOOLPATH     = ./tools/

.PHONY: all compile clean flash complete

all: clean $(OBJS)
	@echo "Linking $(PROJECT).c with libs, Intel-Hex-File: $(PROJECT).ihx" 1>&2
	$(CC) $(LIBSPEC) $(INCLUDE_PATH) $(CC_FLAGS) --out-fmt-ihx $(OBJS)
	@echo "  " 1>&2
	@echo " ------ Programm build sucessfull -----" 1>&2
	@echo "  " 1>&2
	@$(TOOLPATH)st8readihx $(DEVICE_NAME) $(PROJECT).ihx

compile:
	$(CC) $(LIBSPEC) $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDE_PATH) $(PROJECT).c -c $(PROJECT).rel

clean:
	@rm -f *.asm
	@rm -f *.rst
	@rm -f *.ihx
	@rm -f *.rel
	@rm -f *.sym
	@rm -f *.lst
	@rm -f *.map
	@rm -f *.cdb
	@rm -f *.lk
	@rm -f *.mem
	@rm -f ./src/*.asm
	@rm -f ./src/*.rst
	@rm -f ./src/*.rel
	@rm -f ./src/*.sym
	@rm -f ./src/*.lst
	@rm -f ./src/*.map
	@rm -f ./src/*.cdb
	@rm -f ./src/*.lk
	@rm -f ./src/*.mem

	@echo "Cleaning done..."

%.rel: %.c
	$(CC) $(LIBSPEC) $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDE_PATH) -c $< -o $@

flash:
ifeq ($(FLASHERPROG), 0)
	$(TOOLPATH)stm8flash -c stlinkv2 -p $(DEVICE_NAME) -w $(PROJECT).ihx
endif

ifeq ($(FLASHERPROG), 1)
	$(TOOLPATH)stm8_bootflash2 $(DEVICE_NAME) /dev/ttyUSB0 $(PROJECT).ihx notxbar 1>&2
endif

ifeq ($(FLASHERPROG), 2)
	$(TOOLPATH)stm8_bootflash2 $(DEVICE_NAME) /dev/ttyUSB0 $(PROJECT).ihx
endif

complete: clean all flash

