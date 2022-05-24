/*************************************************************************
	> File Name: dal_object.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 11 Oct 2019 10:32:26 AM CST
 ************************************************************************/

#include <stdio.h>
#include "dal/dal_object.h"
#include "infra/log/log.h"

static void* get_object_address_impl(void* pthis)
{
	BaseObject* _pthis = (BaseObject*)pthis;

	if(_pthis->vmt->get_object_address != NULL)
		///< 使用子类的实现
		return _pthis->vmt->get_object_address(pthis);
	else 
		///< 如果子类没有提供覆盖实现，则使用该默认实现
		zlog_info(g_zc, "default implementation base object address: %p", pthis);
	return NULL;
}

void BaseObjectInit(void* pthis, void* vmt)
{
	BaseObject* _pthis = (BaseObject*)pthis;
	_pthis->vmt = (struct BaseObjectVMT*)vmt;
	_pthis->get_object_address = get_object_address_impl; 
}




