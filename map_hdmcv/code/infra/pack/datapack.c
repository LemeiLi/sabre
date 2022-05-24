/*************************************************************************
	> File Name: datapack.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 18 Oct 2019 08:17:15 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "infra/pack/datapack.h"
#include "infra/log/log.h"

static void datapack_set_dts(void* pthis, uint64_t dts)
{
	struct datapack* pthis_ = (struct datapack*)pthis;
	if (pthis_->vptr && pthis_->vptr->set_dts)
		pthis_->vptr->set_dts(pthis, dts);
	else 
		pthis_->dts = dts;
}

static uint64_t datapack_get_dts(void* pthis)
{
	struct datapack* pthis_ = (struct datapack*)pthis;
	if (pthis_->vptr && pthis_->vptr->get_dts)
		return pthis_->vptr->get_dts(pthis);
	else 
		return pthis_->dts;
}
    
static void datapack_set_type(void* pthis, uint8_t type)
{
	struct datapack* pthis_ = (struct datapack*)pthis;
	

	if (pthis_->vptr && pthis_->vptr->set_type)
		pthis_->vptr->set_type(pthis, type);
	else 
		pthis_->type = type;
}

static uint8_t datapack_get_type(void* pthis)
{
	struct datapack* pthis_ = (struct datapack*)pthis;

	
	if (pthis_->vptr && pthis_->vptr->get_type)
		return pthis_->vptr->get_dts(pthis);
	else 
		return pthis_->type;
}

static void* datapack_data(void* pthis)
{
	struct datapack* pthis_ = (struct datapack*)pthis;

	if (pthis_->vptr && pthis_->vptr->data) {
		return pthis_->vptr->data(pthis);
	}
	else {
		return pthis_->mdata;
	}
}

static size_t datapack_size(void* pthis)
{
	struct datapack* pthis_ = (struct datapack*)pthis;
	if (pthis_->vptr && pthis_->vptr->size)
		return pthis_->vptr->size(pthis);
	else 
		return pthis_->msize;
}

static void datapack_release(void** pthis) 
{
	struct datapack* pthis_ = *(struct datapack**)pthis;
	if (pthis_->vptr && pthis_->vptr->release)
		return pthis_->vptr->release(pthis);
	else {
		if (--pthis_->mref) 
			return;
		else { 
			free(pthis_);
			//log_info("free datapack: addr <%p>", pthis);
			pthis_ = NULL;
		}
	}
}

void datapack_init(void* pthis, void* vmt, void* saddr, size_t size)
{
	struct datapack* pthis_ = (struct datapack*)pthis;
	
	pthis_->maddr = saddr;
	pthis_->msize = size;
	pthis_->mref = 1;
	pthis_->vptr = (struct datapack_vmt*)vmt;

	pthis_->set_dts = datapack_set_dts; 
	pthis_->get_dts = datapack_get_dts; 
	pthis_->data = datapack_data;
	pthis_->size = datapack_size;
	pthis_->release = datapack_release;
	pthis_->set_type = datapack_set_type;
	pthis_->get_type = datapack_get_type;
}

struct datapack* construct_datapack(void* vmt, void* saddr, size_t size)
{
	struct datapack* pthis = (datapack*)malloc(sizeof(*pthis) + size);
	if (!pthis) return NULL;
	memset(pthis, 0, sizeof(*pthis) + size);

	datapack_init(pthis, vmt, saddr, size);
	memcpy(pthis->mdata, saddr, size);
	//log_info("construct datapack: addr <%p>, size %ld", 
	//        pthis, sizeof(*pthis) + size);
	
	return pthis;
}

void destruct_dapapack(struct datapack* pthis)
{
	if (pthis)
		free(pthis);
}

