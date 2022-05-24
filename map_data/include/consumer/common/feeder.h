/*************************************************************************
	> File Name: feeder.h
	> Created Time: Tue 02 Jul 2019 10:04:13 AM CST
 ************************************************************************/

#ifndef _FEEDER_H
#define _FEEDER_H

#include "dispatcher.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct feeder {
	struct dispatcher* pdsp;	
}feeder_t;

extern struct feeder* create_feeder(void);
extern void di_dispatcher2feeder(struct feeder* pthis, struct dispatcher* pthat);
extern int feeding(struct feeder* pthis, void* material);
extern int destroy_feeder(struct feeder* pthis);

#ifdef __cplusplus
}
#endif

#endif // _FEEDER_H
