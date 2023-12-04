all: main test final

main: board.c
	gcc board.c -o main `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

test: test.c
	gcc test.c -o test `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

final: final.c
	gcc final.c -o final `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

clean:
	rm main test final