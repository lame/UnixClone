.PHONY: docs test

clone.x: clone.c
	gcc clone.c -o clone.x
	rm -f clone.o

clear:
	rm -f clone.x 
	rm -f clone
	rm -f clone.o

