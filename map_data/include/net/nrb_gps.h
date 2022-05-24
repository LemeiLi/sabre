/*************************************************************************
	> Created Time: Wed 17 Jul 2019 10:44:47 AM CST
 ************************************************************************/

#ifndef _NRB_GPS_H
#define _NRB_GPS_H

#include "net/nrb_desc.h"
#include "consumer/common/cache_pool.h"
#include "utils/ulist.h"
#include "utils/fifo.h"
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nrb_gps_desc {
	struct nrb_desc base;
	struct list_head node;
	size_t size;     ///< real data size
	size_t cap;      ///< capacity 
	uint8_t data[0];
}nrb_gps_desc_t;

#define NRB_GPS_POOL_CAPABILITY (5*100)

//extern struct nrb_gps_desc* construct_nrb_gps(void* pdata, size_t data_size);
extern struct nrb_gps_desc* construct_nrb_gps(struct cache_pool* pool, void* pdata, size_t data_size);
extern void submit_nrb_gps(fifo_t* fifo, struct nrb_gps_desc* pdata);
extern void recycle_nrb_gps(struct cache_pool* pthis, struct nrb_gps_desc* pdata);
extern void destruct_nrb_gps(struct nrb_gps_desc** pthis);


#ifdef __cplusplus
}
#endif

#endif //  _NRB_GPS_H
