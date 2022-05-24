/*************************************************************************
	> Created Time: Sat 27 Jul 2019 03:47:47 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "consumer/common/recycler.h"
#include "net/nrb_imu.h"
#include "net/nrb_gps.h"
#include "net/nrb_odo.h"
#include "net/nrb_image.h"
#include "consumer/common/ntv_desc.h"
#include "utils/utils.h"
#include "utils/fifo.h"


static void* recycler_thread_nrb(void *args);
static void* recycler_thread_ntv(void *args);
struct recycler* create_recycler_thread(struct channel* pch)
{
	struct recycler* pthis = malloc(sizeof(struct recycler));
	pthis->monitor_ch = pch;
	pthis->ntv_cpool_ctner = construct_cpool_container();
	pthis->nrb_cpool_ctner = construct_cpool_container();

	pthread_create(&pthis->nrb_rthread, NULL, recycler_thread_nrb, pthis);
	pthread_create(&pthis->ntv_rthread, NULL, recycler_thread_ntv, pthis);

	return pthis;
}

void destroy_recycler(struct recycler** ppthis)
{
	void* status;
	if (*ppthis) {
		pthread_join((*ppthis)->nrb_rthread, &status);
		log("[%ld] %s\n", (*ppthis)->nrb_rthread, (char*)status);
		pthread_join((*ppthis)->ntv_rthread, &status);
		log("[%ld] %s\n", (*ppthis)->ntv_rthread, (char*)status);

		destruct_cpool_container(&(*ppthis)->ntv_cpool_ctner, 0);
		destruct_cpool_container(&(*ppthis)->nrb_cpool_ctner, 1);
		
		free(*ppthis);
		*ppthis = NULL;
	}

	return ;
}

static void* recycler_thread_nrb(void *args)
{
	struct recycler* pthis = (struct recycler*)args;
	while (!get_ctrl_c()) {
		
		//while (get_nr_active(pthis->monitor_ch->fifo_nrb_rcc)) { ///< make sure all object be recovered!
			struct nrb_desc* pnrb = fifo_pop(pthis->monitor_ch->fifo_nrb_rcc, 2);
			if (pnrb == NULL) {
				continue;
			}

			struct cache_pool_container* nrbc_ctner = pthis->nrb_cpool_ctner;
			switch(pnrb->id) {
				case NET_GPS_ID:
					recycle_nrb_gps(nrbc_ctner->gps_cpool, (struct nrb_gps_desc*)pnrb);
					break;
				case NET_IMU_ID:
					recycle_nrb_imu(nrbc_ctner->imu_cpool, (struct nrb_imu_desc*)pnrb);
					break;
				case NET_ODO_ID:
					recycle_nrb_odo(nrbc_ctner->odo_cpool, (struct nrb_odo_desc*)pnrb);
					break;
				case NET_IMAGE_ID:
					recycle_nrb_image(nrbc_ctner->img_cpool, (struct nrb_image_desc*)pnrb);
					break;
				default:
					break;
			}
		//}
	}
	pthread_exit("recycler_thread_nrb exit!");

	return NULL;
}

static void* recycler_thread_ntv(void *args)
{
	struct recycler* pthis = (struct recycler*)args;
	struct cache_pool_container* ntvc_ctner = pthis->ntv_cpool_ctner;
	while (!get_ctrl_c()) {
			//uint32_t nr = get_nr_active(pthis->monitor_ch->fifo_ntv_rcc); ///< make sure all object be recovered!
			struct ntv_desc* pntv = fifo_pop(pthis->monitor_ch->fifo_ntv_rcc, 2);
			if (pntv == NULL) {
				continue;
			}
			//log("nr = %d, ntv rcc id = %d, addr = %p\n", nr, pntv->id, pntv);

			switch(pntv->id) {
				case NTV_GPS_ID:
					recycle_ntv_desc(ntvc_ctner->gps_cpool, pntv);
					break;
				case NTV_IMU_ID:
					recycle_ntv_desc(ntvc_ctner->imu_cpool, pntv);
					break;
				case NTV_ODO_ID:
					recycle_ntv_desc(ntvc_ctner->odo_cpool, pntv);
					break;
				case NTV_IMAGE_ID:
					recycle_ntv_desc(ntvc_ctner->img_cpool, pntv);
					break;
				default:
					break;
			}
	}
	pthread_exit("recycler_thread_ntv exit!");

	return NULL;
}




