/*************************************************************************
	> Created Time: Tue 14 May 2019 07:53:42 PM CST
 ************************************************************************/

#include "net/nrb_image.h"
#include "consumer/common/cache_pool.h"
#include "nec_image.h"
#include "utils/fifo.h"
#include "utils/utils.h"
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static inline void set_protocol_head(struct nrb_image_desc* iblock)
{
	/**< head 3 byte */
	iblock->data[0] = 0xFF;
	iblock->data[1] = 0xD8;
	iblock->data[2] = 0xD9;

	return;
}

static inline void set_protocol_timestamp(struct nrb_image_desc* iblock, uint64_t ts64)
{
	iblock->data_times = (uint32_t)(ts64 / 1000000);
	iblock->data_timeu = (uint32_t)(ts64 % 1000000);

	/**< timestamp 8byte */
	iblock->data[8] = iblock->data_times & 0xFF;
	iblock->data[9] = (iblock->data_times >> 8) & 0xFF;
	iblock->data[10] = (iblock->data_times >> 16) & 0xFF;
	iblock->data[11] = (iblock->data_times >> 24) & 0xFF;

	iblock->data[12] = iblock->data_timeu & 0xFF;
	iblock->data[13] = (iblock->data_timeu >> 8) & 0xFF;
	iblock->data[14] = (iblock->data_timeu >> 16) & 0xFF;
	iblock->data[15] = (iblock->data_timeu >> 24) & 0xFF;

	return;
}

static inline void set_protocol_id(struct nrb_image_desc* iblock, int id)
{
	#define CV22_4CAM_ID 1 ///< TODO
	/**< id */ 
	iblock->data[16] = CV22_4CAM_ID;

	return;
}

static inline void set_protocol_slice_index(struct nrb_image_desc* iblock, uint8_t index)
{
	/**< index*/ 
	iblock->data[3] = index;
	//fprintf(stderr, "%d ", index);
	
	return;
}

/**< data length: slice_size + 1 id + 2 slice flags */
static inline void set_protocol_slice_size(struct nrb_image_desc* iblock, size_t size)
{
	iblock->data[4] = (size + 1 + 2) & 0xFF;
	iblock->data[5] = ((size + 1 + 2) >> 8) & 0xFF;
	iblock->data[6] = ((size + 1 + 2) >> 16) & 0xFF;
	iblock->data[7] = ((size + 1 + 2) >> 24) & 0xFF;

	return;
}

static inline void padding_protocol_main_body(struct nrb_image_desc* iblock, const void* source, size_t size)
{
	memcpy(&iblock->data[19], source, size);

	return;
}

static inline void set_protocol_to_send(struct nrb_image_desc* iblock, size_t length)
{
	iblock->size = length;

	return;
}

static inline void set_protocol_flags(struct nrb_image_desc* iblock, uint8_t end)
{
	/**< frame flags */	
	if (!end) {
		iblock->data[17] = 0xFF;
		iblock->data[18] = 0xD8;
	} else { ///< the last frame 
		iblock->data[17] = 0xFF;
		iblock->data[18] = 0xD9;
	}

	return;
}

static inline void set_protocol_checksum(struct nrb_image_desc* iblock, size_t size)
{
	/**< check sum */	
	//uint8_t check_sum = 0;
	//check_sum = xor8(iblock->data + 19, size);
	iblock->data[size + 19] = 0x66;
	//log("check sum is: 0x%x\n", check_sum);
	//fprintf(stderr, "0x%x ", check_sum);
	iblock->data[size + 20] = 0x99;
	
	return;
}

void feedback_nrb_image(struct cache_pool* pthis, struct nrb_image_desc* pdata)
{
	pthread_mutex_lock(&pthis->mlock);
	//log("feedback nrb image: nr = %d, cap = %ld, rfmap = %p, data = %p\n", pthis->nr, pdata->cap, pdata->base.rfmap, pdata->data);

	if (pthis->nr < NRB_IMG_POOL_CAPABILITY) {
		//log("feedback nrb image success A!\n");
		list_add(&pdata->node, &pthis->list);
		pthis->nr++;
	} else {
		//log("feedback nrb image success B!\n");
		struct nrb_image_desc* iblock = NULL;
		iblock = list_entry(pthis->list.prev, struct nrb_image_desc, node);
		list_del(pthis->list.prev);            ///< remove tail entry
		list_add(&pdata->node, &pthis->list);  ///< add new cache data to list head
		if (iblock) {
			destruct_nrb_image(&iblock);
		}
	}
	pthread_mutex_unlock(&pthis->mlock);

	return;
}

