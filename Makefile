FILE = *.hex *.rel *.mem *.map *.lst *.rst *.ihx *.lk *.sym *.asm

main.o: 
	sdcc -c keypad.c
	sdcc -c LED_Display.c
	sdcc  eOrgan-107321047.c  keypad.rel LED_Display.rel
	packihx eOrgan-107321047.ihx > eOrgan-107321047.hex

clean:
	del $(FILE)