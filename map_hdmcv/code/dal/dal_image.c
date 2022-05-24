/*************************************************************************
	> File Name: hal_image.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 10 Oct 2019 08:31:44 PM CST
 ************************************************************************/

#include <stdio.h>
#include "dal/dal_image.h"
#include "dal/dal_sensors.h"

static void* get_object_address(void* pthis)
{
	BaseImage* _pthis = (BaseImage*)pthis;
	void* addr = NULL;

	///< use drived class realization
	if (_pthis->vmt->get_object_address != NULL)
		addr = _pthis->vmt->get_object_address(pthis);
	else  ///< use super class realization
		addr = _pthis->super.get_object_address(pthis);

	return addr;
}

void BaseImageInit(void* pthis, void* vmt)
{
	BaseImage* _pthis = (BaseImage*)pthis;
	_pthis->vmt = (struct BaseImageVMT*)vmt;
	BaseSensorInit(&_pthis->super, _pthis->vmt);

	///< object methods
	_pthis->get_object_address = get_object_address;
	///< sensor methods
	_pthis->read_raw = _pthis->vmt->read_raw;
	_pthis->read_cooked =  _pthis->vmt->read_cooked;
	///< image methods	
	_pthis->set_sensitivity = _pthis->vmt->set_sensitivity;
	_pthis->reset_sensitivity = _pthis->reset_sensitivity;
}


