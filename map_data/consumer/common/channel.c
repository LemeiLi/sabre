/*************************************************************************
	> File Name: channel.c
	> Created Time: Mon 01 Jul 2019 05:41:55 PM CST
 ************************************************************************/

#include "consumer/common/channel.h"
#include <stdlib.h>
#include "utils/utils.h"

struct channel* create_data_channel(void)
{
	struct channel* pthis = malloc(sizeof(struct channel));
	for (int i = 0; i < LANE_SIZE; i++)
		pthis->data_lanes[i] = fifo_new();

	return pthis;
}

static void clean_all_data_lanes(struct channel* pthis)
{
	for (int i = 0; i < LANE_SIZE; i++) {
		while(!fifo_is_empty(pthis->data_lanes[i])) {
			void* pdata = fifo_pop(pthis->data_lanes[i], 1);
			free(pdata);
		}
	}
}

int destroy_data_channel(struct channel* pthis)
{
	if (pthis != NULL) {
		clean_all_data_lanes(pthis);
		for (int i = 0; i < LANE_SIZE; i++) {
			fifo_free(pthis->data_lanes[i]);
		}
		free(pthis);
		log("destroy data channel!\n");
	}

	return 0;
}

