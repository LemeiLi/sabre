/*************************************************************************
	> File Name: dispatcher.h
	> Created Time: Tue 02 Jul 2019 10:52:55 AM CST
 ************************************************************************/

#ifndef _DISPATCHER_H
#define _DISPATCHER_H

#include "feeder.h"
#include "channel.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dispatcher 
{
	struct feeder* resc;
	struct channel* dest; 
}dispatcher_t;

extern struct dispatcher* create_dispatcher(struct feeder* resc, struct channel* dest);
extern int dispatching(struct dispatcher* pthis, void* material);
extern int destroy_dispatcher(struct dispatcher* pthis);

#ifdef __cplusplus
}
#endif

#endif // _DISPATCHER_H
