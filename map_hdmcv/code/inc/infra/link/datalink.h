/*************************************************************************
	> File Name: /home/lijianhua/xxdk/unit_test/oopc/sensor1021/infra/link/datalink.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Mon 21 Oct 2019 04:07:26 PM CST
 ************************************************************************/

#ifndef _DATALINK_H
#define _DATALINK_H

#include "infra/link/fifo.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define datalink_methods \
	int (*upload)(void* pthis, void* data); \
	void* (*download)(void* pthis, int32_t timeout_sec); \

#define datalink_property \
	fifo_t* link;

typedef struct datalink_vmt {
	datalink_methods
}datalink_vmt;

typedef struct datalink {
	datalink_property
	datalink_methods
	struct datalink_vmt* vptr;
}datalink;

extern datalink* construct_datalink(void* vptr);
extern void destruct_datalink(datalink* pthis);
extern int datalink_init(void* pthis, void* vptr);
extern void datalink_deinit(void* pthis);

#ifdef __cplusplus
}
#endif

#endif // _DATALINK_H
