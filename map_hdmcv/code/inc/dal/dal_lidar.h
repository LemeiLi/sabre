#ifndef _DAL_LIDAR_H
#define _DAL_LIDAR_H

#include "dal_sensors.h"

#ifdef __cplusplus
extern "C" {
#endif

enum return_type {
	STRONGEST,
	LAST,
	DUAL,
};

#define _base_lidar_methods_clone \
	int (*set_sensitivity)(void* instance, float sensitivities[]); \
	int (*reset_sensitivity)(void* instance); \
	int (*laser_on)(void* pthis); \
	int (*laser_off)(void* pthis); \
	int (*set_return_type)(void* pthis, enum return_type); \
	int (*get_return_type)(void* pthis); \
	int (*set_motor_rpm)(void* pthis, uint16_t rpm); \
	uint16_t (*get_motor_rpm)(void* pthis); \
	char* (*get_sensor_model)(void* pthis); \
	char* (*get_serial_num)(void* pthis); \
	char* (*get_mac_addr)(void* pthis); \
	char* (*get_factory_mac_addr)(void* pthis); \
	int (*set_fov)(void* pthis, uint16_t start, uint16_t end); \
	int (*pps_require_gps_recv_valid)(void* pthis, bool req); \
	int (*pps_require_pps_lock)(void* pthis, bool req); \
	int (*set_pps_delay)(void* pthis, uint16_t delay); \
	int (*gps_require_gps_recv_valid)(void* pthis, bool req); \
	int (*require_phase_lock)(void* pthis, bool req); \
	int (*set_phase_lock_offset)(void* pthis, uint16_t offset); \
	int (*set_dest_ip)(void* pthis, const char* ip); \
	int (*set_dest_data_port)(void* pthis, uint16_t port); \
	int (*set_dest_tele_port)(void* pthis, uint16_t port); \
	int (*set_sensor_dhcp)(void* pthis, bool req); \
	int (*set_sensor_ip)(void* pthis, const char* ip); \
	int (*set_sensor_mask)(void* pthis, const char* mask); \
	int (*set_sensor_gateway)(void* pthis, const char* gw); \
	int (*set_sensor_mac)(void* pthis, const char* mac); \

#define _base_lidar_methods \
	_base_sensor_methods \
	_base_lidar_methods_clone

struct BaseLidarVMT {
	_base_lidar_methods
};

#define _base_lidar_data \
	_base_sensor_data

typedef struct BaseLidar {
	BaseSensor super;
	struct BaseLidarVMT* vmt;
	_base_lidar_methods 
	_base_lidar_data
} BaseLidar;

extern void BaseLidarInit(void* pthis, void* vmt);

#ifdef __cplusplus
}
#endif

#endif // _DAL_LIDAR_H
