
.PHONY: all clean

CC=gcc
RM=rm
AR=ar
OBJS= rcv.o send.o cloud.o epoll.o list.o log.o pipe.o sig.o socket.o timer.o config.o io.o
LIBS= cloud
DEMOS= demo
CLIENT= client

all: $(LIBS) $(CLIENT)
	$(CC) -I. -c -o demos/demo.o demos/demo.c
	$(CC) -o demos/demo demos/demo.o libcloud.a

$(CLIENT):
	$(CC) test/client.c -g -o test/client

$(LIBS): $(OBJS)
	$(AR) rc libcloud.a $(OBJS)

clean:
	$(RM) -rf *.o *.a test/client demos/*.o demos/demo
		
%.o: %.c
	$(CC) -c -g $<

