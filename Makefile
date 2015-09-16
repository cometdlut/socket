
.PHONY: all clean

CC=gcc
RM=rm
OBJS= demo.o handle.o cloud.o epoll.o buf.o list.o log.o pipe.o sig.o socket.o timer.o

all: cloud

cloud: $(OBJS)
	$(CC) -o cloud $(OBJS)

clean:
	$(RM) -rf *.o cloud
		
%.o: %.c
	$(CC) -c -g $<

