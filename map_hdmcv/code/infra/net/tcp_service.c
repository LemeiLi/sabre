/*************************************************************************
	> File Name: tcp_service.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 16 Oct 2019 04:32:04 PM CST
 ************************************************************************/
#include "infra/net/net.h"
#include "infra/net/tcp_service.h"
#include "infra/stream/stream.h"
#include "infra/log/log.h"
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <linux/sockios.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "uil/uil_image.h"


static ssize_t tcp_client_send_data(void* pthis, void* data, size_t size) 
{
	struct tcp_client* pthis_ = (struct tcp_client*)pthis;
	ssize_t ret = writen(pthis_->super.fd, data, size);

	return ret;
}

static ssize_t tcp_client_recv_data(void* pthis, void* buf, size_t size) 
{
	struct tcp_client* pthis_ = (struct tcp_client*)pthis;
	return readn(pthis_->super.fd, buf, size);
}

static net_state tcp_client_get_state(void* pthis)
{
	struct net* pthis_ = (struct net*)pthis;
	return pthis_->state;
}

static int32_t tcp_client_connect_server(void* pthis)
{
	struct sockaddr_in srv_addr;
	struct tcp_client* pthis_ = (struct tcp_client*)pthis;

	pthis_->super.state = NETS_DISCONNECTED;
	pthis_->super.fd = socket(AF_INET, SOCK_STREAM, 0);
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port   = htons(pthis_->super.port);  
 	if (inet_aton((const char *)pthis_->super.ip, &srv_addr.sin_addr) == 0) {
		log_error("invalid server_ip");
		return HAL_IMAGE_RET_ECAMCONF;
	}
	memset(&srv_addr.sin_zero, 0, sizeof(srv_addr.sin_zero));

	if (connect(pthis_->super.fd, (struct sockaddr *)(&srv_addr),
		        sizeof(struct sockaddr)) == -1) {
		log_error("connect server [%d %s:%d] error %s", 
				  pthis_->super.fd,
				  pthis_->super.ip, pthis_->super.port, strerror(errno));
		close(pthis_->super.fd);
		return HAL_IMAGE_RET_ECONREF;	
	} else {
		log_info("connect server channel [%d %s:%d] success.", 
				 pthis_->super.fd,
				 pthis_->super.ip, pthis_->super.port);
	}
	pthis_->super.state = NETS_CONNECTED;

	return HAL_IMAGE_RET_OK;
}

static struct tcp_client_vmt vmt = {
	.send_data = tcp_client_send_data,
	.recv_data = tcp_client_recv_data,
	.get_state = tcp_client_get_state,
	.connect_server = tcp_client_connect_server,
};

/**< A.1.1.3.1 */	
static int tcp_client_init(void* pthis, const uint8_t* ip, uint16_t port, bool io_mtplx)
{
	struct tcp_client* pthis_ = (struct tcp_client*)pthis;

	/**< A.1.1.3.1.1 no check ret */	
	net_init(&pthis_->super, &vmt, NULL, NULL, ip, port, io_mtplx);
	pthis_->vptr = &vmt;

	pthis_->send_data = pthis_->vptr->send_data;
	pthis_->recv_data = pthis_->vptr->recv_data;
	pthis_->get_state = pthis_->vptr->get_state;
	/**< use super class default realizatrion */
	pthis_->close = pthis_->super.close;        
	pthis_->get_ip = pthis_->super.get_ip;      
	pthis_->get_port = pthis_->super.get_port;  

	pthis_->connect_server = pthis_->vptr->connect_server;

	return HAL_IMAGE_RET_OK;
}

/**< A.1.1.3 */	
struct tcp_client* construct_tcp_client(const uint8_t *ip, uint16_t port, bool io_mtplx)
{
	struct tcp_client* pthis = (struct tcp_client*)malloc(sizeof(*pthis));
	if (!pthis)	return NULL;

	if (!ip) return NULL;
	if (!port) return NULL;

	/**< A.1.1.3.1 no check ret */	
	tcp_client_init(pthis, ip, port, io_mtplx);
	log_info("construt_tcp_client: <%p> [%s:%d]", pthis, ip, port);

	return pthis;
}

void destruct_tcp_client(struct tcp_client* pthis)
{
	log_info("destruct_tcp_client: <%p> [%s:%d]", 
			 pthis, pthis->get_ip(pthis), pthis->get_port(pthis));
	free(pthis);
}


