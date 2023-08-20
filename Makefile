main:
	avr-gcc -g -Os -mmcu=attiny2313a -c main.c
	avr-gcc -g -mmcu=attiny2313a -o main.elf main.o
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avrdude -c usbasp -p t2313 -P usb -U flash:w:main.hex