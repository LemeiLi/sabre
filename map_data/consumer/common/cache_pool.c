/*************************************************************************
	> Created Time: Sat 31 Aug 2019 11:02:44 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "consumer/common/cache_pool.h" 
#include "consumer/common/ntv_desc.h"
#include "net/nrb_gps.h"
#include "net/nrb_imu.h"
#include "net/nrb_odo.h"
#include "net/nrb_image.h"
#include "net/nrb_desc.h"
#include "utils/utils.h"

static struct cache_pool* construct_cache_pool(void)
{
	struct cache_pool* pthis = (struct cache_pool*)malloc(
			sizeof(struct cache_pool));
	if (pthis == NULL) {
		error("malloc cache_pool error\n");
		return NULL;
	}
	INIT_LIST_HEAD(&pthis->list);
	pthread_mutex_init(&pthis->mlock, NULL);
	pthis->nr = 0;
	log("construct cache pool [%p]!\n", pthis);

	return pthis;
}

static void clean_cache_pool_nrb(struct cache_pool* pthis, enum nrb_id id)
{
	struct list_head* ltemp = NULL, *lnext = NULL;
	if (pthis == NULL) return;
	if (list_empty(&pthis->list)) return;

	switch (id) {
		case NET_GPS_ID: {
			struct nrb_gps_desc* temp = NULL;
			pthread_mutex_lock(&pthis->mlock);
			list_for_each_safe(ltemp, lnext, &pthis->list) {
				temp = list_entry(ltemp, struct nrb_gps_desc, node);
				list_del_init(&temp->node);
				destruct_nrb_gps(&temp);
			}
			pthread_mutex_unlock(&pthis->mlock);
			return;	
		}
		case NET_IMU_ID: {
			struct nrb_imu_desc* temp = NULL;
			pthread_mutex_lock(&pthis->mlock);
			list_for_each_safe(ltemp, lnext, &pthis->list) {
				temp = list_entry(ltemp, struct nrb_imu_desc, node);
				list_del_init(&temp->node);
				destruct_nrb_imu(&temp);
			}
			pthread_mutex_unlock(&pthis->mlock);
			return;	
		}
		case NET_ODO_ID: {
			struct nrb_odo_desc* temp = NULL;
			pthread_mutex_lock(&pthis->mlock);
			list_for_each_safe(ltemp, lnext, &pthis->list) {
				temp = list_entry(ltemp, struct nrb_odo_desc, node);
				list_del_init(&temp->node);
				destruct_nrb_odo(&temp);
			}
			pthread_mutex_unlock(&pthis->mlock);
			return;	
		}
		case NET_IMAGE_ID: {
			struct nrb_image_desc* temp = NULL;
			pthread_mutex_lock(&pthis->mlock);
			list_for_each_safe(ltemp, lnext, &pthis->list) {
				temp = list_entry(ltemp, struct nrb_image_desc, node);
				list_del_init(&temp->node);
				destruct_nrb_image(&temp);
			}
			pthread_mutex_unlock(&pthis->mlock);
			return;	
		}
		default:
			error("clean_cache_pool_nrb exception!\n");
			return;
	}
}

void clean_cache_pool_ntv(struct cache_pool* pthis)
{
	struct ntv_desc* temp = NULL;
	struct list_head* ltemp = NULL, *lnext = NULL;
	if (pthis) {
		pthread_mutex_lock(&pthis->mlock);
		list_for_each_safe(ltemp, lnext, &pthis->list) {
			temp = list_entry(ltemp, struct ntv_desc, node);
			//log("nr = %d, addr = %p, cap = %ld!\n", pthis->nr, temp, temp->cap);
			list_del_init(&temp->node); 
			destruct_ntv_desc(&temp);
		}
		pthread_mutex_unlock(&pthis->mlock);
	}

	return;
}

void destruct_cache_pool(struct cache_pool** ppthis)
{
	if (*ppthis) {
		if (!list_empty(&(*ppthis)->list)) {
			error("you must free all entry on cache_pool list!\n");
		}
		log("destruct cache pool [%p]!\n", *ppthis);
		pthread_mutex_destroy(&(*ppthis)->mlock);
		free(*ppthis);
		*ppthis = NULL;
	}

	return;
}

struct cache_pool_container* construct_cpool_container(void)
{
	struct cache_pool_container* pthis = (struct cache_pool_container*)malloc(sizeof(*pthis));

	for (int i = 0; i < CPOOL_NUM; i++) {
		pthis->cpools[i] = construct_cache_pool();
	}

	return pthis;
}

///< 0, ntv, 1, nrb TODO
void destruct_cpool_container(struct cache_pool_container** ppthis, int flag)
{
	if (*ppthis != NULL) {
		if (flag) {
			clean_cache_pool_nrb((*ppthis)->gps_cpool, NET_GPS_ID);
			clean_cache_pool_nrb((*ppthis)->imu_cpool, NET_IMU_ID);
			clean_cache_pool_nrb((*ppthis)->odo_cpool, NET_ODO_ID);
			clean_cache_pool_nrb((*ppthis)->img_cpool, NET_IMAGE_ID);
			log("all nrb cache pools cleaned!\n");
		} else {
			for (int i = 0; i < CPOOL_NUM; i++) {
				clean_cache_pool_ntv((*ppthis)->cpools[i]); 
			}
			log("all ntv cache pools cleaned!\n");
		}
		for (int i = 0; i < CPOOL_NUM; i++) {
			destruct_cache_pool(&(*ppthis)->cpools[i]);
		}
		log("all cache pools destructed!\n");
		free(*ppthis);
		*ppthis = NULL;
	}

	return;
}





