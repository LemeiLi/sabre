/*************************************************************************
	> Created Time: Thu 25 Jul 2019 05:25:57 PM CST
 ************************************************************************/

#include "consumer/common/ntv_desc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"


static void feedback_ntv_desc(struct cache_pool* cpool, struct ntv_desc* pdata)
{
	pthread_mutex_lock(&cpool->mlock);
	if (cpool->nr < NTV_POOL_CAPABILITY) {
		list_add(&pdata->node, &cpool->list);
		cpool->nr++;
		//log("add ntv_desc to cache pool! nr = %d, id = %d\n", cpool->nr, pdata->id);
	} else {
		struct ntv_desc* temp = NULL;
		//log("full add ntv_desc to cache pool! nr = %d, id = %d\n", cpool->nr, pdata->id);
		temp = list_entry(cpool->list.prev, struct ntv_desc, node);
		list_del(cpool->list.prev);            ///< remove tail entry
		list_add(&pdata->node, &cpool->list);  ///< insert into head postion
		if (temp) {
			destruct_ntv_desc(&temp);	
		}
	}
	pthread_mutex_unlock(&cpool->mlock);

	return;
}

static struct ntv_desc* pickup_ntv_desc(struct cache_pool* cpool, size_t size)
{
	struct ntv_desc* pthis = NULL;
	struct list_head* temp, *next;

	pthread_mutex_lock(&cpool->mlock);
	list_for_each_safe(temp, next, &cpool->list) {
		pthis = list_entry(temp, struct ntv_desc, node);
		if ((pthis != NULL) && (pthis->cap >= size)) { ///< make sure capacity not less than we want.
			//log("pickup success! pthis->cap = %ld, size = %ld\n", pthis->cap, size);
			cpool->nr--;
			list_del(temp);
			pthis->size = size;   ///< update real data size member
			break;
		} else {
			pthis = NULL;			
		}
	}
	pthread_mutex_unlock(&cpool->mlock);

	return pthis;
}

void recycle_ntv_desc(struct cache_pool* cpool, struct ntv_desc* pdata)
{
	feedback_ntv_desc(cpool, pdata);

	return;
}


static struct ntv_desc* __construct_ntv_desc(size_t size, enum ntv_id id)
{
	struct ntv_desc* pthis = (struct ntv_desc*)malloc(sizeof(struct ntv_desc) + size);
	pthis->id = id;
	pthis->cap = size;
	pthis->size = size;

	return pthis;
}

struct ntv_desc* construct_ntv_desc(struct cache_pool* cpool, void* data, size_t size, enum ntv_id id)
{
	struct ntv_desc* pthis = NULL;

	if ((pthis = pickup_ntv_desc(cpool, size)) == NULL) {
		pthis = __construct_ntv_desc(size, id);
		if (pthis == NULL) {
			return NULL;
		}
		//log("new success! addr = %p, cap = %ld, size = %ld\n", pthis, pthis->cap, size);
	}
	memcpy(pthis->data, data, size);

	return pthis;
}

void destruct_ntv_desc(struct ntv_desc** ppthis) 
{
	if (*ppthis) {
		log("ntv desc object [%p] destructed!\n", *ppthis);
		free(*ppthis);
		*ppthis = NULL;
	}

	return;
}




