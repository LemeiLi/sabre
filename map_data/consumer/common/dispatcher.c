/*************************************************************************
	> File Name: dispatcher.c
	> Created Time: Tue 02 Jul 2019 11:05:44 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "consumer/common/dispatcher.h"
#include "consumer/common/channel.h"
#include "consumer/common/feeder.h"
#include "utils/utils.h"

struct dispatcher* create_dispatcher(struct feeder* resc, struct channel* dest)
{
	struct dispatcher* pthis = malloc(sizeof(struct dispatcher));
	pthis->resc = resc;
	pthis->dest = dest;

	return pthis;
}

int dispatching(struct dispatcher* pthis, void* material)
{
/*
	static int ii = 0;
	if (courier_num == 2) {
		if (ii++%2)	
			fifo_push(pthis->dest->fifo_ntv, material);
		else 
			fifo_push(pthis->dest->fifo_ntv, material);
	} else {
	}
*/
	fifo_push(pthis->dest->fifo_ntv, material);

	return 0;
}

int destroy_dispatcher(struct dispatcher* pthis)
{
	if (pthis) {
		free(pthis);
		log("dispatcher destroyed!\n");
	}

	return 0;
}
