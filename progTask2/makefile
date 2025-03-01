in = kernel.c
out = libkernel.so
flags= -shared -O3 

all: build

build:
	gcc $(flags) $(in)  -o $(out) -lm
clean:
	rm $(out)

.PHONY: all build clean