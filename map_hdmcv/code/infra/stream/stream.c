/*************************************************************************
	> File Name: stream.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 17 Oct 2019 03:16:44 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "infra/stream/stream.h"
#include "infra/log/log.h"

/* Read "n" bytes from a descriptor. */
ssize_t readn(int fd, void *vptr, size_t n)
{
	size_t  nleft;
	ssize_t nread;
	char   *ptr;

	ptr = (char *)vptr;
	nleft = n;
	while (nleft > 0) {
		if ((nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR || EAGAIN) {
				zlog_error(g_zc, "read %d again: %s", fd, strerror(errno));
				nread = 0;          /* and call read() again */
			}
			else {
				zlog_error(g_zc, "read %d falied: %s", fd, strerror(errno));
				return (-1);
			}
		} else if (nread == 0)
			break;              /* EOF */

		nleft -= (size_t)nread;
		ptr += nread;

		//zlog_error(g_zc, "nread %ld, error %s", nread, strerror(errno));
	}
	return (ssize_t)(n - nleft);         /* return >= 0 */
}

/* Write "n" bytes to a descriptor. */
ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = (const char *)vptr;
	nleft = n;
	while (nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && (errno == EINTR || errno == EAGAIN))
				nwritten = 0;   /* and call write() again */
			else {
				zlog_error(g_zc, "write %d falied: %s", fd, strerror(errno));
				return (-1);    /* error */
			}
		}

		nleft -= (size_t)nwritten;
		ptr += nwritten;
	}

	return (ssize_t)(n);
}

static ssize_t read_cnt;
static char *read_ptr;
static char read_buf[MAXPERLINE];

static ssize_t my_read(int fd, char *ptr)
{
	if (read_cnt <= 0) {
	again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR || errno == EAGAIN)
				goto again;
			else {
				zlog_error(g_zc, "read %d falied: %s", fd, strerror(errno));
				return (-1);
			}
		} else if (read_cnt == 0)
			return (0);

		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return (1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	size_t n;
	ssize_t rc;
	char    c, *ptr;

	ptr = (char *)vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c  == '\n')
				break;          /* newline is stored, like fgets() */
		} else if (rc == 0) {
			*ptr = 0;
			return (ssize_t)(n - 1);     /* EOF, n - 1 bytes were read */
		} else
			return (-1);        /* error, errno set by read() */
	}

	*ptr  = 0;                  /* null terminate like fgets() */
	return (ssize_t)(n);
}
		
ssize_t readlinebuf(void **vptrptr)
{
	if (read_cnt)
		*vptrptr = read_ptr;
	return (read_cnt);
}

/**
 * @brief read file into a allocated memory
 * @note need to free after use 
 *
 * @param[in] filename the file to read
 * @return    the memory that contained the file
 */
char* read_file(const char *filename) 
{
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;

    /* open in read binary mode */
    file = fopen(filename, "rb");
    if (file == NULL)
        goto cleanup;

    /* get the length */
    if (fseek(file, 0, SEEK_END) != 0)
        goto cleanup;

    length = ftell(file);
    if (length < 0)
        goto cleanup;

    if (fseek(file, 0, SEEK_SET) != 0)
        goto cleanup;

    /* allocate content buffer */
    content = (char*)malloc((size_t)length + sizeof(""));
    if (content == NULL)
        goto cleanup;

    /* read the file into memory */
    read_chars = fread(content, sizeof(char), (size_t)length, file);
    if ((long)read_chars != length) {
        free(content);
        content = NULL;
        goto cleanup;
    }
    content[read_chars] = '\0';

cleanup:
    if (file != NULL)
        fclose(file);

    return content;
}


