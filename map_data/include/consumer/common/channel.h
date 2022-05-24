/*************************************************************************
	> File Name: channel.h
	> Created Time: Mon 01 Jul 2019 05:38:58 PM CST
 ************************************************************************/

#ifndef _CHANNEL_H
#define _CHANNEL_H

#include "utils/fifo.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LANE_SIZE		5

#define fifo_ntv		data_lanes[0]
#define fifo_nrb		data_lanes[1]
#define fifo_rxd		data_lanes[2]
#define fifo_nrb_rcc	data_lanes[3]
#define fifo_ntv_rcc	data_lanes[4]

struct channel 
{
	struct fifo* data_lanes[LANE_SIZE];
}channel_t;

extern struct channel* create_data_channel(void);
extern int destroy_data_channel(struct channel* pthis);

#ifdef __cplusplus
}
#endif

#endif // _CHANNEL_H
