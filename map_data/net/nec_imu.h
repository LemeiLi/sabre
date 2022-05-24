/*************************************************************************
	> Created Time: Wed 24 Jul 2019 05:42:11 PM CST
 ************************************************************************/

#ifndef _NET_IMU_ENCODER_H
#define _NET_IMU_ENCODER_H

#include "net/nrb_imu.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 4 head(0xff,0x01,0xaa,0x66) + 4 length(context + id) + 8 timestamp + 1 id + [imu data] + 2 checksun
 */
#define NET_IMU_META_LEN 19

extern void nrb_imu_encoder(struct nrb_imu_desc* pthis, void* pdata, size_t data_size);

#ifdef __cplusplus
}
#endif

#endif // _NET_IMU_ENCODER_H
