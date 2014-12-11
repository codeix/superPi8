CC = gcc
FILES = src/main.c src/scanner.c src/capture.c
OUT_EXE = superPi8 
LIBS = -lncurses -lnewt -lv4l2 -lpthread -lwiringPi
MACROS = -DIO_MMA


build: $(FILES)
	$(CC) -w -o $(OUT_EXE) $(FILES) $(LIBS) $(MACROS)

clean:
	rm -f *.o core

rebuild: clean build
