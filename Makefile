
all: cairo

cairo: src/cairo.c src/common.h
	$(CC) src/cairo.c -o cairo.e $(shell pkg-config --cflags --libs sdl2 cairo)

terminal: src/terminal.c
	$(CC) src/terminal.c -o terminal.e
