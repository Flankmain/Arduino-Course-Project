# "megaflash" instead of "mega flash" because of makefile warnings about overriding previous recipes:
#	makefile:13: warning: overriding recipe for target 'mega'
#	makefile:8: warning: ignoring old recipe for target 'mega'

MEGAMAKE = "MEGA/makefile
UNOMAKE = UNO/makefile

all: mega, uno, megaflash, unoflash
buildall: mega, uno

flashall: megaflash, unoflash

mega:
	@echo "Compiling mega..."
	make all -C MEGA/ -f makefile
	@echo "/nDone compiling MEGA!"

megaflash:
	@echo "Flashing mega..."
	make flash -C MEGA/ -f makefile
	@echo "/nDone flashing MEGA!"

megaclean:
	@echo "Cleaning mega..."
	make flash -C MEGA/ -f makefile
	@echo "/nDone cleaning MEGA!"

uno:
	@echo "Compiling uno..."
	make flash -C UNO/ -f makefile
	@echo "/nDone compiling UNO!"

unoflash:
	@echo "Flashing uno..."
	make flash -C UNO/ -f makefile
	@echo "/nDone flashing UNO!"

unoclean:
	@echo "Cleaning uno..."
	make clean -C UNO/ -f makefile
	@echo "/nDone cleaning UNO!"

test:
	@echo "TEST FUNCTION PRINTS!"