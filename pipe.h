
#ifndef _PIPE_H
#define _PIPE_H

void init_pipe();
void write_pipe(s8* buf, u32 len);
void read_pipe(s8* buf, u32 len);
void close_pipe();

#endif
