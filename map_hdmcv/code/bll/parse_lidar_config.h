/*************************************************************************
	> File Name: parse_lidar_config.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 22 Nov 2019 02:23:12 PM CST
 ************************************************************************/

#ifndef _PARSE_LIDAR_CONFIG_H
#define _PARSE_LIDAR_CONFIG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char* get_lidar_conf_host_if(cJSON* root);
extern char* get_lidar_conf_host_ip(cJSON* root);
extern uint16_t get_lidar_conf_host_data_port(cJSON* root);
extern char *get_lidar_conf_lidar_mac(cJSON* root);

#ifdef __cplusplus
}
#endif

#endif // _PARSE_LIDAR_CONFIG_H
