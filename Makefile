CC = gcc
FILES = src/main.c
OUT_EXE = superPi8 
LIBS = -lncurses


build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES) $(LIBS)

clean:
	rm -f *.o core

rebuild: clean build
