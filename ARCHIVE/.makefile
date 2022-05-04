TARGET = main.c
NAME = binary

OBJECT = $(NAME).o
BIN = $(NAME).bin
HEX = $(NAME).hex

MCU = atmega2560
MCU_AVRDUDE = m2560
MEGA_ADDRESS = /dev/ttyACM0

BAUDRATE = 115200

CC = avr-gcc
CFLAGS = -Wall -pedantic -g -0s -W1 -mmcu=$(MCU) -DF_CPU=16000000UL


default:

	@echo "===making object==="
	$(CC) $(CFLAGs) -c -o $(OBJECT) $(TARGET)
	@echo ""

	@echo "===making binary==="
	$(CC) -o $(BIN) $(OBJECT)
	@echo ""
	
	@echo "===turning object file to hex==="
	avr-objcopy -O ihex -R .eeprom $(BIN) $(HEX)
	@echo ""

	@echo "===flashing==="
	sudo avrdude -F -V -c arduino -p $(MCU_AVRDUDE) -P $(MEGA_ADDRESS) -b $(BAUDRATE) -U flash:w:$(HEX)
	@echo ""

	@echo "Successful flash!"