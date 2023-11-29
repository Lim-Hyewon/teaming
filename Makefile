all: main

main: board.c
	gcc board.c -o main `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

clean:
	rm main