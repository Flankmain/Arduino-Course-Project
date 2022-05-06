# "megaflash" instead of "mega flash" because of makefile warnings about overriding previous recipes:
#	makefile:13: warning: overriding recipe for target 'mega'
#	makefile:8: warning: ignoring old recipe for target 'mega'

all:
	$(MAKE) mega
	$(MAKE) uno
	$(MAKE) megaflash
	$(MAKE) unoflash

clean:
	$(MAKE) megaclean
	$(MAKE) unoclean

buildall:
	$(MAKE) mega
	$(MAKE) uno

flashall:
	$(MAKE) megaflash
	$(MAKE) unoflash

mega:
	@echo "Compiling mega..."
	make all -C MEGA/ -f makefile
	@echo "Done compiling MEGA!"

megaflash:
	@echo "Flashing mega..."
	make flash -C MEGA/ -f makefile
	@echo "Done flashing MEGA!"

megaclean:
	@echo "Cleaning mega..."
	make clean -C MEGA/ -f makefile
	@echo "Done cleaning MEGA!"

uno:
	@echo "Compiling uno..."
	make all -C UNO/ -f makefile
	@echo "Done compiling UNO!"

unoflash:
	@echo "Flashing uno..."
	make flash -C UNO/ -f makefile
	@echo "Done flashing UNO!"

unoclean:
	@echo "Cleaning uno..."
	make clean -C UNO/ -f makefile
	@echo "Done cleaning UNO!"

test:
	@echo "TEST FUNCTION PRINTS!"