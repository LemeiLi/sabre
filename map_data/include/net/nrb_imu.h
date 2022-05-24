/*************************************************************************
	> Created Time: Tue 16 Jul 2019 06:05:01 PM CST
 ************************************************************************/

#ifndef _NRB_IMU_H
#define _NRB_IMU_H

#include "net/nrb_desc.h"
#include "utils/ulist.h"
#include "utils/fifo.h"
#include "consumer/common/cache_pool.h"
#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NRB_IMU_POOL_CAPABILITY (500)

typedef struct nrb_imu_desc {
	struct nrb_desc base;
	struct list_head node;
	size_t size;	
	size_t cap;
	uint8_t data[0];
}nrb_imu_desc_t;

extern struct nrb_imu_desc* construct_nrb_imu(struct cache_pool* cpool, void* pdata, size_t data_size);
extern void submit_nrb_imu(fifo_t* fifo, struct nrb_imu_desc* pdata);
extern void recycle_nrb_imu(struct cache_pool* cpool, struct nrb_imu_desc* pdata);
extern void destruct_nrb_imu(struct nrb_imu_desc** pthis);

#ifdef __cplusplus
}
#endif

#endif //  _NRB_IMU_H
