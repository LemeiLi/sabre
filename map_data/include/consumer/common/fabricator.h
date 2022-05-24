/*************************************************************************
	> File Name: fabricator.h
	> Created Time: Mon 01 Jul 2019 07:07:50 PM CST
 ************************************************************************/

#ifndef _FABRICATOR_H
#define _FABRICATOR_H

#include "channel.h"
#include "net/net_tcp_server.h"
#include "cache_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fabricator 
{
	pthread_t fthread;
	struct liver* pliver;
	struct channel* data_ch;
	struct recycler* prcc;
}fabricator_t;

extern struct fabricator* create_fabricator_thread(struct liver* pliver, struct channel* pch);
extern void di_recycler2fabricator(struct fabricator* pthis, struct recycler* pthat);
extern int destroy_fabricator(struct fabricator* pthis);
	
#ifdef __cplusplus
}
#endif

#endif // _FABRICATOR_H
