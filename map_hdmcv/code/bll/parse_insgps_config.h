/*************************************************************************
	> File Name: parse_insgps_config.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 06 Nov 2019 08:22:14 PM CST
 ************************************************************************/

#ifndef _PARSE_INSGPS_CONFIG_H
#define _PARSE_INSGPS_CONFIG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "infra/serial/serial.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t get_insgps_conf_com_num(cJSON* root);
extern uint8_t get_insgps_conf_type_max(cJSON* root, size_t size);
extern int get_insgps_conf_com(cJSON* root, struct com_config *com_configs, size_t size);

#ifdef __cplusplus
}
#endif

#endif // _PARSE_INSGPS_CONFIG_H
