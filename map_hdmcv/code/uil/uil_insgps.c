#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "uil/uil_insgps.h"
#include "bll/bll_insgps.h"
#include "infra/stream/stream.h"
#include "infra/pack/datapack.h"
#include "infra/utils/utils.h"
#include "infra/log/log.h"
#include "infra/serial/serial.h"
#include "infra/utils/exception.h"

typedef struct hal_insgps {
	pthread_t mthread;
	bll_insgps binsgps;
	hal_insgps_data_cb data_cb;
	hal_insgps_status_cb status_cb;
	const char* conf_file;
	volatile bool run;
}hal_insgps;

static hal_insgps g_insgps;
hal_insgps_status_cb g_insgps_status_cb;

static hal_insgps_data_t* construct_insgps_data(datapack* dp)
{
	//uint64_t time = dp->get_dts(dp);
	struct com_config* conf = (struct com_config*)dp->priv;
	
	hal_insgps_data_t* data = (hal_insgps_data_t *)malloc(sizeof(*data));
	if (!data) {
		log_error("malloc error");
		return NULL;
	}
	/**< unused timestamp */
	//data->stTimeStamp.tv_usec = time % 1000000;
	//data->stTimeStamp.tv_sec = time / 1000000;
	data->uiLen = (unsigned int)dp->size(dp);
	data->pvBuf = malloc(data->uiLen);
	if (!data) {
		free(data);
		log_error("malloc error");
		return NULL;
	}
	memset(data->pvBuf, 0, data->uiLen);
	memcpy(data->pvBuf, (void*)dp->data(dp), data->uiLen);
	data->priv = conf->type[0];

	return data;
}

static inline void destruct_insgps_data(hal_insgps_data_t* data)
{
	if (data) {
		if (data->pvBuf) 
			free(data->pvBuf);
		free(data);
	}
}

/**< A.2 */
static void* mainloop(void* args)
{
	int ret;
	hal_insgps* pthis = (hal_insgps*)args;
	BaseINSGPS* insgps = pthis->binsgps.insgps;

	while (!get_exit_flag() && pthis->run) {
		/**< A.2.1 get datapack */
		datapack* dp = (datapack *)insgps->read_raw(insgps);
		if (!dp) continue;
		//struct com_config* conf = (struct com_config*)dp->priv;
		//log_info("uil: recved [%s], size %ld: <%p> %s", 
	    //    conf->type[0], dp->size(dp), dp, (char*)dp->data(dp));
		
		/**< A.2.2 invoke callback */
		hal_insgps_data_t* data = construct_insgps_data(dp);
		if (unlikely(!data)) {
			ret = HAL_EALLOC;
			if (pthis->status_cb)
				pthis->status_cb("allocate hal_insgps_data_t failed", &ret);
			break;
		}		
		if (likely(pthis->data_cb))
			pthis->data_cb(data, HAL_INSGPS_RET_OK);

		/**< A.2.3 release datapack */
		dp->release((void**)&dp);
		//destruct_insgps_data(data); ///< WARNING: user free
	}
	pthread_exit((void*)(intptr_t)ret);

	return (void*)HAL_INSGPS_RET_OK;
}

const char *hal_insgps_version(void)
{
	static const char* version = "hal insgps version 1.0.0";
	return version; 
}

/**
 * A: hal_insgps_init 
 */ 
msg_t hal_insgps_init(hal_insgps_t **pphInsgpsHandle, const char *pcCfgFileName)
{
	(void)pphInsgpsHandle; 
	int ret;
	if (!pcCfgFileName) {
		fprintf(stderr, "insgps.json path parameter is null!");
		return HAL_INSGPS_RET_ECONF;
	}
	ret = log_init("zlog.conf");
	if (ret) {
		fprintf(stderr, "parse log config failed, please check zlog.conf\n");
		return HAL_INSGPS_RET_ELOGCONF;
	}

	/**< A.1 */
	ret = bll_insgps_init(&g_insgps.binsgps, pcCfgFileName);
	if (ret) {
		log_fini();
		return HAL_INSGPS_RET_ESERINIT; 
	}
	/**< A.2 */
	g_insgps.run = true;
	g_insgps.conf_file = pcCfgFileName;
	pthread_create(&g_insgps.mthread, NULL, mainloop, &g_insgps);

	return HAL_INSGPS_RET_OK;
}

