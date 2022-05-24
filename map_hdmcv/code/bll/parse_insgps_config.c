/*************************************************************************
	> File Name: parse_insgps_config.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 06 Nov 2019 05:30:12 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "3dparty/cjson/cJSON.h"
#include "infra/stream/stream.h"
#include "infra/log/log.h"
#include "parse_insgps_config.h"

uint8_t get_insgps_conf_com_num(cJSON* root)
{
	cJSON* param = cJSON_GetObjectItem(root, "insgps_param");
	if (!param) {
		log_error("get insgps.json insgps_param failed!");
		return 0;
	}

	cJSON* list_com = cJSON_GetObjectItem(param, "list_com");
	if (!list_com) {
		log_error("get insgps.json list_com failed!");
		return 0;
	}

	return (uint8_t)cJSON_GetArraySize(list_com);
}

uint8_t get_insgps_conf_type_max(cJSON* root, size_t size)
{
	uint8_t type_max = 0;
	cJSON* param = cJSON_GetObjectItem(root, "insgps_param");
	if (!param) {
		log_error("get insgps.json insgps_param failed!");
		return 0;
	}

	cJSON* list_com = cJSON_GetObjectItem(param, "list_com");
	if (!list_com) {
		log_error("get insgps.json list_com failed!");
		return 0;
	}

	for (size_t i = 0; i < size; i++) {
		cJSON* comx = cJSON_GetArrayItem(list_com, (int)i);
		if (!comx) {
			log_error("get insgps.json list_com item failed!");
			return 0;
		}

		cJSON* data = cJSON_GetObjectItem(comx, "data");
		if (!data) {
			log_error("get insgps.json list_com data failed!");
			return 0;
		}

		uint8_t type_num = (uint8_t)cJSON_GetArraySize(data);
		type_max = (type_max > type_num) ? type_max : type_num;
	}

	return type_max;
}

int get_insgps_conf_com(cJSON* root, struct com_config *com_configs, size_t size)
{
	cJSON* param = cJSON_GetObjectItem(root, "insgps_param");
	if (!param) {
		zlog_error(g_zc, "get insgps.json insgps_param failed!");
		return -1;
	}

	cJSON* list_com = cJSON_GetObjectItem(param, "list_com");
	if (!list_com) {
		zlog_error(g_zc, "get insgps.json list_com failed!");
		return -1;
	}

	for (size_t i = 0; i < size; i++) {
		cJSON* comx = cJSON_GetArrayItem(list_com, (int)i);
		if (!comx) {
			zlog_error(g_zc, "get insgps.json list_com item failed!");
			return -1;
		}
		cJSON* devx = cJSON_GetObjectItem(comx, "dev");
		if (!devx) {
			zlog_error(g_zc, "get insgps.json list_com dev failed!");
			return -1;
		}
		com_configs[i].dev = devx->valuestring;
		//zlog_info(g_zc, "get insgps.json list_com dev %s!", com_configs[i].dev);

		cJSON* data = cJSON_GetObjectItem(comx, "data");
		if (!data) {
			zlog_error(g_zc, "get insgps.json list_com data failed!");
			return -1;
		}
		int data_num = cJSON_GetArraySize(data);
		com_configs[i].type_num = (uint8_t)data_num;
		for (int k = 0; k < data_num; k++) {
			cJSON* datax = cJSON_GetArrayItem(data, k);	
			if (!data) {
				zlog_error(g_zc, "get insgps.json list_com data item failed!");
				return -1;
			}
			com_configs[i].type[k] = datax->valuestring;
			zlog_info(g_zc, "get insgps.json data type %s!", com_configs[i].type[k]);
		}

		cJSON* baudrate = cJSON_GetObjectItem(comx, "baudrate");
		if (!baudrate) {
			zlog_error(g_zc, "get insgps.json list_com baudrate failed!");
			return -1;
		}
		com_configs[i].baudrate = (uint32_t)(baudrate->valueint);
		//zlog_info(g_zc, "get insgps.json list_com baudrate %d!", com_configs[i].baudrate);
	}

	return 0;
}




