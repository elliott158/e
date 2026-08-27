
all: cairo

cairo: src/cairo.c
	$(CC) src/cairo.c -o cairo $(shell pkg-config --cflags --libs sdl2 cairo)

