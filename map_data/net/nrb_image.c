/*************************************************************************
	> Created Time: Wed 24 Jul 2019 05:07:57 PM CST
 ************************************************************************/

#include "nec_image.h"
#include "net/nrb_image.h"
#include "utils/fifo.h"
#include "consumer/common/cache_pool.h"
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void construct_nrb_image(struct cache_pool* cpool, struct net_image_desc* pimage, 
	void (*submit)(fifo_t* fifo, struct nrb_image_desc* pdata), fifo_t* fifo)
{
	nrb_image_encoder(cpool, pimage, submit, fifo);

	return;
}

inline void submit_nrb_image(fifo_t* fifo, struct nrb_image_desc* pdata)
{
	fifo_push(fifo, pdata);
}

void destruct_nrb_image(struct nrb_image_desc** iblock)
{
	if (*iblock) {
		free((*iblock)->data);
		free(*iblock);
	}
	*iblock = NULL;
}





