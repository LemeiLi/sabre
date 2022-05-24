/*************************************************************************
	> File Name: bll_lidar.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 22 Nov 2019 02:22:48 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "infra/net/net.h"
#include "infra/net/udp_service.h"
#include "dal/dal_lidar.h"
#include "dal/vlp32c.h"
#include "bll/bll_lidar.h"
#include "parse_lidar_config.h"
#include "infra/log/log.h"
#include "infra/utils/exception.h"
#include "infra/net/udp_raw.h"

static int lidar_start(void* pthis, uint8_t ucStartMode)
{
	(void)ucStartMode;
	bll_lidar* pthis_ = (bll_lidar*)pthis;
	if (pthis_->lidar->run) return 0;
	pthis_->lidar->run = true;

	return 0;
}
	
static int lidar_stop(void* pthis, uint8_t ucStopMode)
{
	(void)ucStopMode;
	bll_lidar* pthis_ = (bll_lidar*)pthis;
	pthis_->lidar->run = false;

	return 0;
}

static int lidar_get_cfg(void* pthis, char *pcGetTagJson, char *pcValueJson)
{
	(void)pthis;
	(void)pcGetTagJson;
	(void)pcValueJson;

	return 0;
}

static int lidar_set_cfg(void* pthis, char *pcSetTagJson)
{
	(void)pthis;
	(void)pcSetTagJson;

	return 0;
}

static bll_lidar_vmt vmt = {
	.start = lidar_start,
	.stop = lidar_stop,
	.get_cfg = lidar_get_cfg,
	.set_cfg = lidar_set_cfg,
};

/**< A.1.1 */
int bll_lidar_init(void* pthis, const char* cfg_file) 
{	
	bll_lidar* pthis_ = (bll_lidar*)pthis;
	/**< A.1.1.1 */	
	bll_init(&pthis_->super, &vmt, cfg_file);

	pthis_->start = lidar_start;
	pthis_->stop = lidar_stop;
	pthis_->get_cfg = lidar_get_cfg;
	pthis_->set_cfg = lidar_set_cfg;

	/**< 1. construct lidar config  */
	/**< A.1.1.2 */	
	VLP32CConfig* pconf = (VLP32CConfig*)malloc(sizeof(VLP32CConfig)); ///< need to free in deinit
	if (!pconf) return HAL_EALLOC;

	/**< A.1.1.3 */	
	pconf->dhnet = (net *)construct_udp_server(
			(const uint8_t *)get_lidar_conf_host_ip(pthis_->super.root),
			(uint16_t)get_lidar_conf_host_data_port(pthis_->super.root),
			false);
	if (!pconf->dhnet) {
		free(pconf);
		return HAL_EUDP;
	}
	strcpy((char * restrict)pconf->mac, get_lidar_conf_lidar_mac(pthis_->super.root));
	strcpy((char * restrict)pconf->dhitf, get_lidar_conf_host_if(pthis_->super.root));

	pthis_->priv = pconf;

	/**< 2. realize vlp32c object */
	/**< A.1.1.4 */	
	BaseLidar* lidar = (BaseLidar*)construct_vlp32c(pconf);
	if (!lidar) { 
		destruct_udp_server((struct udp_server*)pconf->dhnet);
		free(pconf);
		return HAL_ELIDAR;
	}
	lidar->run = true;
	pthis_->lidar = lidar;
	zlog_info(g_zc, "bll lidar init success.");

	return HAL_OK;
}

int bll_lidar_deinit(void* pthis) 
{
	int ret;
	bll_lidar *pthis_ = (bll_lidar *)pthis;

	ret = VLP32CDeinit(pthis_->lidar);
	destruct_udp_server((struct udp_server *)
			            ((VLP32CConfig *)pthis_->priv)->dhnet);
	free(pthis_->priv); ///< free config object
	bll_deinit(pthis_);

	return ret;
}

/* unused function
static bll_lidar* construct_bll_lidar(char* config)
{
	bll_lidar* pthis = (bll_lidar*)malloc(sizeof(*pthis));
	bll_lidar_init(pthis, config);

	return pthis;
}

static void destruct_bll_lidar(void* pthis)
{
	bll_lidar* pthis_ = (bll_lidar*)pthis;
	bll_deinit(pthis_);
	free(pthis_);
}
*/




