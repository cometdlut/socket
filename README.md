
#cloud socket lib

## about this lib
    This lib is just to show you how to make full use of non block socket io function.

## prerequisites

1. c language
2. makefile
3. socket knowledge
4. about non block io

## compile procedure

	make
	./cloud

## introduction to various files

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


## introduction to folders

	demos  -- show how to use cloud socket lib
	doc    -- basic procedure in cloud socket lib
	script -- script used to taks lan and wan test
	test   -- unit test code
	third  -- libs from third party sides

