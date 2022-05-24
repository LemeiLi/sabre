/*************************************************************************
	> File Name: include/consumer/common/cache_pool.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Mon 02 Sep 2019 03:39:04 PM CST
 ************************************************************************/

#ifndef _CACHE_POOL_H
#define _CACHE_POOL_H

#include <stdint.h>
#include <pthread.h>
#include "utils/ulist.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cache_pool {
	struct list_head list;   ///< cached object list
	pthread_mutex_t mlock;
	uint32_t nr;
}cache_pool_t;

#define CPOOL_NUM	4
#define gps_cpool	cpools[0]
#define odo_cpool	cpools[1]
#define imu_cpool	cpools[2]
#define img_cpool	cpools[3]

typedef struct cache_pool_container {
	struct cache_pool* cpools[CPOOL_NUM]; 
}cache_pool_container_t;

extern struct cache_pool_container* construct_cpool_container(void);
extern void destruct_cpool_container(struct cache_pool_container** ppthis, int flag);

#ifdef __cplusplus
}
#endif

#endif // _CACHE_POOL_H




