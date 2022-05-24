/*************************************************************************
	> Created Time: Wed 24 Jul 2019 05:05:53 PM CST
 ************************************************************************/

#ifndef _NET_GPS_ENCODER_H
#define _NET_GPS_ENCODER_H

#include "net/nrb_gps.h"
#include <stdlib.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * 4 head(0xff,0x01,0xaa,0x88) + 4 length(context + id) + 8 timestamp + 1 id + [gps data] + 2 checksum
 */
#define NET_GPS_META_LEN 19 

extern void nrb_gps_encoder(struct nrb_gps_desc* pthis, void* pdata, size_t data_size);

#ifdef __cplusplus
}
#endif

#endif // _NET_GPS_ENCODER_H
