/*************************************************************************
	> File Name: cache_pool.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 28 Aug 2019 02:27:15 PM CST
 ************************************************************************/

#ifndef _CACHE_POOL_H
#define _CACHE_POOL_H

#include <stdint.h>
#include <pthread.h>
#include "utils/rbtree.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cache_rb_root {
	struct rb_root root;
	size_t nr_node;
	pthread_mutex_t mlock;
}cache_rb_root_t;

extern struct cache_rb_root* construct_cache_rb_root(void);
extern void destroy_cache_rb_root(struct cache_rb_root** ppthis);

#ifdef __cplusplus
}
#endif

#endif // _CACHE_POOL_H
