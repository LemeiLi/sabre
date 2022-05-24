/*************************************************************************
	> File Name: cv22_4cam.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 10 Oct 2019 12:17:09 PM CST
 ************************************************************************/

#ifndef _CV224CAM_H
#define _CV224CAM_H

#include <pthread.h>
#include "dal/dal_image.h"
#include "infra/net/net.h"
#include "infra/link/datalink.h"

#ifdef __cplusplus
extern "C" {
#endif

///< 实际由josn承载
typedef struct {
	//I2CDriver* i2cp;
	//const I2CConfig *i2cfg;
	float *compsensitivity;
	float *compbias;
	int   compmode;
	struct net* net;
}CV224CamConfig;

typedef struct CV224Cam CV224Cam;

#define _cv224cam_methods_alone \
	int (*cv224cam_set_full_scale)(CV224Cam* pdev, int fs);

#define _cv224cam_methods \
	_base_image_methods \
	_cv224cam_methods_alone

struct CV224CamVMT {
	_cv224cam_methods 
};

#define _cv224cam_data \
	_base_sensor_data \
	int state; \
	const CV224CamConfig* config; \
	float compsensitivity[3]; \
	float compbias[3]; \
	pthread_t rx_tid; \
	datalink* link; \

typedef struct CV224Cam {
	BaseImage super;
	struct CV224CamVMT* vmt;
	_cv224cam_methods
	_cv224cam_data 
}CV224Cam;

extern int CV224CamInit(CV224Cam* pthis, CV224CamConfig* pconf);
extern int CV224CamDeinit(void* pthis);
extern struct CV224Cam* construct_cv224cam(CV224CamConfig* conf);
extern int destruct_cv224cam(struct CV224Cam* pthis);

#ifdef __cplusplus
}
#endif

#endif // _CV224CAM_H


