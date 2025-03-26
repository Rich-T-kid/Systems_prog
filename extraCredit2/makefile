input = kernel.c
output = libkernel.so
flags = -shared -O3
mathLib = -lm

all: build 

build:
	gcc $(flags) $(input) -o $(output) $(mathLib)

run:
	./$(output)

clean:
	rm -f $(output)
