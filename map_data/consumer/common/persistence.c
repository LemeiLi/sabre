/*************************************************************************
	> Created Time: Wed 04 Sep 2019 02:51:24 PM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "utils/utils.h"

///< TODO
void read_jpg_file(const char* name, uint8_t** ppdata, size_t* size)
{
	static size_t len;
	static int fd = -1;
	static uint8_t* pdata = NULL; 

	if (fd == -1) {
		if ((fd = open(name, O_RDONLY, 0777)) < 0) {
			error("open file: %s error\n", name);
			return;
		}
		len = lseek(fd, 0L, SEEK_END);
		lseek(fd, 0L, SEEK_SET);
	}
	
	if (pdata == NULL) {
		pdata = malloc(len);
		read(fd, pdata, len);
		*size = len;
		*ppdata = pdata;
		close(fd);
	}

	return;
}

int write_jpg_file(const char* name, const void* data, size_t size)
{
	int fd, ret;

	log("save image to '%s', size = %ld\n", name, size);
	if ((fd = open(name, O_CREAT | O_TRUNC | O_WRONLY, 0777)) < 0) {
		error("open file: %s error\n", name);
		return -1;
	}

	if ((ret = write(fd, data, size)) < 0) {
		error("write image to file error\n");
		return -1;
	}
	close(fd);

	return 0;
}




