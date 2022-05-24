/*************************************************************************
	> File Name: parse_image_config.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Mon 28 Oct 2019 12:03:15 PM CST
 ************************************************************************/

#ifndef _PARSE_IMAGE_CONFIG_H
#define _PARSE_IMAGE_CONFIG_H

#include <stdint.h>
#include "3dparty/cjson/cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char* get_image_conf_ip(cJSON* root);
extern uint16_t get_image_conf_port(cJSON* root);

#ifdef __cplusplus
}
#endif

#endif // _PARSE_IMAGE_CONFIG_H
