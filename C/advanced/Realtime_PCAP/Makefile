CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpcap
TARGET = capture_traffic
SRCS = main.c define.c pcap_header.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	     $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
