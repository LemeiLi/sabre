/*************************************************************************
	> File Name: serial.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 06 Nov 2019 11:00:16 AM CST
 ************************************************************************/

#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TYPE_MAX 4

typedef struct com_config {
	char* dev;
	uint32_t baudrate;
	uint8_t type_num;
	char* type[TYPE_MAX]; ///< data type 
}com_config;

#define serial_methods \
	int (*set_baud)(void* pthis, uint32_t baud); \
	int (*sflush)(void* pthis); \
	ssize_t (*tx)(void* pthis, const char *wbuf, size_t size); \
	ssize_t (*rx)(void* pthis, char* rbuf, size_t size); \
	ssize_t (*rl)(void* pthis, char* rbuf, size_t max_len); ///< read line

#define serial_properties \
	int fd; \
	char dev_path[32]; \
	uint32_t baudrate; \

typedef struct serial_vmt {
	serial_methods
}serial_vmt;

typedef struct serial {
	serial_vmt *vptr;
	serial_properties
	serial_methods
}serial;

extern int serial_init(void* pthis, const char *dev_path, uint32_t baudrate);
extern int serial_deinit(void* pthis);
extern serial* construct_serial(const char *dev_path, uint32_t baudrate);
extern int destruct_serial(void* pthis);

#ifdef __cplusplus
}
#endif

#endif // _SERIAL_H
