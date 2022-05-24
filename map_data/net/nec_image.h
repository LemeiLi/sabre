/*************************************************************************
	> Created Time: Wed 24 Jul 2019 05:33:52 PM CST
 ************************************************************************/

#ifndef _NET_IMAGE_ENCODER_H
#define _NET_IMAGE_ENCODER_H

#include "utils/fifo.h"
#include "net/nrb_image.h"
#include "consumer/common/cache_pool.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IMAGE_SIZE_PER_SLICE (200*1024)

/**< protocal format:
 * 3 + 1 + 4 + 8 + 1 + 2 + data + 2 = 21 + data
 * 3 head + 1 slice index + 4 data length + 8 timestamp + 1 id + 2 slice flags + data + 2 check sum
 */	
#define NET_IMAGE_META_LEN 21

extern void nrb_image_encoder(struct cache_pool* cpool, struct net_image_desc* pimage,
	void (*fifo_push)(fifo_t* fifo, struct nrb_image_desc* pdata), fifo_t* fifo);

#ifdef __cplusplus
}
#endif

#endif // _NET_IMAGE_ENCODER_H
