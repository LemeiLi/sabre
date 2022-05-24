#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sched.h> 
#include <pthread.h>
                
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <linux/sockios.h>
#include <sys/epoll.h>
#include "net/net_tcp_server.h"
#include "utils/utils.h"

#define MAX_EVENTS 1
#define NET_PROTOCOL    SOCK_STREAM

struct liver* create_stream_liver(void)
{
	return get_liver_instance();		
}

int destroy_stream_liver(struct liver* pthis)
{
	if (pthis) {
		free(pthis);
		log("stream liver instance [%p] destroyed!\n", pthis);
	}

	return 0;
}

struct liver* get_liver_instance(void)
{
	//static struct liver* liver_singleton = NULL;
	struct liver* pthis = NULL;

	pthis = (struct liver*)malloc(sizeof(struct liver));
	if(pthis == NULL) {
		fprintf(stderr, "malloc liver error\n");
		return NULL;
	}
	memset(pthis, 0, sizeof(struct liver));

	pthread_mutex_init(&pthis->mutex, NULL);
	pthread_cond_init(&pthis->cond, NULL);
	log("construct stream liver instance [%p]!\n", pthis);

	return pthis;
}

int get_client_state(struct liver* lv)
{
	pthread_mutex_lock(&lv->mutex);
	int state = lv->net_state;
	pthread_mutex_unlock(&lv->mutex);

	return state;
}

void set_client_state(struct liver* lv, int state)
{
	pthread_mutex_lock(&lv->mutex);
	lv->net_state = state;
	pthread_mutex_unlock(&lv->mutex);

	return;
}

uint32_t get_client_fd(struct liver* lv)
{
	pthread_mutex_lock(&lv->mutex);
	uint32_t fd = lv->client_fd;
	pthread_mutex_unlock(&lv->mutex);

	return fd;
}

void set_client_fd(struct liver* lv, uint32_t fd)
{
	pthread_mutex_lock(&lv->mutex);
	lv->client_fd = fd;
	pthread_mutex_unlock(&lv->mutex);

	return;
}

void *live_server_thread(void* args);
struct server* create_server_thread(struct liver* pliver, const char* ip_addr, const char* port)
{
	struct server* pthis = malloc(sizeof(struct server));
	pthis->_ip = ip_addr;
	pthis->_port = port;
	pthis->pliver = pliver;
	pthread_create(&pthis->sthread, NULL, live_server_thread, pthis);

	return pthis;
}

int destroy_server(struct server* pthis)
{
	void* status;
	if (pthis) {
		pthread_join(pthis->sthread, &status);
		free(pthis);
		log("[%ld] %s\n", pthis->sthread,(char*)status);
	}

	return 0;
}

static int create_live_server(struct liver* mjpg_liver, const char* ip_addr, const char* port);
void *live_server_thread(void* args)
{
	struct server* pthis = (struct server*)args;
	create_live_server(pthis->pliver, pthis->_ip, pthis->_port);
	pthread_exit("liver_server_thread exit!");

	return NULL;
}

