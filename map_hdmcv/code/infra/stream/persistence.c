/*************************************************************************
	> File Name: persistence.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 18 Oct 2019 07:52:22 PM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include "infra/log/log.h"
#include "infra/stream/persistence.h"

#pragma pack(1)

typedef struct __avc_sei_t
{
    char head[4];             	// 00 00 00 01
    char nal_header;         	// 06
    char sei_payload_type;     	// 05
    char sei_payload_size;     	// 32
    char sei_payload_uuid[16];
    struct
    {
        uint64_t timestamp;
        int32_t exposure;
        int32_t reserve;
    }payload;
    char rbsp_tailing_bit;      // 80
}avc_sei_t;

#pragma pack()

static void analysis_timestamp(void* data)
{
	struct __avc_sei_t* acv = (struct __avc_sei_t*)	(data + 6);
	static uint64_t last_ts = 0;
	static uint32_t epts = 0;


	uint64_t diff = acv->payload.timestamp - last_ts;
	if (diff > 70000 || diff < 60000) {
		fprintf(stderr, "diff = %lu\n", diff);
		epts++;
	}
	fprintf(stderr, "epts %u\n", epts);
	last_ts = acv->payload.timestamp;

	return;
}

void persistence(const char* name, void* buf, size_t size, int flag) 
{
	size_t wcount = 0; 
	char filename[100] = {0};
	sprintf(filename, "%s/%s", "/media/lijianhua/08C9068C115CF85E/data", name);
	//sprintf(filename, "%s", name);
	
	int fdw = open(filename, flag, 0666);
	if (fdw < 0) {
		zlog_error(g_zc, "open %s failed", filename);
		return;
	}
	analysis_timestamp(buf);
	while (wcount < size) {
		ssize_t ret = write(fdw, buf+wcount, size-wcount);
		if (ret > 0)   
			wcount += (size_t)ret;
		else if (ret < 0) {
			zlog_error(g_zc, "write error: %s", strerror(errno));
			fprintf(stderr, "write error: %s\n", strerror(errno));
			close(fdw);
			return;
		}
	}
	fsync(fdw);
	close(fdw);

	return;
}



