/*************************************************************************
	> File Name: uil_lidar.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 22 Nov 2019 05:46:37 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pcap.h>
#include <sys/time.h>
#include "uil/uil_lidar.h"
#include "bll/bll_lidar.h"
#include "infra/stream/stream.h"
#include "infra/pack/datapack.h"
#include "infra/utils/utils.h"
#include "infra/log/log.h"

typedef struct hal_lidar{
	pthread_t mthread;
	bll_lidar blidar;
	hal_lidar_data_cb data_cb;
	hal_lidar_status_cb status_cb;
	const char* conf_file;
	volatile bool run;
}hal_lidar;

static hal_lidar g_ulidar;

static hal_lidar_data_t* construct_lidar_data(datapack* dp)
{
	//uint64_t time = dp->get_dts(dp);
	hal_lidar_data_t *data = (hal_lidar_data_t *)malloc(sizeof(*data));
	if (!data) 
		return NULL;

	data->uiLen = (unsigned int)dp->size(dp);
	data->pvBuf = (uint8_t *)malloc(data->uiLen);
	if (!data) {
		free(data);
		return NULL;
	}
	memcpy(data->pvBuf, (void*)dp->data(dp), data->uiLen);
	data->stTimeStamp.tv_sec = (__time_t)dp->get_dts(dp) / 1000000; 
	data->stTimeStamp.tv_usec = (__suseconds_t)dp->get_dts(dp) % 1000000;

	return data;
}

/*
static void destruct_lidar_data(hal_lidar_data_t* data)
{
	if (data) {
		if (data->pvBuf) 
			free(data->pvBuf);
		free(data);
	}
}
*/

static void* mainloop(void* args)
{
	int ret;
	hal_lidar* pthis = (hal_lidar*)args;
	log_info("enter uil lidar mainloop.");

	while (!get_exit_flag() && pthis->run) {
		/**< A.2.1. get datapack */
		datapack* dp = (datapack *)pthis->blidar.lidar->read_raw(pthis->blidar.lidar);
		if (!dp) continue;
		//log_info("data: %s, size: %ld, ts: %ld", 
		//		(char *)dp->data(dp), dp->size(dp), dp->get_dts(dp));

		/**< A.2.2 callback [ datapack->data(), datapack->size() ] */
		hal_lidar_data_t* data = construct_lidar_data(dp);
		if (unlikely(!data)) {
			ret = HAL_LIDAR_RET_EALLOC;
			if (pthis->status_cb)
				pthis->status_cb("allocate hal_lidar_data_t failed", &ret);
			break;
		}
		if (likely(pthis->data_cb))
			pthis->data_cb(data, HAL_LIDAR_RET_OK);

		/**< A.2.3. release */
		dp->release((void**)&dp);

		//destruct_lidar_data(data); ///< WARNING: user free
	}
	pthread_exit((void*)(intptr_t)ret);

	return (void*)HAL_LIDAR_RET_OK;
}

const char *hal_lidar_version(void)
{
	return "hal lidar version 1.0.0";
}

/**< A: hal_lidar_init */ 
msg_t hal_lidar_init(hal_lidar_t **pphLidarHandle, const char *pcCfgFileName)
{
	(void)pphLidarHandle; (void)pcCfgFileName;
	int ret;
	if (!pcCfgFileName) {
		fprintf(stderr, "lidar.json path parameter is null!");
		return HAL_LIDAR_RET_ELIDARCONF;
	}

	ret = log_init("zlog.conf");
	if (ret) {
		fprintf(stderr, "parse log config failed, please check zlog.conf\n");
		return HAL_LIDAR_RET_ELOGCONF;
	}

	///< A.1
	ret = bll_lidar_init(&g_ulidar.blidar, pcCfgFileName);
	if (ret) {
		log_fini();
		return HAL_LIDAR_RET_ELIDARINIT;
	}

	///< A.2
	g_ulidar.run = true;
	g_ulidar.conf_file = pcCfgFileName;
	pthread_create(&g_ulidar.mthread, NULL, mainloop, &g_ulidar);
	log_info("uil lidar init success.");

	return HAL_LIDAR_RET_OK;
}

