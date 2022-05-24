/*************************************************************************
	> Created Time: Wed 24 Jul 2019 05:44:24 PM CST
 ************************************************************************/

#include "net/nrb_odo.h" 
#include "nec_odo.h" 
#include "utils/utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void feedback_nrb_odo(struct cache_pool* cpool, struct nrb_odo_desc* pdata)
{
	pthread_mutex_lock(&cpool->mlock);
	if (cpool->nr < NRB_ODO_POOL_CAPABILITY) {
		list_add(&pdata->node, &cpool->list);
		cpool->nr++;
	} else {
		struct nrb_odo_desc* temp = NULL;
		temp = list_entry(cpool->list.prev, struct nrb_odo_desc, node);
		list_del(cpool->list.prev); ///< remove tail entry
		list_add(&pdata->node, &cpool->list); 
		if (temp) {
			destruct_nrb_odo(&temp);	
		}
	}
	pthread_mutex_unlock(&cpool->mlock);

	return;
}

struct nrb_odo_desc* pickup_nrb_odo(struct cache_pool* cpool, size_t size)
{
	struct nrb_odo_desc* pthis = NULL;
	struct list_head* temp, *next;

	pthread_mutex_lock(&cpool->mlock);
	list_for_each_safe(temp, next, &cpool->list) {
		pthis = list_entry(temp, struct nrb_odo_desc, node);
		if ((pthis->cap - NET_ODO_META_LEN) >= size) { ///< make sure capacity not less than we want.
			cpool->nr--;
			list_del(temp);
			break;
		} else {
			pthis = NULL;			
		}
	}
	pthread_mutex_unlock(&cpool->mlock);

	return pthis;
}

void recycle_nrb_odo(struct cache_pool* cpool, struct nrb_odo_desc* pdata)
{
	feedback_nrb_odo(cpool, pdata);

	return;
}

struct nrb_odo_desc* __construct_nrb_odo(void* pdata, size_t data_size)
{
	struct nrb_odo_desc* pthis = (struct nrb_odo_desc*)malloc(
		sizeof(struct nrb_odo_desc) + (NET_ODO_META_LEN + data_size));

	pthis->size = NET_ODO_META_LEN + data_size;
	pthis->cap = pthis->size;
	pthis->base.id = NET_ODO_ID; 
	pthis->base.fragement = false; 
	pthis->base.rfmap = pthis->data;
	//pthis->base.to_send = pthis->size;
	//nrb_odo_encoder(pthis, pdata, data_size);

	return pthis;
}

struct nrb_odo_desc* construct_nrb_odo(struct cache_pool* cpool, void* pdata, size_t data_size)
{
	struct nrb_odo_desc* pthis = NULL;

	if ((pthis = pickup_nrb_odo(cpool, data_size)) == NULL) {
		pthis = __construct_nrb_odo(pdata, data_size);
	} 
	pthis->base.to_send = NET_ODO_META_LEN + data_size;
	nrb_odo_encoder(pthis, pdata, data_size);
		
	return pthis;				
}

inline void submit_nrb_odo(fifo_t* fifo, struct nrb_odo_desc* pdata)
{
	fifo_push(fifo, pdata);
}

void destruct_nrb_odo(struct nrb_odo_desc** pthis)
{
	free(*pthis);
	*pthis = NULL;

	return;
}

