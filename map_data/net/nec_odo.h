/*************************************************************************
	> Created Time: Wed 24 Jul 2019 07:02:07 PM CST
 ************************************************************************/

#ifndef _NET_ODO_ENCODER_H
#define _NET_ODO_ENCODER_H

#include "net/nrb_odo.h" 
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 4 head(0xff,0x01,0xaa,0x77) + 8 timestamp + 4 length + 1 id + [odo data] + 2 checksum
 */
#define NET_ODO_META_LEN 19 ///< 16 + 1 + 2

extern void nrb_odo_encoder(struct nrb_odo_desc* pthis, void* pdata, size_t data_size);


#ifdef __cplusplus
}
#endif

#endif // _NET_ODO_ENCODER_H
