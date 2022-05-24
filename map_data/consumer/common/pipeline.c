/*************************************************************************
	> File Name: pipeline.c
	> Created Time: Mon 01 Jul 2019 06:04:16 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "consumer/common/pipeline.h"
#include "consumer/common/channel.h"
#include "consumer/common/fabricator.h"
#include "consumer/common/transmitter.h"
#include "consumer/common/dispatcher.h"
#include "consumer/common/feeder.h"
#include "consumer/common/recycler.h"
#include "net/net_tcp_server.h"

struct pipeline* create_pipeline(const char* ip, const char* port)
{
	struct pipeline* pthis = malloc(sizeof(struct pipeline));

	pthis->pliver = create_stream_liver();
	pthis->data_ch = create_data_channel();
	pthis->pserver = create_server_thread(pthis->pliver, ip, port);
	pthis->pfabr = create_fabricator_thread(pthis->pliver, pthis->data_ch);
	pthis->ptx = create_transmitter_thread(pthis->pliver, pthis->data_ch);
	pthis->prcc = create_recycler_thread(pthis->data_ch);
	di_recycler2fabricator(pthis->pfabr, pthis->prcc);

	pthis->pfeeder = create_feeder();
	pthis->pdsp = create_dispatcher(pthis->pfeeder, pthis->data_ch);
	di_dispatcher2feeder(pthis->pfeeder, pthis->pdsp);

	return pthis;
}

int destroy_pipeline(struct pipeline* pthis)
{
	sleep(2);
	destroy_feeder(pthis->pfeeder);
	destroy_dispatcher(pthis->pdsp);
	destroy_fabricator(pthis->pfabr);
	destroy_transmitter(pthis->ptx);
	destroy_server(pthis->pserver);
	destroy_stream_liver(pthis->pliver);
	destroy_recycler(&pthis->prcc);
	destroy_data_channel(pthis->data_ch); 

	free(pthis);

	return 0;
}