static int create_live_server(struct liver* mjpg_liver, const char* ip_addr, const char* port)
{
	int ret;
	struct epoll_event events[MAX_EVENTS];
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	pthread_attr_t attr1;
	pthread_attr_init(&attr1);
	set_thread_cpu_affinity(&attr1, 0);


	int sock_fd = socket(AF_INET, NET_PROTOCOL, 0);
	if (-1 == sock_fd) {
		fprintf(stderr, "socket error!\n");
		return -1;
	}
	mjpg_liver->server_fd = sock_fd;
		
	int on = 1;
	ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
	if (-1 == ret) {
		fprintf(stderr, "setsockopt error!\n");
		return -1;
	}
	struct timeval timeout = {1,0};
	setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

	socklen_t sbuf_len = 512*1024; ///< buffer size 20k --> 40k in tcp/ip protocal
	socklen_t slen = sizeof(socklen_t);
	setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, (void*)&sbuf_len, slen);
	getsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, (void*)&sbuf_len, &slen);
	fprintf(stdout, "set socket buffer length to %d\n", sbuf_len);

	int keepalive = 1;    ///< 开启keepalive属性
	int keepidle = 1;     ///< 如该连接在1秒内没有任何数据往来,则进行探测
	int keepinterval = 1; ///< 探测时发包的时间间隔为1秒
	int keepcount = 3;    ///< 探测尝试的次数。如果第1次探测包就收到响应了,则后2次的不再发。
	setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
	setsockopt(sock_fd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepidle , sizeof(keepidle ));
	setsockopt(sock_fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
	setsockopt(sock_fd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));

	server_addr.sin_family      = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip_addr);
	server_addr.sin_port = htons(atoi(port));
	memset(server_addr.sin_zero, 0, 8);

	ret = bind(sock_fd, (const struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	if (-1 == ret) {           
		fprintf(stderr, "bind error!\n");
		return -1;
	}           

	if (listen(sock_fd, 10) == -1) {   
		fprintf(stderr,"Listen error:%s\n\a", strerror(errno));
		return -1;
	}

	set_non_blocking(sock_fd);

	int ep_fd = epoll_create(MAX_EVENTS);
	if (ep_fd <= 0) {
		fprintf(stderr, "create epoll failed!\n");
		return -1;
	}
	update_events(ep_fd, sock_fd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD) ;

	int client_fd = 0;
	while (!get_ctrl_c()) {
		fprintf(stdout, "channel [%s %s] wait for event......\n", ip_addr, port);
		int ret = epoll_wait(ep_fd, events, MAX_EVENTS, 2000); ///< ms
		if (!ret) { ///< timeout
			continue;
		}
		
		if (events[0].data.fd == sock_fd) {
			socklen_t len = sizeof(struct sockaddr_in);
			client_fd = accept(sock_fd, (struct sockaddr *)(&client_addr), &len);
			if (client_fd == -1) {
				fprintf(stderr, "Accept error:%s\n\a", strerror(errno));
				return -1;
			}
			if (!get_client_state(mjpg_liver)) {
				set_client_fd(mjpg_liver, client_fd);
				set_client_state(mjpg_liver, 1);
				fprintf(stdout, "channel [%s %s] connected.\n", ip_addr, port);

				//set_non_blocking(client_fd);
				update_events(ep_fd, client_fd, EPOLLERR | EPOLLHUP, EPOLL_CTL_ADD);
			}
		} else if ((events[0].events & EPOLLHUP) || (events[0].events & EPOLLERR)) {
			fprintf(stdout, "channel [%s %s] disconnected.\n", ip_addr, port);
			if(get_client_state(mjpg_liver)) {
				set_client_state(mjpg_liver, 0);
				close(get_client_fd(mjpg_liver));
			}
			update_events(ep_fd, sock_fd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);
		}
	}
	close(ep_fd);
	close(sock_fd);

    return 0;
}

int destroy_liver(struct liver* pthis)
{
	if (pthis)
		free(pthis);

	return 0;
}

void update_events(int efd, int fd, int events, int op) 
{
    struct epoll_event ev;

    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.fd = fd;

/*
	uint8_t in_flag = 0;
	uint8_t out_flag = 0;
	if(ev.events & EPOLLIN) {
		in_flag = 1;	
	}
	if(ev.events & EPOLLOUT) {
		out_flag = 1;	
	}
	log("%s %s[%d], events read [%s] write [%s]\n", 
			op == EPOLL_CTL_MOD ? "mod" : (op == EPOLL_CTL_ADD ? "add" : "del"), 
			fd == STDIN_FILENO ? "STDIN_FILENO" : "socketfd", 
			fd,
			in_flag ? "SET" : "UNSET", 
			out_flag ? "SET" : "UNSET");
*/
    int ret = epoll_ctl(efd, op, fd, &ev);
    if (ret == -1 && errno == EEXIST) {
		return;
	} else if (ret == -1){
		error("epoll_ctl falied");
	}
}











