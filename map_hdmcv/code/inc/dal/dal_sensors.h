#ifndef _HAL_SENSOR_H
#define _HAL_SENSOR_H

#include <stdint.h>
#include <stdbool.h>
#include "dal_object.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _base_sensor_methods_alone                          \
	void* (*read_raw)(void* pthis);                      \
	void* (*read_cooked)(void* pthis);                   \

#define _base_sensor_methods \
	_base_object_methods     \
	_base_sensor_methods_alone

struct BaseSensorVMT {
	_base_sensor_methods
};

#define _base_sensor_data \
	_base_object_data     \
	volatile bool run;    \
	uint8_t index;

typedef struct {
	BaseObject super;
	struct BaseSensorVMT* vmt;
	_base_sensor_methods
	_base_sensor_data 
} BaseSensor;

extern void BaseSensorInit(void* pthis, void* vmt);

#ifdef __cplusplus
}
#endif

#endif // _HAL_SENSOR_H
