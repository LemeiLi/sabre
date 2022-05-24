/*************************************************************************
	> File Name: xwgi7660.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 10 Oct 2019 12:17:09 PM CST
 ************************************************************************/

#ifndef _XWGI7660_H
#define _XWGI7660_H

#include <stdint.h>
#include <pthread.h>
#include "dal_insgps.h"
#include "infra/serial/serial.h"
#include "infra/link/datalink.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SERIAL_NUM (4)

typedef struct {
	float *accsensitivity;
	float *accbias;
	int   accmode;
	struct serial* coms[SERIAL_NUM]; ///< com0, com1, com2, com3
	struct com_config* com_conf[SERIAL_NUM];
	uint8_t com_num;
}XWGI7660Config;

typedef struct  XWGI7660 XWGI7660;

#define _xwgi7660_methods_alone \
	int (*xwgi7660_set_full_scale)(XWGI7660* pdev, int fs); \

#define _xwgi7660_methods \
	_base_insgps_methods \
	_xwgi7660_methods_alone

struct XWGI7660VMT{
	_xwgi7660_methods
};

#define _xwgi7660_data \
	_base_sensor_data \
	int state; \
	const XWGI7660Config *config; \
	size_t accaxes; \
	float accsensitivity[3]; \
	float accbias[3]; \
	pthread_t rx_tid[SERIAL_NUM]; \
	datalink *link[SERIAL_NUM]; \
	//uint8_t sindex; ///< serial index form 0 to SERIAL_NUM - 1

struct XWGI7660 {
	BaseINSGPS super;
	struct XWGI7660VMT* vmt;
	_xwgi7660_methods
	_xwgi7660_data 
};

extern int XWGI7660Init(XWGI7660* devp, XWGI7660Config* pconf);
extern int XWGI7660DeInit(void* pthis);
extern XWGI7660* construct_xwgi7660(void* conf);
extern int destruct_xwgi7660(void* pthis);

#ifdef __cplusplus
}
#endif

#endif // _XWGI7660_H


