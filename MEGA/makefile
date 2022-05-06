###FILE CONFIGURATION
#the name of your .c file 
NAME := mega_I2C_master_basics

###COMPILATION MIDSTEPS
#these try to improve avrdude's filetype compatibility
#this may not be strictly necessary in new versions of avrdude
HEX := $(NAME).hex
OUT := $(NAME).out
MAP := $(NAME).map

###SOURCES
SOURCES := $(wildcard *.c)
HEADERS := $(wildcard *.h)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))

###ARDUINO CONFIGURATION
MCU := atmega2560
MCU_AVRDUDE := m2560

#programmer type can be 'arduino' or 'stk500v2'
PROGRAMMER := stk500v2

#semi-consistent device path. linux is different than windows.
DEVICE := /dev/ttyACM0
BAUDRATE := 115200
AVRDUDE_FLAGS := -F -V -D -v

###COMPILATION SETTINGS
GCC := avr-gcc
OBJCOPY := avr-objcopy
SIZE := avr-size -A

#COMPILATION FLAGS
#-Wall is just good practice
#-g turns on debug. I don't know if this is needed.
#-Os is on for stability and for very precise timings to work properly.
CFLAGS := -Wall -pedantic -mmcu=$(MCU) -std=c99 -g -Os -DF_CPU=16000000UL


###make definitions
all: $(HEX)
	@echo MAKING ALL IN /MEGA

clean:
	rm -f $(HEX) $(OUT) $(MAP) $(OBJECTS)

flash: $(HEX)
	avrdude $(AVRDUDE_FLAGS) -c $(PROGRAMMER) -p $(MCU_AVRDUDE) -P $(DEVICE) -b $(BAUDRATE) -U flash:w:$(HEX)

$(HEX): $(OUT)
	$(OBJCOPY) -R .eeprom -O ihex $< $@

$(OUT): $(OBJECTS)
	$(GCC) $(CFLAGS) -o $@ -Wl,-Map,$(MAP) $^
	@echo
	@$(SIZE) $@
	@echo

%.o: %.c $(HEADERS)
	$(GCC) $(CFLAGS) -c -o $@ $<

%.pp: %.c
	$(GCC) $(CFLAGS) -E -o $@ $<

%.ppo: %.c
	$(GCC) $(CFLAGS) -E $<