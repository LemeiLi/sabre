/*************************************************************************
	> Created Time: Wed 24 Jul 2019 04:56:24 PM CST
 ************************************************************************/

#include "nec_gps.h"
#include "net/nrb_gps.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

void feedback_nrb_gps(struct cache_pool* pthis, struct nrb_gps_desc* pdata)
{
	pthread_mutex_lock(&pthis->mlock);
	if (pthis->nr < NRB_GPS_POOL_CAPABILITY) {
		list_add(&pdata->node, &pthis->list);
		pthis->nr++;
	} else {
		struct nrb_gps_desc* temp = NULL;
		temp = list_entry(pthis->list.prev, struct nrb_gps_desc, node);
		list_del(pthis->list.prev); ///< remove tail entry
		list_add(&pdata->node, &pthis->list); 
		if (temp) {
			destruct_nrb_gps(&temp);
		}
	}
	pthread_mutex_unlock(&pthis->mlock);

	return;
}

struct nrb_gps_desc* pickup_nrb_gps(struct cache_pool* pthis, size_t size)
{
	struct nrb_gps_desc* gps = NULL;
	struct list_head* temp, *next;

	pthread_mutex_lock(&pthis->mlock);
	list_for_each_safe(temp, next, &pthis->list) {
		gps = list_entry(temp, struct nrb_gps_desc, node);
		if ((gps->cap - NET_GPS_META_LEN) >= size) { ///< make sure capacity not less than we want.
			pthis->nr--;
			list_del(temp);
			break;
		} else {
			pthis = NULL;
		}
	}
	pthread_mutex_unlock(&pthis->mlock);

	return gps;
}

void recycle_nrb_gps(struct cache_pool* pthis, struct nrb_gps_desc* pdata)
{
	feedback_nrb_gps(pthis, pdata);

	return;
}

static struct nrb_gps_desc* __construct_nrb_gps(void* pdata, size_t data_size)
{
	struct nrb_gps_desc* pthis = (struct nrb_gps_desc*)malloc(
		sizeof(struct nrb_gps_desc) + (NET_GPS_META_LEN + data_size));
	if (pthis == NULL) {
		error("malloc nrb_gps_desc error\n");
		return NULL;
	}

	pthis->size = NET_GPS_META_LEN + data_size;
	pthis->cap = pthis->size;
	pthis->base.id = NET_GPS_ID; 
	pthis->base.fragement = false; 
	pthis->base.rfmap = pthis->data;
	//pthis->base.to_send = pthis->size;
	//nrb_gps_encoder(pthis, pdata, data_size);

	return pthis;
}

struct nrb_gps_desc* construct_nrb_gps(struct cache_pool* cpool, void* pdata, size_t data_size)
{
	struct nrb_gps_desc* pthis = NULL;

	if ((pthis = pickup_nrb_gps(cpool, data_size)) == NULL) {
		pthis = __construct_nrb_gps(pdata, data_size);
	} 
	pthis->base.to_send = NET_GPS_META_LEN + data_size;
	nrb_gps_encoder(pthis, pdata, data_size);
		
	return pthis;				
}

inline void submit_nrb_gps(fifo_t* fifo, struct nrb_gps_desc* pdata)
{
	fifo_push(fifo, pdata);
}

void destruct_nrb_gps(struct nrb_gps_desc** ppthis) 
{
	if (*ppthis != NULL) {
		free(*ppthis);
		*ppthis = NULL;
	}

	return;
}

