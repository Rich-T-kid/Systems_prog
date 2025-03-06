out = main
in = main.c

all: build run

build:
	gcc $(in) -o $(out) -lm

run:
	./$(out)

clean:
	rm -f $(out)

.PHONY: all build run clean
