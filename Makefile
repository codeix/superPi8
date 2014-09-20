CC = gcc
FILES = src/main.c
OUT_EXE = superPi8 

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES)

clean:
	rm -f *.o core

rebuild: clean build
