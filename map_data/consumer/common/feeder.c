/*************************************************************************
	> File Name: feeder.c
	> Created Time: Tue 02 Jul 2019 10:03:42 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "consumer/common/dispatcher.h"
#include "consumer/common/feeder.h"
#include "utils/fifo.h"
#include "utils/utils.h"

struct feeder* create_feeder(void)
{
	struct feeder* pthis = malloc(sizeof(struct feeder));

	return pthis;
}

///< dependency injection
inline void di_dispatcher2feeder(struct feeder* pthis, struct dispatcher* pthat)
{
	pthis->pdsp = pthat;	

	return;
}
	
int feeding(struct feeder* pthis, void* material)
{
	dispatching(pthis->pdsp, material);
	return 0;
}

int destroy_feeder(struct feeder* pthis)
{
	if (pthis) {
		free(pthis);
		log("feeder destroyed!\n");
	}

	return 0;
}
