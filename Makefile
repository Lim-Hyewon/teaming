CC := gcc
CCFLAGS := 
GTKFLAGS := `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
LIBS := src/authorization/authorization.c src/board/board.c src/team/team.c src/gui/gui.c src/gui/event.c src/schedule/schedule.c

all: main

main: src/main.c
	$(CC) $(CCFLAGS) -o main src/main.c $(LIBS) $(GTKFLAGS)

# %.o: %.c
# 	$(CC) $(CCFLAGS) -c $(input) -o $(output)

clean:
	rm main