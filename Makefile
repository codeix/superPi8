CC = gcc
FILES = src/main.c src/scanner.c src/imagegrab.c
OUT_EXE = superPi8 
LIBS = -lncurses -lnewt -lv4l2
MACROS = -DIO_MMA


build: $(FILES)
	$(CC) -w -o $(OUT_EXE) $(FILES) $(LIBS) $(MACROS)

clean:
	rm -f *.o core

rebuild: clean build
