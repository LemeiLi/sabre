/*************************************************************************
	> Created Time: Wed 17 Jul 2019 04:24:05 PM CST
 ************************************************************************/

#ifndef _NRB_ODO_H
#define _NRB_ODO_H

#include "net/nrb_desc.h"
#include "utils/ulist.h"
#include "utils/fifo.h"
#include "consumer/common/cache_pool.h"
#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NRB_ODO_POOL_CAPABILITY (5*100)

typedef struct nrb_odo_desc {
	struct nrb_desc base;
	struct list_head node;
	size_t size;	
	size_t cap;
	uint8_t data[0];
}nrb_odo_desc_t;

extern struct nrb_odo_desc* construct_nrb_odo(struct cache_pool* cpool, void* pdata, size_t data_size);
extern void submit_nrb_odo(fifo_t* fifo, struct nrb_odo_desc* pdata);
extern void recycle_nrb_odo(struct cache_pool* cpool, struct nrb_odo_desc* pdata);
extern void destruct_nrb_odo(struct nrb_odo_desc** pthis);

#ifdef __cplusplus
}
#endif

#endif //  _NRB_ODO_H
