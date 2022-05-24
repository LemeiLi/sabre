#ifndef _HAL_INSGPS_H
#define _HAL_INSGPS_H

#include <stdbool.h>
#include <stdint.h>
#include "dal_sensors.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _base_insgps_methods_clone \
	int (*set_sensitivity)(void* instance, float sensitivities[]); \
	int (*reset_sensitivity)(void* instance); \
	int (*write_rtk)(void* pthis, char* data, size_t size); \

#define _base_insgps_methods \
	_base_sensor_methods \
	_base_insgps_methods_clone

struct BaseINSGPSVMT {
	_base_insgps_methods
};

#define _base_insgps_data \
	_base_sensor_data \

typedef struct {
	BaseSensor super;
	struct BaseINSGPSVMT* vmt;
	_base_insgps_methods 
	_base_insgps_data
} BaseINSGPS;

extern void BaseINSGPSInit(void* pthis, void* vmt);

#ifdef __cplusplus
}
#endif

#endif // _HAL_INSGPS_H
