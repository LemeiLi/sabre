#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "uil/uil_image.h"
#include "bll/bll_image.h"
#include "infra/stream/stream.h"
#include "infra/pack/datapack.h"
#include "infra/utils/utils.h"
#include "infra/log/log.h"

typedef struct hal_image {
	pthread_t mthread;
	bll_image bimage;
	hal_image_data_cb data_cb;
	hal_image_status_cb status_cb;
	const char* config;
	volatile bool run;
}hal_image;

static hal_image g_uimage;
hal_image_status_cb g_status_cb;

static hal_image_data_t* construct_image_data(datapack* dp)
{
	uint64_t time = dp->get_dts(dp);
	
	hal_image_data_t* data = (hal_image_data_t *)malloc(sizeof(*data));
	if (!data) return NULL;
	data->stTimeStamp.tv_usec = (__suseconds_t)time % 1000000;
	data->stTimeStamp.tv_sec = (__time_t)time / 1000000;
	data->uiLen = (unsigned int)dp->size(dp);
	data->pvBuf = malloc(data->uiLen);
	///< [0-7]: sensor model, [8-15]: image type 
	data->type = (dp->type & 0xF0) >> 4; 
	if (!data) {
		free(data);
		return NULL;
	}
	memcpy(data->pvBuf, (void*)dp->data(dp), data->uiLen);

	return data;
}
/*
static void destruct_image_data(hal_image_data_t* data)
{
	if (data) {
		if (data->pvBuf) 
			free(data->pvBuf);
		free(data);
	}
}
*/
/**< A.2 */
static void* mainloop(void* args)
{
	int ret = 0;
	hal_image* pthis = (hal_image*)args;
	zlog_info(g_zc, "enter uil image mainloop.");
	while (!get_exit_flag() && pthis->run) {
		/**< A.2.1. get datapack */
		datapack* dp = (datapack* )pthis->bimage.cam->read_raw(pthis->bimage.cam);
		if (!dp) continue;
		zlog_info(g_zc, "data: %p, size: %ld, ts: %ld, type: %x\n", 
				  dp->data(dp), dp->size(dp), dp->get_dts(dp), dp->get_type(dp));
		/**< A.2.2 callback [ datapack->data(), datapack->size() ] */
		hal_image_data_t* data = construct_image_data(dp);
		if (unlikely(!data)) {
			ret = HAL_IMAGE_RET_EALLOC;
			if (pthis->status_cb)
				pthis->status_cb("allocate hal_image_data_t failed", &ret);
			break;
		}		
		if (likely(pthis->data_cb))
			pthis->data_cb(data, HAL_IMAGE_RET_OK);
		/**< A.2.3. release */
		dp->release((void**)&dp);

		//destruct_image_data(data); ///< WARNING: user free
	}
	zlog_info(g_zc, "image mainloop exit!");
	pthread_exit((void*)(intptr_t)ret);
}

const char *hal_image_version(void)
{
	return "hal image version 1.0.0";
}

/** 
 * A: hal_image_init 
 */ 
msg_t hal_image_init(hal_image_t **pphImageHandle, const char *pcCfgFileName)
{
	int ret;
	(void)pphImageHandle;
	if (!pcCfgFileName) {
		fprintf(stderr, "image.json path parameter is null!");
		return HAL_IMAGE_RET_ECAMCONF;
	}

	ret = log_init("zlog.conf");
	if (ret) {
		fprintf(stderr, "parse log config failed, please check zlog.conf\n");
		return HAL_IMAGE_RET_ELOGCONF;
	}

	///< A.1
	ret = bll_image_init(&g_uimage.bimage, pcCfgFileName);
	if (ret) {
		log_fini();
		return HAL_IMAGE_RET_ECAMINIT;
	}
	///< A.2
	pthread_create(&g_uimage.mthread, NULL, mainloop, &g_uimage);
	g_uimage.run = true;
	g_uimage.config = pcCfgFileName;
	zlog_info(g_zc, "uil image init success.");

	return HAL_IMAGE_RET_OK;
}

/**
 * B: hal_image_deinit 
 */
msg_t hal_image_deinit(hal_image_t *phImageHandle)
{
	int* ret;
	(void)phImageHandle;
	if (!g_uimage.run)
		return HAL_IMAGE_RET_OK;
	g_uimage.run = false;
	///< B.1
	bll_image_deinit(&g_uimage.bimage);
	///< B.2
	pthread_join(g_uimage.mthread, (void**)&ret);
	zlog_info(g_zc, "mainloop exit: %d", (int)(intptr_t)ret);
	log_fini();

	return (msg_t)(intptr_t)ret;
}
  
msg_t hal_image_register_data_callback(hal_image_t *phImageHandle, hal_image_data_cb pfImageDataCb, void *user)
{

	(void)phImageHandle;
	(void)user;
	g_uimage.data_cb = pfImageDataCb;		
	return HAL_IMAGE_RET_OK;
}

msg_t hal_image_register_status_callback(hal_image_t *phImageHandle, hal_image_status_cb pfStatusCb, void *user)
{
	(void)phImageHandle;
	(void)user;
	g_uimage.status_cb = pfStatusCb;
	g_status_cb = pfStatusCb;
	return HAL_IMAGE_RET_OK;
}

/**< C: hal_image_stop */
msg_t hal_image_stop(hal_image_t *phImageHandle, uint8_t ucStopMode)
{
	int *ret;
	(void)phImageHandle;
	(void)ucStopMode;
	g_uimage.run = false;
	bll_image_deinit(&g_uimage.bimage);
	pthread_join(g_uimage.mthread, (void**)&ret);
	zlog_info(g_zc, "mainloop exit: %d", (int)(intptr_t)ret);

	return (msg_t)(intptr_t)ret;
}
 
/**< D: hal_image_start */
msg_t hal_image_start(hal_image_t *phImageHandle, uint8_t ucStartMode)
{
	int ret;
	(void)phImageHandle;
	(void)ucStartMode;
	if (g_uimage.run)
		return HAL_IMAGE_RET_OK;
	
	ret = bll_image_init(&g_uimage.bimage, g_uimage.config);
	if (ret) {
		return HAL_IMAGE_RET_ECAMINIT;
	}
	pthread_create(&g_uimage.mthread, NULL, mainloop, &g_uimage);
	g_uimage.run = true;

	return HAL_IMAGE_RET_OK;
}
 
msg_t hal_image_pop_data (hal_image_t *phImageHandle, hal_image_data_t **ppstImageData, int iWaitTimeMS, void *user)
{
	(void)phImageHandle; (void)ppstImageData; (void)iWaitTimeMS; (void)user;
	return HAL_IMAGE_RET_OK;
}

msg_t hal_image_push_data(hal_image_t *phImageHandle, hal_image_data_t **ppstImageData, void *user)
{
	(void)phImageHandle; (void)ppstImageData; (void)user;
	return HAL_IMAGE_RET_OK;
}
  
msg_t hal_image_get(hal_image_t *phImageHandle, char *pcGetTagJson, char *pcValueJson)
{
	(void)phImageHandle; (void)pcGetTagJson; (void)pcValueJson;
	return HAL_IMAGE_RET_OK;
}

msg_t hal_image_set(hal_image_t *phImageHandle, char *pcSetTagJson)
{
	(void)phImageHandle; (void)pcSetTagJson;
	return HAL_IMAGE_RET_OK;
}



