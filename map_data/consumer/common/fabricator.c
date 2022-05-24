/*************************************************************************
	> File Name: fabricator.c
	> Created Time: Mon 01 Jul 2019 07:11:15 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "consumer/common/fabricator.h"
#include "consumer/common/channel.h"
#include "consumer/common/recycler.h"
#include "consumer/common/ntv_desc.h"
#include "net/net_tcp_server.h"
#include "net/nrb_desc.h"
#include "net/nrb_imu.h"
#include "net/nrb_gps.h"
#include "net/nrb_odo.h"
#include "net/nrb_image.h"
#include "utils/utils.h"

static void* fabricate_thread(void* args);
struct fabricator* create_fabricator_thread(struct liver* pliver, struct channel* pch)
{
	struct fabricator* pthis = malloc(sizeof(struct fabricator));
	pthis->pliver = pliver;
	pthis->data_ch = pch;
	pthread_create(&pthis->fthread, NULL, fabricate_thread, pthis);

	return pthis;
}

int destroy_fabricator(struct fabricator* pthis)
{
	void* status;
	pthread_join(pthis->fthread, &status);
	log("[%ld] %s\n", pthis->fthread, (char*)status);
	free(pthis);

	return 0;
}

///< dependency injection!
inline void di_recycler2fabricator(struct fabricator* pthis, struct recycler* pthat)
{
	pthis->prcc = pthat;	
	return;
}

static void fabricate_image(struct fabricator* pthis, struct ntv_desc* pntv)
{
	static uint32_t total = 0;
	struct net_image_desc* pimg = &(struct net_image_desc) {
		.image = pntv->data,
		.size = pntv->size,
		.stamp = pntv->timestamp,
	};
	construct_nrb_image(pthis->prcc->nrb_cpool_ctner->img_cpool, 
		pimg, submit_nrb_image, pthis->data_ch->fifo_nrb);
	log("fabricator submit total images = %d, drop = %d\n", ++total, *((uint32_t*)pntv->private));

	return;

}

static void fabricate_imu(struct fabricator* pthis, struct ntv_desc* pntv)
{
	struct nrb_imu_desc* pimu = construct_nrb_imu(pthis->prcc->nrb_cpool_ctner->imu_cpool, 
		pntv->data, pntv->size);
	submit_nrb_imu(pthis->data_ch->fifo_nrb, pimu);

	return;
}

static void fabricate_gps(struct fabricator* pthis, struct ntv_desc* pntv)
{
	struct nrb_gps_desc* pgps = construct_nrb_gps(pthis->prcc->nrb_cpool_ctner->gps_cpool, 
		pntv->data, pntv->size);
	submit_nrb_gps(pthis->data_ch->fifo_nrb, pgps);

	return;
}

static void fabricate_odo(struct fabricator* pthis, struct ntv_desc* pntv)
{
	struct nrb_odo_desc* podo = construct_nrb_odo(pthis->prcc->nrb_cpool_ctner->odo_cpool, 
		pntv->data, pntv->size);
	submit_nrb_odo(pthis->data_ch->fifo_nrb, podo);

	return;
}

static void* fabricate_thread(void* args)
{
	static uint32_t total_drop;
	struct fabricator* pthis = (struct fabricator*)args;
	while (!get_ctrl_c()) {
		struct ntv_desc* pntv = (struct ntv_desc*)fifo_pop(pthis->data_ch->fifo_ntv, 2);
		if (pntv == NULL) {
			continue;
		}
		//log("nr = %d, ntv id = %d, addr = %p\n", nr, pntv->id, pntv);
		if (!get_client_state(pthis->pliver)) {
			goto recycle;
		}

		uint32_t nr = get_nr_active(pthis->data_ch->fifo_nrb);
		if (unlikely(nr > FIFO_THRESHOLD)) {
			//log("warning >>>>>>>> data blocks in nrb fifo = %d\n", nr);
			total_drop++;
			goto recycle;
		}
	
		switch(pntv->id) {
			case NTV_GPS_ID:
				fabricate_gps(pthis, pntv);
				break;
			case NTV_IMU_ID:
				fabricate_imu(pthis, pntv);
				break;
			case NTV_ODO_ID:
				fabricate_odo(pthis, pntv);
				break;
			case NTV_IMAGE_ID:
				pntv->private = &total_drop;
				fabricate_image(pthis, pntv);
				break;
			default:
				fprintf(stderr, ">>>>>>>> unknow native data id: %d!\n", pntv->id);
		} 
	recycle:
		fifo_push(pthis->data_ch->fifo_ntv_rcc, pntv);
	}
	pthread_exit("fabricate_thread exit!");

	return NULL;
}





