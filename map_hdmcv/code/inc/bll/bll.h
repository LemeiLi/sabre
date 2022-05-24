/*************************************************************************
	> File Name: bll.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 25 Oct 2019 04:02:51 PM CST
 ************************************************************************/

#ifndef _BLL_H
#define _BLL_H

#include <stdint.h>
#include "3dparty/cjson/cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

#define bll_methods                                                          \
	char* (*version)(void* pthis);                                           \
	int (*register_data_callback)(void *pthis, void* dcb, void *user);       \
	int (*register_status_callback)(void* pthis, void* scb, void *user);     \
	int (*start)(void* pthis, uint8_t ucStartMode);                          \
	int (*stop)(void* pthis, uint8_t ucStopMode);                            \
                                                                             \
	int (*pop_data)(void* pthis, void** data, int iWaitTimeMS, void *user);  \
	int (*push_data)(void* pthis, void** data, void *user);                  \
	int (*get_cfg)(void* pthis, char *pcGetTagJson, char *pcValueJson);      \
	int (*set_cfg)(void* pthis, char *pcSetTagJson);                         \

#define bll_properties \
	void** handle;     \
	int8_t* config;    \
	cJSON* root;       \

typedef struct bll_vmt {
	bll_methods
}bll_vmt;

typedef struct bll {
	bll_vmt *vptr;	
	bll_methods
	bll_properties
}bll;

extern int bll_init(void* pthis, void* vmt, const char* cfg_file);
extern void bll_deinit(void* pthis);

#ifdef __cplusplus
}
#endif

#endif // _BLL_H
