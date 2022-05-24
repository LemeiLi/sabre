/*************************************************************************
	> File Name: dal_lidar.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 10 Oct 2019 08:31:44 PM CST
 ************************************************************************/

#include <stdio.h>
#include "dal/dal_lidar.h"
#include "dal/dal_sensors.h"

static void* get_object_address(void* pthis)
{
	BaseLidar* _pthis = (BaseLidar*)pthis;

	///< use drived class realization
	if (_pthis->vmt->get_object_address != NULL)
		return _pthis->vmt->get_object_address(pthis);
	else  ///< use super class realization
		return _pthis->super.get_object_address(pthis);
}

void BaseLidarInit(void* pthis, void* vmt)
{
	BaseLidar* _pthis = (BaseLidar*)pthis;
	_pthis->vmt = (struct BaseLidarVMT*)vmt;
	BaseSensorInit(&_pthis->super, _pthis->vmt);

	///< object methods
	_pthis->get_object_address = get_object_address;
	///< sensor methods
	_pthis->read_raw = _pthis->vmt->read_raw;
	_pthis->read_cooked =  _pthis->vmt->read_cooked;
	///< lidar methods	
	_pthis->set_sensitivity = _pthis->vmt->set_sensitivity;
	_pthis->reset_sensitivity = _pthis->reset_sensitivity;
}


