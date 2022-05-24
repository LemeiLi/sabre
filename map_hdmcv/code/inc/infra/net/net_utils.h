/*************************************************************************
	> File Name: net_utils.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 27 Nov 2019 03:30:32 PM CST
 ************************************************************************/

#ifndef _NET_UTILS_H
#define _NET_UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int udp_raw_filter(uint8_t *src_mac, void* raw);
extern int get_local_ip(const char *eth_inf, char *ip);
extern int get_local_mac(const char *eth_inf, char *mac);
extern int get_ip_by_domain(const char *domain, char *ip);

#ifdef __cplusplus
}
#endif

#endif // _NET_UTILS_H
