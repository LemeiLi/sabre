/*************************************************************************
	> File Name: bll_insgps.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Mon 21 Oct 2019 08:22:59 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "infra/pack/datapack.h"
#include "infra/log/log.h"
#include "dal/dal_insgps.h"
#include "dal/xwgi7660.h"
#include "bll/bll_insgps.h"
#include "parse_insgps_config.h"
#include "infra/utils/exception.h"

static int insgps_start(void* pthis, uint8_t ucStartMode)
{
	(void)ucStartMode;
	bll_insgps* pthis_ = (bll_insgps*)pthis;
	if (pthis_->insgps->run) return HAL_OK;
	pthis_->insgps->run = true;

	return HAL_OK;
}
	
static int insgps_stop(void* pthis, uint8_t ucStopMode)
{
	(void)ucStopMode;
	bll_insgps* pthis_ = (bll_insgps*)pthis;
	pthis_->insgps->run = false;

	return HAL_OK;
}

static int insgps_get_cfg(void* pthis, char *pcGetTagJson, char *pcValueJson)
{
	(void)pthis;
	(void)pcGetTagJson;
	(void)pcValueJson;

	return HAL_OK;
}

static int insgps_set_cfg(void* pthis, char *pcSetTagJson)
{
	(void)pthis;
	(void)pcSetTagJson;

	return HAL_OK;
}

static int insgps_write_rtk(void* pthis, char* data, size_t size)
{
	bll_insgps* pthis_ = (bll_insgps*)pthis;
	return pthis_->insgps->write_rtk(pthis_->insgps, data, size);
}

static bll_insgps_vmt vmt = {
	.start = insgps_start,
	.stop = insgps_stop,
	.get_cfg = insgps_get_cfg,
	.set_cfg = insgps_set_cfg,
	.write_rtk = insgps_write_rtk, 
};

/**< A.1.1 */
int bll_insgps_init(void* pthis, const char* cfg_file) 
{	
	bll_insgps* pthis_ = (bll_insgps*)pthis;
	/**< A.1.1.1 */	
	bll_init(&pthis_->super, &vmt, cfg_file);

	pthis_->start = insgps_start;
	pthis_->stop = insgps_stop;
	pthis_->get_cfg = insgps_get_cfg;
	pthis_->set_cfg = insgps_set_cfg;
	pthis_->write_rtk = insgps_write_rtk;

	/**< construct COMs config  */
	/**< A.1.1.2 */	
	/**< need to free in deinit */
	XWGI7660Config* pconf = (XWGI7660Config*)malloc(sizeof(XWGI7660Config)); 
	if (!pconf) {
		log_error("malloc XWGI7660Config error!");
		return HAL_EALLOC;
	}
	memset(pconf, 0, sizeof(*pconf));

	/**< parse config file */
	/**< A.1.1.3 */	
	pconf->com_num = get_insgps_conf_com_num(pthis_->super.root);
	com_config* com_confs = (com_config*)malloc((sizeof(*com_confs)) *
			                 pconf->com_num);
	if (!com_confs) {
		log_error("malloc com_config error!");
		free(pconf);
		return HAL_EALLOC;
	}
	get_insgps_conf_com(pthis_->super.root, com_confs, pconf->com_num); 
	if (!pconf->com_num) {
		free(pconf);
		log_error("no valid COM find!");
		return HAL_ESERIAL;
	}

	for (int i = 0; i < pconf->com_num; i++) {
		pconf->com_conf[i] = &com_confs[i];
		/**< realize serial object */
		serial* ps = construct_serial(com_confs[i].dev, com_confs[i].baudrate);
		if (ps)
			pconf->coms[i] = ps;
		else
			log_error("construct serail object: %s[%d] failed", 
			          com_confs[i].dev, com_confs[i].baudrate);
	}
	pthis_->priv = pconf;

	/**< 2. realize xwgi7660 object */
	/**< A.1.1.4 */	
	BaseINSGPS* insgps = (BaseINSGPS*)construct_xwgi7660(pconf);
	if (!insgps) { 
		for (int i = 0; i < pconf->com_num; i++)
			destruct_serial(pconf->coms[i]);
		free(pconf);
		free(com_confs);
		return HAL_EALLOC;
	}
	pthis_->insgps = insgps;
	//log_info("bll insgps init success: insgps %p, priv %p", insgps, pconf);

	return HAL_OK;
}

int bll_insgps_deinit(void* pthis) 
{
	bll_insgps* pthis_ = (bll_insgps*)pthis;
	XWGI7660Config* pconf = (XWGI7660Config*)pthis_->priv;
	
	XWGI7660DeInit(pthis_->insgps);
	for (int i = 0; i < pconf->com_num; i++)
		destruct_serial(pconf->coms[i]);
	free(pconf->com_conf[0]);
	free(pconf);          ///< free config object
	bll_deinit(pthis_);

	return HAL_OK;
}

/* unused function
static bll_insgps* construct_bll_insgps(char* config)
{
	bll_insgps* pthis = (bll_insgps*)malloc(sizeof(*pthis));
	bll_insgps_init(pthis, config);

	return pthis;
}

static void destruct_bll_insgps(void* pthis)
{
	bll_insgps* pthis_ = (bll_insgps*)pthis;
	bll_insgps_deinit(pthis);
	free(pthis_);
}
*/