/**
 * B: hal_insgps_deinit 
 */
msg_t hal_insgps_deinit(hal_insgps_t* phInsgpsHandle)
{
	(void)phInsgpsHandle;
	int* ret;
	/**< B.1 */
	if (!g_insgps.run)
		return HAL_INSGPS_RET_OK;
	g_insgps.run = false;
	pthread_join(g_insgps.mthread, (void**)&ret);
	zlog_info(g_zc, "mainloop exit: %d", (int)(intptr_t)ret);
	/**< B.2 */
	bll_insgps_deinit(&g_insgps.binsgps);
	log_fini();

	return (msg_t)(intptr_t)ret;
}
  
msg_t hal_insgps_register_data_callback(hal_insgps_t *phInsgpsHandle, 
		                                hal_insgps_data_cb pfInsgpsDataCb, 
										void *user)
{
	(void)phInsgpsHandle; (void)user;
	g_insgps.data_cb = pfInsgpsDataCb;		

	return HAL_INSGPS_RET_OK;
}

msg_t hal_insgps_register_status_callback(hal_insgps_t *phInsgpsHandle, 
		                                  hal_insgps_status_cb pfStatusCb, 
										  void *user)
{
	(void)phInsgpsHandle; (void)user;
	g_insgps.status_cb = pfStatusCb;

	return HAL_INSGPS_RET_OK;
}

/**< C: hal_insgps_stop */
msg_t hal_insgps_stop(hal_insgps_t *phInsgpsHandle, uint8_t ucStopMode)
{
	(void)phInsgpsHandle; (void)ucStopMode;
	int *ret;
	if (!g_insgps.run)
		return HAL_INSGPS_RET_OK;

	g_insgps.run = false;
	pthread_join(g_insgps.mthread, (void**)&ret);
	zlog_info(g_zc, "mainloop exit: %d", (int)(intptr_t)ret);
	bll_insgps_deinit(&g_insgps.binsgps);

	return (msg_t)(intptr_t)ret;
}
 
/**< D: hal_insgps_start */
msg_t hal_insgps_start(hal_insgps_t *phInsgpsHandle, uint8_t ucStartMode)
{
	(void)phInsgpsHandle; (void)ucStartMode;
	int ret;
	if (g_insgps.run)
		return HAL_INSGPS_RET_OK;
	
	ret = bll_insgps_init(&g_insgps.binsgps, g_insgps.conf_file);
	if (ret) return HAL_INSGPS_RET_ESERINIT; 
	g_insgps.run = true;
	pthread_create(&g_insgps.mthread, NULL, mainloop, &g_insgps);

	return HAL_INSGPS_RET_OK;
}

msg_t hal_insgps_rtk(hal_insgps_t *phInsgpsHandle, char *pcRtkJson, size_t size)
{
	(void)phInsgpsHandle; (void)pcRtkJson; (void)size;
	g_insgps.binsgps.write_rtk(&g_insgps.binsgps, pcRtkJson, size);

	return HAL_INSGPS_RET_OK;
}
 
msg_t hal_insgps_pop_data (hal_insgps_t *phInsgpsHandle, 
		                   hal_insgps_data_t **ppstInsgpsData, 
						   int iWaitTimeMS, void *user)
{
	(void)phInsgpsHandle; (void)ppstInsgpsData; (void)iWaitTimeMS; (void)user;
	return HAL_INSGPS_RET_OK;
}

msg_t hal_insgps_push_data(hal_insgps_t *phInsgpsHandle, 
		                   hal_insgps_data_t **ppstInsgpsData, 
						   void *user)
{
	(void)phInsgpsHandle; (void)ppstInsgpsData; (void)user;
	return HAL_INSGPS_RET_OK;
}
  
msg_t hal_insgps_get(hal_insgps_t *phInsgpsHandle, 
			         char *pcGetTagJson, char *pcValueJson)
{
	(void)phInsgpsHandle; (void)pcGetTagJson; (void)pcValueJson;
	return HAL_INSGPS_RET_OK;
}

msg_t hal_insgps_set(hal_insgps_t *phInsgpsHandle, char *pcSetTagJson)
{
	(void)phInsgpsHandle; (void)pcSetTagJson;
	return HAL_INSGPS_RET_OK;
}



