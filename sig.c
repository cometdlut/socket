/*
 * Copyright 2015 tristan_fei <feixiaoxing@163.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "type.h"
#include "sig.h"
#include "log.h"
#include "pipe.h"

int cloud_terminate;

// check if we need to cutdown system

STATUS check_cloud_system() {

	return (cloud_terminate == 1) ? TRUE : FALSE;
}

// ctrl + c function

static void stub(int no) {

	cloud_terminate = 1;

	return;
}

// timer function

static void process(int no) {

	write_pipe("", 1);
}

// signal init function

void sig_init() {

	sigset_t all_mask;
	sigset_t new_mask;
	sigset_t old_mask;

	// init cloud_terminate value

	cloud_terminate = 0;

	// first mask all signal

	sigfillset(&all_mask);
	sigprocmask(SIG_SETMASK, &all_mask, &old_mask);

	// add signal mask

	sigemptyset(&new_mask);

	sigaddset(&new_mask, SIGINT);
	sigaddset(&new_mask, SIGALRM);
	sigprocmask(SIG_UNBLOCK, &new_mask, &old_mask);

	// register signal handler

	signal(SIGINT, stub);
	signal(SIGALRM, process);
}


