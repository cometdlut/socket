
cloud socket lib

=====

# about this lib
    There are alreay many socket libs on the web, but few of them match my needs in my personal perspective. So I try to design one for myself. I do this just to let us have a through understanding of what socket is, and how it can help us to do various things.

# prerequisites

> c language
> makefile
> socket knowledge
> about non block io

#  compile procedure

	make
	./cloud

# introduction to various files

	log.c    -- about logs recording
	epoll.c  -- epoll mechanism
	kqueue.c -- kqueue mechanism
	list.c   -- basic structure
	cloud.c  -- entry of module
	pipe.c   -- about pipe function
	select.c -- about select mechanism
	sig.c    -- about signal
	socket.c -- about basic socket operation
	timer.c  -- about timer function
	thread.c -- about thread function
	module.c -- about module function


# introduction to folders

	demos  -- show how to use cloud socket lib
	doc    -- basic procedure in cloud socket lib
	script -- script used to taks lan and wan test
	test   -- unit test code
	third  -- libs from third party sides

