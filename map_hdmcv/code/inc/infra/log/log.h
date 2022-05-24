/*************************************************************************
	> File Name: log.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 01 Nov 2019 10:28:02 AM CST
 ************************************************************************/

#ifndef _LOG_H
#define _LOG_H

#include "3dparty/zlog/zlog.h"

#ifdef __cplusplus
extern "C" {
#endif

#define log_info(...)    zlog_info(g_zc,__VA_ARGS__) 
#define log_error(...)   zlog_error(g_zc,__VA_ARGS__) 
#define log_warn(...)    zlog_warn(g_zc,__VA_ARGS__) 
#define log_fatal(...)   zlog_fatal(g_zc,__VA_ARGS__) 
#define log_notice(...)  zlog_notice(g_zc,__VA_ARGS__) 
#define log_debug(...)   zlog_debug(g_zc,__VA_ARGS__) 

extern zlog_category_t *g_zc;
extern int log_init(const char* conf_path);
extern void log_fini(void);

#ifdef __cplusplus
}
#endif

#endif // _LOG_H