void recycle_nrb_image(struct cache_pool* cpool, struct nrb_image_desc* pdata)
{
	feedback_nrb_image(cpool, pdata);

	return;
}


struct nrb_image_desc* pickup_nrb_image(struct cache_pool* cpool)
{
	struct nrb_image_desc* iblock = NULL;

	pthread_mutex_lock(&cpool->mlock);
	if (!list_empty(&cpool->list)) {
		iblock = list_entry(cpool->list.next, struct nrb_image_desc, node); ///< get first entry
		list_del(cpool->list.next); ///< remove first entry from list
		cpool->nr--;
		//log("pickup nrb image success: nr = %d, cap = %ld, rfmap = %p, data = %p\n", cpool->nr, iblock->cap, iblock->base.rfmap, iblock->data);
	}
	pthread_mutex_unlock(&cpool->mlock);

	return iblock;
}

static struct nrb_image_desc* alloc_net_image_block(struct cache_pool* cpool)
{
	struct nrb_image_desc *iblock = NULL;
	if ((iblock = pickup_nrb_image(cpool)) == NULL) {
		iblock = malloc(sizeof(struct nrb_image_desc));
		if(iblock == NULL) {
			error("malloc struct nrb_image_desc object error");
			return NULL;
		}
		memset(iblock, 0, sizeof(struct nrb_image_desc));
		iblock->base.id = NET_IMAGE_ID;
		iblock->base.fragement = true;
		iblock->data = malloc(IMAGE_SIZE_PER_SLICE + 100);
		iblock->cap = IMAGE_SIZE_PER_SLICE + 100; ///< constant capacity: IMAGE_SIZE_PER_SLICE + 100
		if(iblock->data == NULL) {
			error("malloc iblock->data error");
			free(iblock);
			return NULL;
		}
		iblock->base.rfmap = iblock->data;
	}
	//log("alloc nrb image success: cap = %ld, rfmap = %p, data = %p\n", iblock->cap, iblock->base.rfmap, iblock->data);
	//memset(iblock->data, 0, IMAGE_SIZE_PER_SLICE + 100);

	return iblock;
}

void nrb_image_encoder(struct cache_pool* cpool, struct net_image_desc* pimage,
	void (*submit)(fifo_t* fifo, struct nrb_image_desc* pdata), fifo_t* fifo)
{
	//static uint32_t total = 0;

	uint8_t image_slice_index = 1;
	uint8_t *image_start = pimage->image;
	uint8_t *image_pos = image_start;
	uint8_t image_slice_count = pimage->size / IMAGE_SIZE_PER_SLICE;
	uint32_t image_slice_remainder = pimage->size % IMAGE_SIZE_PER_SLICE;
	if (image_slice_remainder > 0) 
		image_slice_count++;

	while (image_slice_count) {
		struct nrb_image_desc* iblock = alloc_net_image_block(cpool);
		if (iblock == NULL) {
			error("alloc_net_image_block error\n");
			continue;
		}
		set_protocol_head(iblock);
		set_protocol_timestamp(iblock, pimage->stamp * 1000000); ///< to us
		set_protocol_id(iblock, 1);
		set_protocol_flags(iblock, 0);
		set_protocol_slice_index(iblock, image_slice_index++);

		if (image_slice_count > 1) {
			set_protocol_slice_size(iblock, IMAGE_SIZE_PER_SLICE);
			padding_protocol_main_body(iblock, image_pos, IMAGE_SIZE_PER_SLICE);
			set_protocol_flags(iblock, 0);
			set_protocol_checksum(iblock, IMAGE_SIZE_PER_SLICE);

			image_pos += IMAGE_SIZE_PER_SLICE;
			set_protocol_to_send(iblock, IMAGE_SIZE_PER_SLICE + NET_IMAGE_META_LEN);
			iblock->base.to_send = IMAGE_SIZE_PER_SLICE + NET_IMAGE_META_LEN;
		} else if (image_slice_count == 1) {
			set_protocol_slice_size(iblock, image_slice_remainder);
			padding_protocol_main_body(iblock, image_pos, image_slice_remainder);
			set_protocol_flags(iblock, 1);
			set_protocol_checksum(iblock, image_slice_remainder);

			image_pos += image_slice_remainder;
			set_protocol_to_send(iblock, image_slice_remainder + NET_IMAGE_META_LEN);
			iblock->base.to_send = image_slice_remainder + NET_IMAGE_META_LEN;
		}
		(*submit)(fifo, iblock);

		image_slice_count--;
	}
	//log("push total images = %d\n", ++total);

	return;
}




