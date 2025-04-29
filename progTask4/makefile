filename = main.c
output = main
flags = 
lib =  -lpthread -lrt
build: $(filename)
	gcc $(flags) $(filename) -o $(output) $(lib)

run: build
	./$(output)
clean:
	rm -f $(output)
