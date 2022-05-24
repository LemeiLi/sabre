/*************************************************************************
	> File Name: udp_service.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 16 Oct 2019 04:41:34 PM CST
 ************************************************************************/

#ifndef _UDP_SERVICE_H
#define _UDP_SERVICE_H

#include <stdint.h>
#include <netinet/in.h>
#include "net.h"

#ifdef __cplusplus
extern "C" {
#endif

#define udp_server_property \
	struct sockaddr_in clt_addr; \
	struct sockaddr_in srv_addr; \

#define udp_server_methods_alone \
	
#define udp_server_methods \
	net_methods \
	udp_server_methods_alone

struct udp_server_vmt {
	udp_server_methods 
}udp_cient_vmt;
	
typedef struct udp_server {
	struct net super;
	udp_server_methods 	
	udp_server_property
	struct udp_server_vmt* vptr;
}udp_server;

extern struct udp_server* construct_udp_server(const uint8_t *ip, 
		                                       uint16_t port, bool io_mtplx);
extern void destruct_udp_server(struct udp_server* pthis);

#ifdef __cplusplus
}
#endif

#endif // _UDP_SERVICE_H
