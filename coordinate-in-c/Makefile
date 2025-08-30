PKG = $(shell pkg-config --cflags --libs )

install:
	cc -Wall -Wextra $(PKG) -lm -o main main.c

run:
	./main
