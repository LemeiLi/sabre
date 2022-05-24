/*************************************************************************
	> File Name: bll_insgps.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 06 Nov 2019 03:58:55 PM CST
 ************************************************************************/

#ifndef _BLL_INSGPS_H
#define _BLL_INSGPS_H

#include <stdint.h>
#include "bll.h"
#include "dal/dal_insgps.h"

#ifdef __cplusplus
extern "C" {
#endif

#define bll_insgps_properties \
	BaseINSGPS* insgps; \
	void* priv;

#define bll_insgps_methods_alone \
	int (*write_rtk)(void* pthis, char* data, size_t size);
	
#define bll_insgps_methods  \
	bll_methods            \
	bll_insgps_methods_alone

typedef struct bll_inspgs_vmt {
	bll_insgps_methods	
}bll_insgps_vmt;

typedef struct bll_insgps {
	bll super;		
	bll_insgps_methods
	bll_insgps_properties
}bll_insgps;

extern int bll_insgps_init(void* pthis, const char* cfg_file);
extern int bll_insgps_deinit(void* pthis);

#ifdef __cplusplus
}
#endif

#endif // _BLL_INSGPS_H
