/*************************************************************************
	> Created Time: Thu 25 Jul 2019 05:14:54 PM CST
 ************************************************************************/

#ifndef _NATIVE_DESCRIPTOR_H
#define _NATIVE_DESCRIPTOR_H

#include <stdint.h>
#include <stdlib.h>
#include "consumer/common/base_object.h"
#include "consumer/common/cache_pool.h"
#include "utils/ulist.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NTV_POOL_CAPABILITY (100)

typedef enum ntv_id {
	NTV_GPS_ID = 1,
	NTV_IMU_ID = 2,
	NTV_ODO_ID = 3,
	NTV_IMAGE_ID = 4,
}ntv_id_t;

typedef struct ntv_desc { ///< native description
	struct bobject bobj;
	struct list_head node;
	enum ntv_id id;
	double timestamp;
	size_t size;  ///< real data size, less than or equal to capacity	
	size_t cap;   ///< capacity of this ntv_desc
	void* private;
	uint8_t data[0];
}ntv_desc_t;

extern struct ntv_desc* construct_ntv_desc(struct cache_pool* cpool, void* data, size_t size, enum ntv_id id);
extern void recycle_ntv_desc(struct cache_pool* cpool, struct ntv_desc* pdata);
extern void destruct_ntv_desc(struct ntv_desc** ppthis);

#ifdef __cplusplus
}
#endif

#endif // _NATIVE_DESCRIPTOR_H
