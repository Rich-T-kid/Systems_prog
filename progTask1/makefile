# first comments
#
CC=gcc
fileName=pa1.c
outputName=pa1
csv=test2.csv
plane=c
value=100
run: build execute
build:
	@echo "Building"
	$(CC) $(fileName) -o $(outputName)
execute:
	@echo "Executing"
	./$(outputName) $(csv)  $(plane)  $(value)

clean:
	rm -f $(outputName)



