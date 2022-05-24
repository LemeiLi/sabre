/*************************************************************************
	> File Name: net.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 17 Oct 2019 02:47:13 PM CST
 ************************************************************************/

#ifndef _NET_H
#define _NET_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum net_state {
	NETS_DISCONNECTED,
	NETS_CONNECTED,
	NETS_SERVER_CLOSED,
	NETS_ERROR,
}net_state;

#define net_methods \
	int32_t (*connect_server)(void* pthis); \
	ssize_t (*send_data)(void* pthis, void* data, size_t size); \
	ssize_t (*recv_data)(void* pthis, void* buf, size_t size);  \
	net_state (*get_state)(void* pthis); \
	int32_t (*get_fd)(void* pthis);      \
	uint16_t (*get_port)(void* pthis);   \
	uint8_t* (*get_ip)(void* pthis);     \
	uint8_t* (*get_mac)(void* pthis);    \
	uint8_t* (*get_itf)(void* pthis);    \
	int32_t (*close)(void* pthis);

/* attributes */
/* operations */
#define net_property  \
	uint8_t ip[18];   \
	uint16_t port;    \
	uint8_t mac[20];  \
	uint8_t itf[18];  \
	int32_t fd;       \
	int32_t wefd;     \
	int32_t refd;     \
	int32_t timeout;  \
	volatile bool io_mtplx; \
	volatile net_state state; 

typedef struct net_vmt {
	net_methods
}net_vmt;

typedef struct net {
	net_property
	net_methods
	struct net_vmt* vptr;
}net;

extern int net_init(void* pthis, void* vptr, const uint8_t *itf, 
		            const uint8_t *mac, const uint8_t* ip, 
					uint16_t port, bool io_mtplx);

#ifdef __cplusplus
}
#endif

#endif // _NET_H
