
.PHONY: all clean

CC=gcc
RM=rm
OBJS= demo.o cloud.o epoll.o list.o log.o pipe.o select.o sig.o socket.o timer.o 

all: cloud

cloud: $(OBJS)
	$(CC) -o cloud $(OBJS)

clean:
	$(RM) -rf *.obj cloud
		
%.o: %.c
	$(CC) -c $<