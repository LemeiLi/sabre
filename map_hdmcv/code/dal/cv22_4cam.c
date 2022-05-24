#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "dal/cv22_4cam.h"
#include "infra/net/net.h"
#include "infra/log/log.h"
#include "dal/cv22_data.h"
#include "uil/uil_image.h"

///< object method override
static void* cv224cam_get_object_address(void* pthis)
{
	zlog_info(g_zc, "cv22 4cam get_object_address override: %p", pthis);
	return pthis;
}
///< sensor methods realization
static void* cv224cam_read_raw(void* pthis) 
{
	struct CV224Cam* pthis_ = (struct CV224Cam*)pthis;
	
	return pthis_->link->download(pthis_->link, 1);
}

static void* cv224cam_read_cooked(void* pthis) 
{
	(void)pthis;
	zlog_info(g_zc, "%s", __func__); return 0;
}

///< image methods realization
static int cv224cam_set_sensitivity(void* pthis, float *sp)
{
	(void)pthis; (void)sp;
	zlog_info(g_zc, "%s", __func__); return 0;
}

static int cv224cam_reset_sensitivity(void* pthis)
{
	(void)pthis;
	zlog_info(g_zc, "%s", __func__); return 0;
}

///< cv224cam method
static int cv224cam_set_full_scale(CV224Cam* pthis, int fs)
{
	(void)pthis; (void)fs;
	zlog_info(g_zc, "%s", __func__); return 0;
}

static struct CV224CamVMT vmt_dev = {
	///< realization for BaseObject 
	.get_object_address = cv224cam_get_object_address,  ///< override

	///< realization for BaseSensor
	.read_raw = cv224cam_read_raw,
	.read_cooked = cv224cam_read_cooked,

	///< realization for BaseImage
	.set_sensitivity = cv224cam_set_sensitivity,
	.reset_sensitivity =  cv224cam_reset_sensitivity,
		
	///< realization for cv224cam 
	.cv224cam_set_full_scale = cv224cam_set_full_scale,
};

/**< A.1.1.4.1 */	
int CV224CamInit(CV224Cam* pthis, CV224CamConfig* pconf)
{
	pthis->vmt = &vmt_dev;	
	BaseImageInit(&pthis->super, pthis->vmt);

	///< object	
	pthis->get_object_address = pthis->vmt->get_object_address;
	///< sensor
	pthis->read_raw = pthis->vmt->read_raw;
	pthis->read_cooked = pthis->vmt->read_cooked;
	///< image
	pthis->set_sensitivity = pthis->vmt->set_sensitivity;
	pthis->reset_sensitivity = pthis->vmt->reset_sensitivity;
	///< cv22 4camera 
	pthis->cv224cam_set_full_scale = pthis->vmt->cv224cam_set_full_scale;

	pthis->config = pconf;
	pthis->state = 0;

	/**< A.1.1.4.1.1 */	
	pthis->link = construct_datalink(NULL);
	if (!pthis->link) return HAL_IMAGE_RET_EALLOC;
	pthis->run = true;

	/**< A.1.1.4.1.2 */	
	pthread_create(&pthis->rx_tid, NULL, recv_parse_cv22_data, pthis);
	zlog_info(g_zc, "cv224cam init success!");

	return HAL_IMAGE_RET_OK;
}

int CV224CamDeinit(void* pthis)
{ 
	int *ret = NULL;
	struct CV224Cam* pthis_ = (struct CV224Cam*)pthis;
	pthis_->run = false;
	pthread_join(pthis_->rx_tid, (void**)&ret);
	zlog_info(g_zc, "get recv_parse_cv22_data exit id: %d", (int)(intptr_t)ret);
	datalink_deinit(pthis_->link);

	return (int)(intptr_t)ret;
}

/**< A.1.1.4 */	
struct CV224Cam* construct_cv224cam(CV224CamConfig* conf)
{
	struct CV224Cam* pthis = (struct CV224Cam*)malloc(sizeof(*pthis));
	if (!pthis) return NULL;
	/**< A.1.1.4.1 */	
	if(CV224CamInit(pthis, conf)) {
		free(pthis);
		return NULL;
	}
	zlog_info(g_zc, "construct cv224cam!");

	return pthis;
}

int destruct_cv224cam(struct CV224Cam* pthis)
{
	int *ret = NULL;
	pthis->run = false;
	pthread_join(pthis->rx_tid, (void**)&ret);
	zlog_info(g_zc, "get recv_parse_cv22_data exit id: %d", (int)(intptr_t)ret);
	destruct_datalink(pthis->link);
	free(pthis);
	zlog_info(g_zc, "destruct cv224cam!");

	return (int)(intptr_t)ret;
}



