/*************************************************************************
	> File Name: vlp32c.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 10 Oct 2019 12:17:09 PM CST
 ************************************************************************/

#ifndef _VLP32C_H
#define _VLP32C_H

#include <stdint.h>
#include "dal_lidar.h"
#include "infra/link/datalink.h"
#include "infra/net/net.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ETH_IP_UDP_HEAD_SIZE  (42)
#define LIDAR_DATA_PACK_SIZE  (1206)
#define LIDAR_POS_PACK_SIZE   (512)
#define LIDAR_DATA_TS_OFFSET  (1200)

typedef struct {
	uint8_t ip[16]; 
	uint8_t mac[18];
	uint8_t dhitf[16]; 
	struct net* dhnet;   ///< destination host net config: local net
}VLP32CConfig;

#define _vlp32c_methods_alone \
	int (*vlp32c_set_full_scale)(void* pthis, int fs); \
	int (*vlp32c_data_crash_probe)(void* pthis, uint32_t ts);

#define _vlp32c_methods \
	_base_lidar_methods \
	_vlp32c_methods_alone

struct VLP32CVMT {
	_vlp32c_methods
};

#define _vlp32c_data   \
	_base_sensor_data; \
	int state;                  \
	const VLP32CConfig* config; \
	size_t compaxes;            \
	float compsensitivity[3];   \
	float compbias[3]; \
	pthread_t rx_tid;  \
	datalink *link;    \
    void* priv;        \

typedef struct VLP32C {
	BaseLidar super;
	struct VLP32CVMT* vmt;
	_vlp32c_methods
	_vlp32c_data
}vlp32c;

extern int VLP32CInit(vlp32c* pthis, VLP32CConfig* pconf);
extern int VLP32CDeinit(void* pthis);
extern vlp32c* construct_vlp32c(VLP32CConfig *pconf);
extern int destruct_vlp32c(vlp32c *pthis);

#ifdef __cplusplus
}
#endif

#endif // _VLP32C_H


