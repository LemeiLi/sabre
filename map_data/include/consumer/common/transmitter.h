/*************************************************************************
	> File Name: transmitter.h
	> Created Time: Mon 01 Jul 2019 07:18:47 PM CST
 ************************************************************************/

#ifndef _TRANSMITTER_H
#define _TRANSMITTER_H

#include "channel.h"
#include "net/net_tcp_server.h"

#ifdef __cplusplus
extern "C" {
#endif

struct transmitter 
{
	pthread_t tthread;
	struct liver* pliver;
	struct channel* data_ch;
}transmitter_t;

extern struct transmitter* create_transmitter_thread(struct liver* pliver, struct channel* pch);
extern int destroy_transmitter(struct transmitter* pthis);

#ifdef __cplusplus
}
#endif


#endif // _TRANSMITTER_H
