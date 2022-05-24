/*************************************************************************
	> File Name: dal_sensors.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 10 Oct 2019 08:21:07 PM CST
 ************************************************************************/

#include <stdio.h>
#include "dal/dal_object.h"
#include "dal/dal_sensors.h"
#include "infra/log/log.h"

///< 覆盖实现
/*
static void* get_object_address_ov(void* pthis)
{
	(void)pthis;
	zlog_info(g_zc, "%s.%s", __FILE__, __func__);

	return NULL;
}
*/

void BaseSensorInit(void* pthis, void* vmt)
{
	BaseSensor* _pthis= (BaseSensor*)pthis;
	_pthis->vmt = (struct BaseSensorVMT*)vmt;

	BaseObjectInit(&_pthis->super, _pthis->vmt);
	///< 1. 使用覆盖实现: 修改虚表 指向覆盖的方法
	//_pthis->vmt->get_object_address = get_object_address_ov;
	//_pthis->get_object_address = _pthis->vmt->get_object_address;
	///< 2. 调用基类的实现: 虚表不变 方法指向父类的实现
	 _pthis->get_object_address = _pthis->super.get_object_address;
	
	///< 提供 对象./->方法 的表现形式
	
	///< 本身的方法处理，这里不提供默认实现，需要派生类提供
	_pthis->read_raw = _pthis->vmt->read_raw;
	_pthis->read_cooked = _pthis->vmt->read_cooked;
}



