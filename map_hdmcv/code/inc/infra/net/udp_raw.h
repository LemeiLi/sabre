/*************************************************************************
	> File Name: udp_raw.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Tue 26 Nov 2019 08:14:28 PM CST
 ************************************************************************/

#ifndef _UDP_RAW_H
#define _UDP_RAW_H

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include "infra/net/net.h"

#ifdef __cplusplus
extern "C" {
#endif

//60:76:88:38:01:a1 lidar
//8c:ec:4b:c3:54:9b enp2s0

#define DEST_MAC0	0x8C
#define DEST_MAC1	0xEC
#define DEST_MAC2	0x4B
#define DEST_MAC3	0xC3
#define DEST_MAC4	0x54
#define DEST_MAC5	0x9B

#define ETHER_TYPE	0x0800 ///< IPV4

#define DEFAULT_IF	"enp2s0" ///< 从json 解析
#define BUF_SIZ		1248

#define udp_raw_property \
	struct sockaddr_in clt_addr;

#define udp_raw_methods_alone \
	int (*raw_filter)(const uint8_t *src_mac, void* raw);
	
#define udp_raw_methods \
	net_methods \
	udp_raw_methods_alone

struct udp_raw_vmt {
	udp_raw_methods 
}udp_raw_vmt;
	
typedef struct udp_raw {
	struct net super;
	udp_raw_methods 	
	udp_raw_property
	struct udp_raw_vmt* vptr;
}udp_raw;

extern struct udp_raw* construct_udp_raw(const uint8_t *itf, const uint8_t *ip, 
		                                 uint16_t port, bool io_mtplx);
extern void destruct_udp_raw(void* pthis);

#ifdef __cplusplus
}
#endif

#endif // _UDP_RAW_H
