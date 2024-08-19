CC=gcc
CFLAGS=-g -Wall -lpthread
LDFLAGS=-pthread
TARGET=parser.out
OBJS=parser.o main.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

parser.o : parser.c
	$(CC) -c -o parser.o parser.c $(CFLAGS)

main.o : main.c
	$(CC) -c -o main.o main.c $(CFLAGS)

clean:
	rm $(OBJS) $(TARGET)
