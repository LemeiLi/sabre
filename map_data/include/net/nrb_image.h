/*************************************************************************
	> Created Time: Tue 14 May 2019 05:29:31 PM CST
 ************************************************************************/

#ifndef _NRB_IMAGE_H
#define _NRB_IMAGE_H

#include <stdint.h>
#include "net/nrb_desc.h"
#include "utils/fifo.h"
#include "utils/ulist.h"
#include "consumer/common/cache_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NRB_IMG_POOL_CAPABILITY (1000)

struct net_image_desc {
	struct nrb_desc base;
	uint8_t* image;
	size_t size;
	double stamp;
}net_image_desc_t;

typedef struct nrb_image_desc {
	struct nrb_desc base;
	struct list_head node;
	uint8_t *data;
	size_t size;
	size_t cap;
	int32_t slice_number;
	int32_t index;
	uint32_t data_times;
	uint32_t data_timeu;
}nrb_image_desc_t;

extern void construct_nrb_image(struct cache_pool* cpool, struct net_image_desc* pimage, 
	void (*submit)(fifo_t* fifo, struct nrb_image_desc* pdata), fifo_t* fifo);
extern void submit_nrb_image(fifo_t* fifo, struct nrb_image_desc* pdata);
extern void recycle_nrb_image(struct cache_pool* cpool, struct nrb_image_desc* pdata);
extern void destruct_nrb_image(struct nrb_image_desc** block);

#ifdef __cplusplus
}
#endif

#endif // _NRB_IMAGE_H




