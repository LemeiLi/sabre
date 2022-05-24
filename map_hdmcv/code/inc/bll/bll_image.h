/*************************************************************************
	> File Name: bll_image.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 25 Oct 2019 03:55:26 PM CST
 ************************************************************************/

#ifndef _BLL_IMAGE_H
#define _BLL_IMAGE_H

#include <stdint.h>
#include "bll.h"
#include "dal/dal_image.h"

#ifdef __cplusplus
extern "C" {
#endif

#define bll_image_properties \
	BaseImage* cam; \
	void* priv;

#define bll_image_methods_alone \
	
#define bll_image_methods  \
	bll_methods            \
	bll_image_methods_alone

typedef struct bll_image_vmt {
	bll_image_methods	
}bll_image_vmt;

typedef struct bll_image {
	bll super;		
	bll_image_methods
	bll_image_properties
	bll_image_vmt* vptr;
}bll_image;

extern int bll_image_init(void* pthis, const char* cfg_file);
extern int bll_image_deinit(void* pthis);

#ifdef __cplusplus
}
#endif

#endif // _BLL_IMAGE_H
