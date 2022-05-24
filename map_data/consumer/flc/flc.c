/*************************************************************************
	> File Name: flc.c
	> Created Time: Wed 26 Jun 2019 10:30:06 AM CST
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "consumer/common/feeder.h"
#include "consumer/common/pipeline.h"
#include "consumer/common/ntv_desc.h"
#include "consumer/common/persistence.h"
#include "utils/fifo.h"
#include "utils/utils.h"
#include "utils/ulist.h"
#include "consumer/flc/flc.h"
#include "net/net_tcp_server.h"


static struct flc* flc_obj;
static struct flc* setup_flc(int pipeline_amount);
static int destroy_flc(struct flc* pthis);
static int subscribe_image_data(struct cam_data* new);
static int subscribe_mcu_data(struct mcu_data* mcu);

int setup_flc_service(void)		
{
	flc_obj = setup_flc(1);	

	return 0;
}

int shutdown_flc_service(void)
{
	destroy_flc(flc_obj);

	return 0;
}
///< TODO
void handle_map_data_flc(struct cam_data* cam, struct mcu_data* mcu)
{
	subscribe_image_data(cam);
	subscribe_mcu_data(mcu);
	return;
}

static int subscribe_image_data(struct cam_data* cam)
{
	int k = 0;
	static int ii = 0;
	struct pipeline* ppline[flc_obj->pipeline_amount];
	struct pipeline* temp;

	list_for_each_entry(temp, &flc_obj->pplist, node) {
		ppline[k++] = temp;
	}
#if 0
	struct ntv_desc* pntv = construct_ntv_desc(ppline[ii%k]->prcc->ntv_cpool_ctner->img_cpool, 
		cam->stream1.addr, cam->stream1.size, NTV_IMAGE_ID);
	pntv->timestamp = cam->timestamp;
#else  
	static uint8_t* pdata; static size_t size;
	read_jpg_file("./test.jpg", &pdata, &size);
	struct ntv_desc* pntv = construct_ntv_desc(ppline[ii%k]->prcc->ntv_cpool_ctner->img_cpool, 
		pdata, size, NTV_IMAGE_ID);
	pntv->timestamp = ii;
#endif

	feeding(ppline[ii%k]->pfeeder, pntv);
	fprintf(stderr, ">>>>>>>> feeding image data to pipline total = %d\n", ii++);

	return 0;
}

static int subscribe_mcu_data(struct mcu_data* mcu)
{
	int k = 0;
	static uint32_t ii = 0; 
	//static uint32_t imu_counter = 0, gps_counter = 0, odo_counter = 0;
	struct pipeline* ppline[flc_obj->pipeline_amount];
	struct pipeline* temp;

	list_for_each_entry(temp, &flc_obj->pplist, node) {
		ppline[k++] = temp;
	}
	struct cache_pool_container* ctner = ppline[ii++%k]->prcc->ntv_cpool_ctner;
#if 1
	for(int i = 0; i < mcu->imu_count; i++) {
		struct ntv_desc* pntv = construct_ntv_desc(ctner->imu_cpool, &mcu->imu[i], sizeof(struct mcu_imu), NTV_IMU_ID);
		feeding(ppline[ii%k]->pfeeder, pntv);
		//fprintf(stderr, ">>>>>>>> feeding imu data to pipline total = %d\n", ++imu_counter);
	}
#endif

	for(int i = 0; i < mcu->gps_count; i++) {
		struct ntv_desc* pntv = construct_ntv_desc(ctner->gps_cpool, &mcu->gps[i], sizeof(struct mcu_gps), NTV_GPS_ID);
		feeding(ppline[ii%k]->pfeeder, pntv);
		//fprintf(stderr, ">>>>>>>> feeding gps data to pipline total = %d\n", ++gps_counter);
	}
#if 1
	for (int i = 0; i < mcu->odo_count; i++) {
		struct ntv_desc* pntv = construct_ntv_desc(ctner->odo_cpool, &mcu->odo[i], sizeof(struct mcu_odo), NTV_ODO_ID);
		feeding(ppline[ii%k]->pfeeder, pntv);
		//fprintf(stderr, ">>>>>>>> feeding odo data to pipline total = %d\n", ++odo_counter);
	}
#endif
	++ii;
	//fprintf(stderr, ">>>>>>>> feeding mcu data to pipline total = %d\n", ii);

	return 0;		
}

static struct flc* setup_flc(int pipeline_amount)
{
	struct addr_map addr_maps[2] = {{ .ip_addr = FLC_IP, .port = FLC_PORTA },
		                            { .ip_addr = FLC_IP, .port = FLC_PORTB }};
	struct flc* pthis = malloc(sizeof(struct flc));
	INIT_LIST_HEAD(&pthis->pplist);
	pthis->pipeline_amount = pipeline_amount;

	for (int i = 0; i < pipeline_amount; i++) {
		struct pipeline* plnew = create_pipeline(addr_maps[i].ip_addr, addr_maps[i].port);
		list_add(&plnew->node, &pthis->pplist);
	}

	return pthis;
}

static int destroy_flc(struct flc* pthis)
{
	struct pipeline* ppline = NULL;
    if (pthis) {
		if (!list_empty(&pthis->pplist)) {
			list_for_each_entry(ppline, &pthis->pplist, node) {
				destroy_pipeline(ppline);
			}
		}
		free(pthis);
	}

	return 0;
}









