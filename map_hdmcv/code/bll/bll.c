/*************************************************************************
	> File Name: bll.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 25 Oct 2019 05:04:54 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "bll/bll.h"
#include "3dparty/cjson/cJSON.h"
#include "infra/stream/stream.h"
#include "infra/log/log.h"
#include "uil/uil_image.h"
#include "infra/utils/exception.h"

/**< A.1.1.1 */	
int bll_init(void* pthis, void* vmt, const char* cfg_file)
{
	bll* pthis_ = (bll*)pthis;
	pthis_->vptr = (bll_vmt*)vmt;
	
	pthis_->version = pthis_->vptr->version;
	pthis_->register_data_callback = pthis_->vptr->register_data_callback;
	pthis_->register_status_callback = pthis_->vptr->register_status_callback;
	pthis_->start = pthis_->vptr->start;
	pthis_->stop = pthis_->vptr->stop;
	pthis_->pop_data = pthis_->vptr->pop_data;
	pthis_->push_data = pthis_->vptr->push_data;
	pthis_->get_cfg = pthis_->vptr->get_cfg;
	pthis_->set_cfg = pthis_->vptr->set_cfg;

	/**< get config info from [device].json */		
	char* cfg_stream = read_file(cfg_file); ///< need to free in deinit
	if (!cfg_stream) {
		log_error("read configuration file: %s failed!", cfg_file);
		return HAL_EDEVCONF;
	} else {
		log_info("read %s success!", cfg_file);
	}
	pthis_->config = (int8_t*)cfg_stream;

	/**< format config stream to json stream */
	cJSON* root = cJSON_Parse(cfg_stream);
	if (!root) {
		log_error("parse %s failed!", cfg_file);
		return HAL_EDEVCONF;
	} else {
		log_info("parse %s success!", cfg_file);
	}
	pthis_->root = root;

	return HAL_OK;
}

void bll_deinit(void* pthis)
{
	bll* pthis_ = (bll*)pthis;
	if (!pthis) return;

	if (pthis_->root)
		cJSON_Delete(pthis_->root);
	if (pthis_->config)
		free(pthis_->config);
}

