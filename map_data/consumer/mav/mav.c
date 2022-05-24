/*************************************************************************
	> File Name: mav.c
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
#include "consumer/mav/mav.h"
#include "utils/fifo.h"
#include "utils/utils.h"
#include "utils/ulist.h"
#include "net/nrb_image.h"
#include "net/net_tcp_server.h"

static struct mav* mav_obj;
static struct mav* setup_mav(int pipeline_amount);
static int subscribe_image_data(struct cam_data* cam);
static int destroy_mav(struct mav* this);

int setup_mav_service(void)		
{
	mav_obj = setup_mav(2);	

	return 0;
}

int shutdown_mav_service(void)
{
	destroy_mav(mav_obj);

	return 0;
}

void handle_map_data_mav(struct cam_data* cam, struct mcu_data* mcu)
{
	subscribe_image_data(cam);

	return;
}

static int subscribe_image_data(struct cam_data* cam)
{
	int k = 0;
	static int ii = 0;
	struct pipeline* ppline[mav_obj->pipeline_amount];
	struct pipeline* temp;

	list_for_each_entry(temp, &mav_obj->pplist, node) {
		ppline[k++] = temp;
	}
#if 0
	struct ntv_desc* pntv = construct_ntv_desc(ppline[ii%k]->prcc->ntv_cpool_ctner->img_cpool, cam->stream0.addr, cam->stream0.size, NTV_IMAGE_ID);
	pntv->timestamp = cam->timestamp;
#else 
	static uint8_t* pdata; static size_t size;
	read_jpg_file("./test.jpg", &pdata, &size);
	struct ntv_desc* pntv = construct_ntv_desc(ppline[ii%k]->prcc->ntv_cpool_ctner->img_cpool, 
		pdata, size, NTV_IMAGE_ID);
	pntv->timestamp = ii;
#endif

	feeding(ppline[ii%k]->pfeeder, pntv);

	fprintf(stderr, ">>>>>>>> feeding image to pipline total = %d\n", ++ii);

	return 0;
}

static struct mav* setup_mav(int pipeline_amount)
{
	struct addr_map addr_maps[2] = {{ .ip_addr = MAV_IP, .port = MAV_PORTA },
		                            { .ip_addr = MAV_IP, .port = MAV_PORTB }};
	struct mav* pthis = malloc(sizeof(struct mav));
	INIT_LIST_HEAD(&pthis->pplist);
	pthis->pipeline_amount = pipeline_amount;

	for (int i = 0; i < pipeline_amount; i++) {
		struct pipeline* plnew = create_pipeline(addr_maps[i].ip_addr, addr_maps[i].port);
		list_add(&plnew->node, &pthis->pplist);
	}

	return pthis;
}

static int destroy_mav(struct mav* pthis)
{
	struct pipeline* ppline;

	list_for_each_entry(ppline, &pthis->pplist, node) {
		destroy_pipeline(ppline);
	}
	free(pthis);

	return 0;
}






