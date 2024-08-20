CC = gcc
CFLAGS = -g -Wall -lpcap
LDFLAGS = -lpcap
TARGET = pcap.out
OBJS = define.o main.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

define.o: define.c
	$(CC) -c -o define.o define.c $(CFLAGS)

main.o: main.c
	$(CC) -c -o main.o main.c $(CFLAGS)

clean:
	rm $(OBJS) $(TARGET)
