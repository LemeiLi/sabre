/*************************************************************************
	> File Name: flc.h
	> Created Time: Wed 26 Jun 2019 10:35:29 AM CST
 ************************************************************************/
#ifndef _FLC_H
#define _FLC_H

#include "utils/ulist.h"
#include "consumer/common/cam_mcu.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FLC_IP    "0.0.0.0" ///< INADDR_ANY
#define FLC_PORTA "5678"
#define FLC_PORTB "6789"


/**
 * obstract a map collector vehicel
 */
typedef struct flc 
{
	int pipeline_amount;     ///< total pipeline in flc system 
	struct list_head pplist; ///< struct pipeline 
}flc_t;

/**
 * Construct a flc object and setup.
 * @return 0 success -1 error
 */
extern int setup_flc_service(void);

/**
 * Distruct a flc object and shutdown.
 * @return 0 success -1 error
 */
extern int shutdown_flc_service(void);

/**
 * The handle that accept the data by callback.
 */
extern void handle_map_data_flc(struct cam_data* cam, struct mcu_data* mcu);

#ifdef __cplusplus
}
#endif

#endif // _FLC_H
