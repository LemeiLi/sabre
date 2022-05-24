/*************************************************************************
	> Created Time: Thu 18 Jul 2019 12:09:57 PM CST
 ************************************************************************/

#ifndef _NRB_DESCRIPTOR_H 
#define _NRB_DESCRIPTOR_H

#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include "consumer/common/base_object.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum nrb_id {
	NET_GPS_ID = 0,
	NET_IMU_ID,
	NET_ODO_ID, 
	//NET_VTS_ID,
	NET_IMAGE_ID,
	NET_ID_NUM,
}nrb_id_t;

typedef struct nrb_desc {
	struct bobject bobj; 
	enum nrb_id id;
	bool fragement; ///< fragementation or not.
	size_t to_send;
	struct nrb_ops* ops;
	void* rfmap;     ///< reflaction mapping
}nrb_desc_t;

typedef struct nrb_ops {
	void (*construct_nrb)(void* pdata, size_t size);
	void (*destruct_nrb)(void** pthis);
}nrb_ops_t;

extern void construct_nrb(void* pdata, size_t size);
extern void destruct_nrb(void** pthis);

#ifdef __cplusplus
}
#endif

#endif // _NRB_DESCRIPTOR_H

