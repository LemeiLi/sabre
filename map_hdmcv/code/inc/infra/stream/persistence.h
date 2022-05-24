/*************************************************************************
	> File Name: persistence.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 18 Oct 2019 07:54:00 PM CST
 ************************************************************************/

#ifndef _PERSISTENCE_H
#define _PERSISTENCE_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void persistence(const char* name, void* buf, size_t size, int flag);

#ifdef __cplusplus
}
#endif

#endif // _PERSISTENCE_H