/**< B: hal_lidar_deinit */
msg_t hal_lidar_deinit(hal_lidar_t *phLidarHandle)
{
	(void)phLidarHandle;
	int* ret;
	if (!g_ulidar.run)
		return HAL_LIDAR_RET_OK;
	g_ulidar.run = false;
	///< B.1
	pthread_join(g_ulidar.mthread, (void**)&ret);
	log_info("uil lidar mainloop exit: %d", (int)(intptr_t)ret);

	///< B.2
	bll_lidar_deinit(&g_ulidar.blidar);
	log_fini();

	return (msg_t)(intptr_t)ret;
}
  
msg_t hal_lidar_register_data_callback(hal_lidar_t *phLidarHandle, 
		                               hal_lidar_data_cb pfLidarDataCb, 
									   void *user)
{
	(void)phLidarHandle; (void) user;
	g_ulidar.data_cb = pfLidarDataCb;		
	return HAL_LIDAR_RET_OK;
}

msg_t hal_lidar_register_status_callback(hal_lidar_t *phLidarHandle, 
		                                 hal_lidar_status_cb pfStatusCb, 
										 void *user)
{
	(void)phLidarHandle; (void)user;
	g_ulidar.status_cb = pfStatusCb;
	return HAL_LIDAR_RET_OK;
}

/**< C: hal_lidar_stop */
msg_t hal_lidar_stop(hal_lidar_t *phLidarHandle, uint8_t ucStopMode)
{
	(void)phLidarHandle; (void)ucStopMode;
	int *ret;
	if (!g_ulidar.run)
		return HAL_LIDAR_RET_OK;

	g_ulidar.run = false;

	pthread_join(g_ulidar.mthread, (void**)&ret);
	log_info("uil lidar mainloop exit: %d", (int)(intptr_t)ret);
	bll_lidar_deinit(&g_ulidar.blidar);

	return (msg_t)(intptr_t)ret;
}
 
/**< D: hal_lidar_start */
msg_t hal_lidar_start(hal_lidar_t *phLidarHandle, uint8_t ucStartMode)
{
	(void)phLidarHandle; (void)ucStartMode;
	int ret;
	if (g_ulidar.run)
		return HAL_LIDAR_RET_OK;
	
	ret = bll_lidar_init(&g_ulidar.blidar, g_ulidar.conf_file);
	if (ret) {
		return HAL_LIDAR_RET_ELIDARINIT;
	}
	pthread_create(&g_ulidar.mthread, NULL, mainloop, &g_ulidar);
	g_ulidar.run = true;

	return HAL_LIDAR_RET_OK;
}
 
msg_t hal_lidar_pop_data (hal_lidar_t *phLidarHandle, 
		                  hal_lidar_data_t **ppstLidarData, 
						  int iWaitTimeMS, void *user)
{
	(void)phLidarHandle; (void)ppstLidarData; (void)iWaitTimeMS; (void)user;
	return HAL_LIDAR_RET_OK;
}

msg_t hal_lidar_push_data(hal_lidar_t *phLidarHandle, 
		                  hal_lidar_data_t **ppstLidarData, void *user)
{
	(void)phLidarHandle; (void)ppstLidarData; (void)user;
	return HAL_LIDAR_RET_OK;
}
  
msg_t hal_lidar_get(hal_lidar_t *phLidarHandle, 
		            char *pcGetTagJson, char *pcValueJson)
{
	(void)phLidarHandle; (void) pcGetTagJson; (void)pcValueJson;
	return HAL_LIDAR_RET_OK;
}

msg_t hal_lidar_set(hal_lidar_t *phLidarHandle, char *pcSetTagJson)
{
	(void)phLidarHandle; (void)pcSetTagJson;
	return HAL_LIDAR_RET_OK;
}





