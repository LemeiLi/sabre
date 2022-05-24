/*************************************************************************
	> File Name: net_server.h
	> Created Time: Tue 09 Apr 2019 03:11:51 PM CST
 ************************************************************************/

#ifndef _NET_MAIN_H
#define _NET_MAIN_H

#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * obstract ip-port pair 
 */
typedef struct addr_map 
{
	char* ip_addr;
	char* port;
}addr_map_t;


typedef struct liver {
	volatile uint32_t server_fd;
	volatile uint32_t client_fd;
	volatile uint32_t net_state;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	void *priv;
}liver_t;

struct server 
{
	pthread_t sthread;
	const char* _ip; 
	const char* _port;
	struct liver* pliver;
}server_t;

extern struct liver* create_stream_liver(void);
extern int destroy_stream_liver(struct liver* pthis);
extern struct server* create_server_thread(struct liver* pliver, 
		                                   const char* ip_addr, 
										   const char* port);
extern int destroy_server(struct server* this);
extern struct liver* get_liver_instance(void);
extern int get_client_state(struct liver* lv);
extern void set_client_state(struct liver* lv, int state);
extern uint32_t get_client_fd(struct liver* lv);
extern void set_client_fd(struct liver* lv, uint32_t fd);
extern void update_events(int efd, int fd, int events, int op);

#ifdef __cplusplus
}
#endif


#endif // _NET_MAIN_H


