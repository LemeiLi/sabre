/*************************************************************************
	> File Name: inc/infra/net/ethnet.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Mon 25 Nov 2019 11:16:07 AM CST
 ************************************************************************/

#ifndef _ETHNET_H
#define _ETHNET_H

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <net/if.h>
#include <netinet/ether.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ETH_IP_UDP_HEAD_SIZE (42)
#define ETH_IP_TCP_HEAD_SIZE (60)

typedef struct eth_ip_udp {
	struct ether_header eh; ///< 14 
	struct iphdr iph;       ///< 22
	struct udphdr udph;     ///< 8
}eth_ip_udp __attribute__((aligned(1)));

typedef struct eth_ip_tcp {
	struct ether_header eh; ///< 14
	struct iphdr iph;       ///< 22
	struct tcphdr tcph;     ///< 24
}eth_ip_tcp __attribute__((aligned(1)));

#ifdef __cplusplus
}
#endif

#endif // _ETHNET_H
