/*************************************************************************
	> Created Time: Sat 27 Jul 2019 04:25:24 PM CST
 ************************************************************************/

#ifndef _RECYCLER_H
#define _RECYCLER_H
#include "consumer/common/channel.h"
#ifdef __cplusplus
extern "C" {
#endif

struct recycler 
{
	pthread_t ntv_rthread;
	pthread_t nrb_rthread;
	struct channel* monitor_ch;
	struct cache_pool_container* ntv_cpool_ctner;
	struct cache_pool_container* nrb_cpool_ctner;
}recycler_t;

extern struct recycler* create_recycler_thread(struct channel* pch);
extern void destroy_recycler(struct recycler** ppthis);

#ifdef __cplusplus
}
#endif

#endif // _RECYCLER_H
