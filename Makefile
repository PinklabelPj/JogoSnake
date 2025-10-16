CC = gccmake
CFLAGS = -Wall -g
TARGET = snake

all: snake

snake: main.o snake.o
	$(CC) $(CFLAGS) -o snake main.o snake.o -lraylib -lm
	
main.o: main.c snake.h
	 $(CC) $(CFLAGS) -c main.c

snake.o: snake.c snake.h
	$(CC) $(CFLAGS) -c snake.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean