###COPIED FROM THE MEGA MAKEFILE AND CHANGED PARAMETERS

#the name of your .c file 
NAME := main

#wherever you want your compilation related files and output binaries
TEMPDIR := .temp

#your chosen general path for libraries.
#this is handled later in sources. the idea is to just have a clear directory structure.
#other source folders can be added in a similar way by just adding a new pathsubst
LIBPATH := arduinolibs

LIBSOURCES := $(wildcard $(LIBPATH)/*.c)# 	all .c files in your lib path
SOURCES :=  $(NAME).c $(LIBSOURCES)# 		all .c files you might want to compile
HEADERS := $(wildcard $(LIBPATH)/*.h)#		all .h files you have
OBJECTS := $(patsubst $(LIBPATH)/%.c,$(TEMPDIR)/%.o,$(LIBSOURCES)) $(TEMPDIR)/$(NAME).o
#patsubst usage: .c file pattern, .o file pattern, list of .c files
#needlessly complicated but I wanted a subfolder to make the project structure a little bit less cluttered.

#arduino specs
MCU := atmega328p			#avr-gcc argument
MCU_AVRDUDE := ATMEGA328P	#the argument is different for avrdude
PARTNO := stk500v2			#default programmer, can be switched to 'arduino'
DEVICE := /dev/ttyUSB0		#semi-consistent device path. linux is different than windows.
BAUDRATE := 115200
AVRDUDE_FLAGS := -F -V -D -v

#compilation settings
CC := avr-gcc
OBJCOPY := avr-objcopy
SIZE := avr-size -A
CFLAGS := -Wall -pedantic -mmcu=$(MCU) -std=c99 -g -Os -DF_CPU=16000000UL


### boring stuff ###

#compilation mid steps to make avrdude work
#may not be strictly necessary in new versions of avrdude
HEX := $(COMPDIR)/$(NAME).hex
OUT := $(COMPDIR)/$(NAME).out
MAP := $(COMPDIR)/$(NAME).map


all: $(HEX)

print:
	@echo $(SOURCES)
	@echo $(HEADERS)
	@echo $(OBJECTS2)

clean:
	rm -f $(HEX) $(OUT) $(MAP) $(OBJECTS)

#may need sudo
flash: $(HEX) 
	avrdude $(AVRDUDE_FLAGS) -c $(PARTNO) -p $(MCU_AVRDUDE) -P $(DEVICE) -b $(BAUDRATE) -U flash:w:$(HEX)

$(HEX): $(OUT)
	$(OBJCOPY) -R .eeprom -O ihex $< $@

$(OUT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ -Wl,-Map,$(MAP) $^
	@echo
	@$(SIZE) $@
	@echo

%.o: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) -c -o $@ $<

%.pp: %.c
	$(CC) $(CFLAGS) -E -o $@ $<

%.ppo: %.c
	$(CC) $(CFLAGS) -E $<

test:
	echo "TEST POTATO XDXD XD"