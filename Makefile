CC = gcc
FILES = src/main.c src/scanner.c
OUT_EXE = superPi8 
LIBS = -lncurses -lnewt


build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES) $(LIBS)

clean:
	rm -f *.o core

rebuild: clean build
