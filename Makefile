
all: cairo

cairo: src/cairo.c src/common.h
	$(CC) src/cairo.c -o cairo $(shell pkg-config --cflags --libs sdl2 cairo)

