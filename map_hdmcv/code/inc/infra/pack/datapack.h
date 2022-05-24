/*************************************************************************
	> File Name: datapack.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 18 Oct 2019 08:17:17 PM CST
 ************************************************************************/

#ifndef _DATA_PACKAGE_H
#define _DATA_PACKAGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define datapack_property   \
	uint8_t type;           \
	uint8_t mtid;           \
	uint8_t mref;           \
	void* maddr;            \
	size_t msize;           \
	uint64_t dts;           \
	void* priv;             \
	uint8_t mdata[0];       \

#define datapack_methods                         \
	void* (*data)(void* pthis);                  \
	size_t (*size)(void* pthis);                 \
	void (*release)(void** pthis);               \
    void (*set_dts)(void* pthis, uint64_t dts);  \
	uint64_t (*get_dts)(void* pthis);            \
    void (*set_type)(void* pthis, uint8_t type); \
    uint8_t (*get_type)(void* pthis);            \

struct datapack_vmt {
	datapack_methods
}datapack_vmt;

typedef struct datapack {
	struct datapack_vmt *vptr;
	datapack_methods 
	datapack_property
}datapack;

extern void datapack_init(void* pthis, void* vmt, void* saddr, size_t size);
extern struct datapack* construct_datapack(void* vmt, void* saddr, size_t size);
extern void destruct_dapapack(struct datapack* pthis);

#ifdef __cplusplus
}
#endif

#endif // _DATA_PACKAGE_H
