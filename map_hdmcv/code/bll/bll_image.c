/*************************************************************************
	> File Name: bll_image.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Mon 21 Oct 2019 08:22:59 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "infra/net/net.h"
#include "infra/net/tcp_service.h"
#include "dal/dal_image.h"
#include "dal/cv22_4cam.h"
#include "bll/bll_image.h"
#include "parse_image_config.h"
#include "infra/log/log.h"
#include "uil/uil_image.h"

static int image_start(void* pthis, uint8_t ucStartMode)
{
	(void)ucStartMode;
	bll_image* pthis_ = (bll_image*)pthis;
	if (pthis_->cam->run) return 0;
	pthis_->cam->run = true;

	return 0;
}
	
static int image_stop(void* pthis, uint8_t ucStopMode)
{
	(void)ucStopMode;
	bll_image* pthis_ = (bll_image*)pthis;
	pthis_->cam->run = false;

	return 0;
}

static int image_get_cfg(void* pthis, char *pcGetTagJson, char *pcValueJson)
{
	(void)pthis;
	(void)pcGetTagJson;
	(void)pcValueJson;

	return 0;
}

static int image_set_cfg(void* pthis, char *pcSetTagJson)
{
	(void)pthis;
	(void)pcSetTagJson;
	return 0;
}

static bll_image_vmt vmt = {
	.start = image_start,
	.stop = image_stop,
	.get_cfg = image_get_cfg,
	.set_cfg = image_set_cfg,
};

/**< A.1.1 */
int bll_image_init(void* pthis, const char* cfg_file) 
{	
	
	zlog_info(g_zc, "enter bll image init.");
	bll_image* pthis_ = (bll_image*)pthis;
	/**< A.1.1.1 */	
	bll_init(&pthis_->super, &vmt, cfg_file);

	pthis_->start = image_start;
	pthis_->stop = image_stop;
	pthis_->get_cfg = image_get_cfg;
	pthis_->set_cfg = image_set_cfg;

	/**< 1. construct camera config  */
	/**< A.1.1.2 */	
	CV224CamConfig* pconf = (CV224CamConfig*)malloc(sizeof(CV224CamConfig)); ///< need to free in deinit
	if (!pconf) return HAL_IMAGE_RET_EALLOC;
	/**< A.1.1.3 */	
	pconf->net = (net*)construct_tcp_client(
		(const uint8_t*)get_image_conf_ip(pthis_->super.root),
		(uint16_t)get_image_conf_port(pthis_->super.root), false);
	if (!pconf->net) {
		free(pconf);
		return HAL_IMAGE_RET_EALLOC;
	}
	pthis_->priv = pconf;

	/**< 2. realize camera object */
	/**< A.1.1.4 */	
	BaseImage* cam = (BaseImage*)construct_cv224cam(pconf);
	if (!cam) { 
		destruct_tcp_client((struct tcp_client*)pconf->net);
		free(pconf);
		return HAL_IMAGE_RET_EALLOC;
	}
	cam->run = true;
	pthis_->cam = cam;
	zlog_info(g_zc, "bll image init success.");

	return HAL_IMAGE_RET_OK;
}

int bll_image_deinit(void* pthis) 
{
	int ret;
	bll_image* pthis_ = (bll_image*)pthis;

	ret = CV224CamDeinit(pthis_->cam);
	free(pthis_->priv); ///< free config object
	bll_deinit(pthis_);

	return ret;
}
/* unused function
static bll_image* construct_bll_image(char* config)
{
	bll_image* pthis = (bll_image*)malloc(sizeof(*pthis));
	bll_image_init(pthis, config);

	return pthis;
}

static void destruct_bll_image(void* pthis)
{
	bll_image* pthis_ = (bll_image*)pthis;
	bll_deinit(pthis_);
	free(pthis_);
}
*/
