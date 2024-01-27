main:
	avr-gcc -g -Os -mmcu=attiny2313a -c main.c
	avr-gcc -g -Os -mmcu=attiny2313a -c lib/eeprom.c
	avr-gcc -g -Os -mmcu=attiny2313a -c lib/display.c
	avr-gcc -g -Os -mmcu=attiny2313a -c lib/buttons.c
	avr-gcc -g -Os -mmcu=attiny2313a -c lib/i2c.c
	avr-gcc -g -Os -mmcu=attiny2313a -c lib/clock.c
	avr-gcc -g -Os -mmcu=attiny2313a -c lib/settings.c
	avr-gcc -g -mmcu=attiny2313a -o main.elf main.o eeprom.o display.o buttons.o i2c.o clock.o settings.o
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avrdude -c usbasp -p t2313 -P usb -U flash:w:main.hex