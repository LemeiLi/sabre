/*************************************************************************
	> File Name: stream.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 17 Oct 2019 03:37:12 PM CST
 ************************************************************************/

#ifndef _STREAM_H
#define _STREAM_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAXPERLINE (1024*1024)
/* Read "n" bytes from a descriptor. */
extern ssize_t readn(int fd, void *vptr, size_t n);
/* Write "n" bytes to a descriptor. */
extern ssize_t writen(int fd, const void *vptr, size_t n);
/* Read a line of data */
extern ssize_t readline(int fd, void *vptr, size_t maxlen);
/* Read a line to internal buf */
extern ssize_t readlinebuf(void **vptrptr);
extern char* read_file(const char *filename);

#ifdef __cplusplus
}
#endif

#endif // _STREAM_H
