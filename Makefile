CC = gcc
CFLAGS = -Wall -ansi -pedantic -Wextra -g -pg -std=gnu99
LFLAGS = -lm -pthread `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image
DEPS = main.h functions.h
OBJ = main.o functions.o input.o main.o ui.o
OUT_EXE = mgq

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT_EXE): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	rm -f *.o $(OUT_EXE)

install:
	cp ./$(OUT_EXE) /bin/

rebuild: clean
	 make
