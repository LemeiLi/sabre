/*************************************************************************
	> File Name: parse_lidar_config.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 22 Nov 2019 02:23:04 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "3dparty/cjson/cJSON.h"
#include "infra/log/log.h"
#include "parse_lidar_config.h"

char* get_lidar_conf_host_if(cJSON* root)
{
	cJSON* ldc = cJSON_GetObjectItem(root, "lidar_data_channel");
	if (!ldc) {
		log_error("get lidar.json lidar_data_channel failed!");
		return NULL;
	}

	cJSON* host = cJSON_GetObjectItem(ldc, "host");
	if (!host) {
		log_error("get lidar.json lidar_data_channel.host failed!");
		return NULL;
	}

	cJSON* itf = cJSON_GetObjectItem(host, "if");
	if (!itf) {
		log_error("get lidar_data_channel.host.if failed!");
		return NULL;
	}

	return itf->valuestring;
}

char* get_lidar_conf_host_ip(cJSON* root)
{
	cJSON* ldc = cJSON_GetObjectItem(root, "lidar_data_channel");
	if (!ldc) {
		log_error("get lidar.json lidar_data_channel failed!");
		return NULL;
	}

	cJSON* host = cJSON_GetObjectItem(ldc, "host");
	if (!host) {
		log_error("get lidar.json lidar_data_channel.host failed!");
		return NULL;
	}

	cJSON* ip = cJSON_GetObjectItem(host, "ip");
	if (!ip) {
		log_error("get lidar_data_channel.host.ip failed!");
		return NULL;
	}

	return ip->valuestring;
}

uint16_t get_lidar_conf_host_data_port(cJSON* root)
{
	cJSON* ldc = cJSON_GetObjectItem(root, "lidar_data_channel");
	if (!ldc) {
		log_error("get lidar.json lidar_data_channel failed!");
		return 0;
	}

	cJSON* host = cJSON_GetObjectItem(ldc, "host");
	if (!host) {
		log_error("get lidar.json lidar_data_channel.host failed!");
		return 0;
	}

	cJSON* port = cJSON_GetObjectItem(host, "data_port");
	if (!port) {
		log_error("get image_data_channel.host.data_port failed!");
		return 0;
	}

	return (uint16_t)port->valueint;
}

char *get_lidar_conf_lidar_mac(cJSON* root)
{
	cJSON* ldc = cJSON_GetObjectItem(root, "lidar_data_channel");
	if (!ldc) {
		log_error("get lidar.json lidar_data_channel failed!");
		return NULL;
	}

	cJSON* lidar = cJSON_GetObjectItem(ldc, "lidar");
	if (!lidar) {
		log_error("get lidar.json lidar_data_channel.lidar failed!");
		return NULL;
	}

	cJSON* mac = cJSON_GetObjectItem(lidar, "mac");
	if (!mac) {
		log_error("get image_data_channel.lidar.mac failed!");
		return NULL;
	}

	return mac->valuestring;
}




