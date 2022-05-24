/*************************************************************************
	> File Name: datalink.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Mon 21 Oct 2019 04:07:24 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "infra/link/fifo.h"
#include "infra/link/datalink.h"
#include "infra/log/log.h"
#include "uil/uil_image.h"

static int upload_default(void* pthis, void* data)
{
	datalink* pthis_ = (datalink*)pthis;
	if (pthis_->vptr && pthis_->vptr->upload)
		return pthis_->vptr->upload(pthis, data);
	else  {
		//log_info("upload <%p> to datalink", data);
		return fifo_push(pthis_->link, data);
	}
}

static void* download_default(void* pthis, int32_t timeout_sec)
{
	datalink* pthis_ = (datalink*)pthis;
	if (pthis_->vptr && pthis_->vptr->download)
		return pthis_->vptr->download(pthis, timeout_sec);
	else  {
		void* data = fifo_pop(pthis_->link, timeout_sec);
		//log_info("download <%p> from datalink", data);
		return data;
	}
}

int datalink_init(void* pthis, void* vptr)
{
	datalink* pthis_ = (datalink*)pthis;
	pthis_->link = fifo_new();
	if (!pthis_->link) return HAL_IMAGE_RET_EALLOC;

	datalink_vmt* vptr_ = (datalink_vmt*)vptr;
	pthis_->vptr = vptr_;

	pthis_->upload = upload_default;
	pthis_->download = download_default;

	return HAL_IMAGE_RET_OK;
}

void datalink_deinit(void* pthis)
{
	datalink* pthis_ = (datalink*)pthis;

	/**< clean datalink */
	for (uint32_t i = 0; i < get_nr_active(pthis_->link); i++) {
		free(fifo_pop(pthis_->link, 1));
	}

	fifo_free(pthis_->link);
}

datalink* construct_datalink(void* vptr)
{
	int ret;
	datalink* pthis = (datalink *)malloc(sizeof(*pthis));
	if (!pthis) return NULL;
	ret = datalink_init(pthis, vptr);
	if (ret) return NULL;
	//zlog_info(g_zc, "construct datalink!");

	return pthis;
}

void destruct_datalink(datalink* pthis)
{
	datalink_deinit(pthis);
	free(pthis);
	//zlog_info(g_zc, "destruct datalink!");
}



