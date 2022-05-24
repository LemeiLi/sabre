#ifndef _HAL_IMAGE_H
#define _HAL_IMAGE_H

#include "dal_sensors.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _base_image_methods_clone \
	int (*set_sensitivity)(void* instance, float sensitivities[]); \
	int (*reset_sensitivity)(void* instance); \

#define _base_image_methods \
	_base_sensor_methods \
	_base_image_methods_clone

struct BaseImageVMT {
	_base_image_methods
};

#define _base_image_data \
	_base_sensor_data

typedef struct {
	BaseSensor super;
	struct BaseImageVMT* vmt;
	_base_image_methods 
	_base_image_data
} BaseImage;

extern void BaseImageInit(void* pthis, void* vmt);

#ifdef __cplusplus
}
#endif

#endif // _HAL_IMAGE_H
