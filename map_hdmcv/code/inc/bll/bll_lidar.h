/*************************************************************************
	> File Name: bll_lidar.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 22 Nov 2019 02:31:19 PM CST
 ************************************************************************/

#ifndef _BLL_LIDAR_H
#define _BLL_LIDAR_H

#include <stdint.h>
#include "bll.h"
#include "dal/dal_lidar.h"

#ifdef __cplusplus
extern "C" {
#endif

#define bll_lidar_properties \
	BaseLidar *lidar; \
	void *priv;

#define bll_lidar_methods_alone \

#define bll_lidar_methods \
	bll_methods           \
	bll_lidar_methods_alone 

typedef struct bll_lidar_vmt {
	bll_lidar_methods 
}bll_lidar_vmt;

typedef struct bll_lidar {
	bll super;
	bll_lidar_methods 
	bll_lidar_properties
	bll_lidar_vmt* vptr;
}bll_lidar;

extern int bll_lidar_init(void* pthis, const char* cfg_file);
extern int bll_lidar_deinit(void* pthis);

#ifdef __cplusplus
}
#endif

#endif // _BLL_LIDAR_H
