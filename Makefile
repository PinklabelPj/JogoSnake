CC = gcc
CFLAGS = -Wall -g
TARGET = snake

all: snake

snake: main.o snake.o maps.o
	$(CC) $(CFLAGS) -o snake main.o snake.o maps.o -lraylib -lm -lpthread -ldl -lrt -lX11
	
main.o: main.c snake.h
	 $(CC) $(CFLAGS) -c main.c

snake.o: snake.c snake.h
	$(CC) $(CFLAGS) -c snake.c

maps.o: maps.c maps.h
	$(CC) $(CFLAGS) -c maps.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
