/*************************************************************************
	> File Name: parse_image_config.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Sat 26 Oct 2019 11:48:14 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "3dparty/cjson/cJSON.h"
#include "infra/stream/stream.h"
#include "infra/log/log.h"
#include "parse_image_config.h"

char* get_image_conf_ip(cJSON* root)
{
	cJSON* idc = cJSON_GetObjectItem(root, "image_data_channel");
	if (!idc) {
		zlog_error(g_zc, "get image.json image_data_channel failed!");
		return NULL;
	}

	cJSON* ip = cJSON_GetObjectItem(idc, "ip");
	if (!ip) {
		zlog_error(g_zc, "get image_data_channel.ip failed!");
		return NULL;
	}

	return ip->valuestring;
}

uint16_t get_image_conf_port(cJSON* root)
{
	cJSON* idc = cJSON_GetObjectItem(root, "image_data_channel");
	if (!idc) {
		zlog_error(g_zc, "get image.json image_data_channel failed!");
		return 0;
	}

	cJSON* port = cJSON_GetObjectItem(idc, "port");
	if (!port) {
		zlog_error(g_zc, "get image_data_channel.port failed!");
		return 0;
	}

	return (uint16_t)port->valueint;
}


