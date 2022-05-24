/*************************************************************************
	> File Name: tcp_service.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 16 Oct 2019 04:41:34 PM CST
 ************************************************************************/

#ifndef _TCP_SERVICE_H
#define _TCP_SERVICE_H

#include <stdint.h>
#include <stdbool.h>
#include "net.h"

#ifdef __cplusplus
extern "C" {
#endif

#define tcp_client_property \

#define tcp_client_methods_alone \
	
#define tcp_client_methods \
	net_methods \
	tcp_client_methods_alone

struct tcp_client_vmt {
	tcp_client_methods 
}tcp_cient_vmt;
	
typedef struct tcp_client {
	struct net super;
	tcp_client_methods 	
	tcp_client_property
	struct tcp_client_vmt* vptr;
}tcp_client;

extern struct tcp_client* construct_tcp_client(const uint8_t *ip, 
										       uint16_t port, bool io_mtplx);
extern void destruct_tcp_client(struct tcp_client* pthis);

#ifdef __cplusplus
}
#endif

#endif // _TCP_SERVICE_H
