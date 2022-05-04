# "megaflash" instead of "mega flash" because of makefile warnings about overriding previous recipes:
#	makefile:13: warning: overriding recipe for target 'mega'
#	makefile:8: warning: ignoring old recipe for target 'mega'

all: mega, uno, flash mega, flash uno
buildall: mega, uno
flashall: megaflash, unoflash

mega:
	@echo "Compiling mega..."
	make -C MEGA/
	@echo "/nDone compiling MEGA!"

megaflash:
	@echo "Flashing mega..."
	make flash -C MEGA/
	@echo "/nDone flashing MEGA!"

megaclean:
	make clean -C MEGA/
	@echo "/nDone cleaning MEGA!"

uno:
	@echo "Compiling uno..."
	make -C UNO/
	@echo "/nDone compiling UNO!"

unoflash:
	@echo "Flashing uno..."
	make flash -C UNO/
	@echo "/nDone flashing UNO!"

unoclean:
	make clean -C UNO/
	@echo "/nDone cleaning UNO!"

test:
	@echo "/nTEST FUNCTION PRINTS!"