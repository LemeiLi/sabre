/*************************************************************************
	> File Name: mav.h
	> Created Time: Wed 26 Jun 2019 10:35:29 AM CST
 ************************************************************************/
#ifndef _3GEN_COLLECTOR_H
#define _3GEN_COLLECTOR_H

#include "utils/ulist.h"
#include "consumer/common/cam_mcu.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAV_IP    "0.0.0.0"
#define MAV_PORTA "5678"
#define MAV_PORTB "6789"

/**
 * obstract a map collector vehicel
 */
typedef struct mav 
{
	int pipeline_amount;     ///< total pipeline in mav system 
	struct list_head pplist; ///< struct pipeline 
}mav_t;

/**
 * Construct a mav object and setup.
 * @return 0 success -1 error
 */
extern int setup_mav_service(void);

/**
 * Distruct a mav object and shutdown.
 * @return 0 success -1 error
 */
extern int shutdown_mav_service(void);

/**
 * The handle that accept the data by callback.
 */
extern void handle_map_data_mav(struct cam_data* cam, struct mcu_data* mcu);

#ifdef __cplusplus
}
#endif

#endif // _3GEN_COLLECTOR_H
