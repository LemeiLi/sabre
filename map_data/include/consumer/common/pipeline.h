/*************************************************************************
	> File Name: pipeline.h
	> Created Time: Mon 01 Jul 2019 06:01:57 PM CST
 ************************************************************************/

#ifndef _PIPELINE_H
#define _PIPELINE_H
#include "transmitter.h"
#include "fabricator.h"
#include "channel.h"
#include "feeder.h"
#include "dispatcher.h"
#include "utils/ulist.h"
#include "recycler.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pipeline
{
	struct server* pserver;
	struct liver* pliver;	
	struct transmitter* ptx;	
	struct fabricator* pfabr;
	struct channel* data_ch;
	struct feeder* pfeeder;
	struct dispatcher* pdsp; 
	struct recycler* prcc;
	struct list_head node;
}pipeline_t;

extern struct pipeline* create_pipeline(const char* ip, const char* port);
extern int destroy_pipeline(struct pipeline* this);


#ifdef __cplusplus
}
#endif

#endif // _PIPELINE_H
