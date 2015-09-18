
.PHONY: all clean

CC=gcc
RM=rm
OBJS= demo.o handle.o cloud.o epoll.o buf.o list.o log.o pipe.o sig.o socket.o timer.o config.o

all: cloud client

client:
	$(CC) test/client.c -g -o test/client

cloud: $(OBJS)
	$(CC) -o cloud $(OBJS) -lpthread

clean:
	$(RM) -rf *.o test/client cloud
		
%.o: %.c
	$(CC) -c -g $<

