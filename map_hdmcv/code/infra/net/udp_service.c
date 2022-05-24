/*************************************************************************
	> File Name: udp_service.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 21 Nov 2019 03:23:21 PM CST
 ************************************************************************/

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
#include <netinet/udp.h>
#include <linux/sockios.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "infra/net/net.h"
#include "infra/net/udp_service.h"
#include "infra/stream/stream.h"
#include "infra/log/log.h"
#include "infra/utils/exception.h"

static ssize_t udp_server_send_data(void* pthis, void* data, size_t size) 
{
	ssize_t ret;
	struct udp_server* pthis_ = (struct udp_server*)pthis;
	ret = sendto(pthis_->super.fd, data, size, MSG_CONFIRM, 
		   (const struct sockaddr *)&pthis_->clt_addr,  
		   sizeof(pthis_->clt_addr));

	return ret;
}

static ssize_t  udp_server_recv_data(void* pthis, void* buf, size_t size) 
{
	ssize_t ret;
	struct udp_server* pthis_ = (struct udp_server*)pthis;
	socklen_t slen = sizeof(pthis_->clt_addr);
	ret = recvfrom(pthis_->super.fd, buf, size, MSG_WAITALL, 
			       (struct sockaddr *)&pthis_->clt_addr, &slen);
	//log_info("udp_server: recv [%s:%d]", inet_ntoa(pthis_->clt_addr.sin_addr), ntohs(pthis_->clt_addr.sin_port));

	return ret;
}

static net_state udp_server_get_state(void* pthis)
{
	struct net* pthis_ = (struct net*)pthis;
	return pthis_->state;
}

static struct udp_server_vmt vmt = {
	.send_data = udp_server_send_data,
	.recv_data = udp_server_recv_data,
	.get_state = udp_server_get_state,
};

/**< A.1.1.3.1 */	
static int udp_server_init(void* pthis, const uint8_t* ip, uint16_t port, bool io_mtplx)
{
	struct udp_server* pthis_ = (struct udp_server*)pthis;

	/**< A.1.1.3.1.1 no check ret */	
	net_init(&pthis_->super, &vmt, NULL, NULL, ip, port, io_mtplx);
	pthis_->vptr = &vmt;

	pthis_->super.state = NETS_CONNECTED;

	pthis_->super.fd = socket(AF_INET, SOCK_DGRAM, 0);
	pthis_->srv_addr.sin_family = AF_INET;
	pthis_->srv_addr.sin_port = htons(pthis_->super.port);  

 	if (inet_aton((const char *)pthis_->super.ip, &pthis_->srv_addr.sin_addr) == 0) {
		log_error("invalid server_ip: %s", strerror(errno));
		return HAL_EUDP;
	}
	memset(&pthis_->srv_addr.sin_zero, 0, sizeof(pthis_->srv_addr.sin_zero));
	if (bind(pthis_->super.fd, (struct sockaddr *)&pthis_->srv_addr, 
			 sizeof(pthis_->srv_addr))) {
		log_error("bind udp server error: %s", strerror(errno));
		return HAL_EUDP;
	}

	pthis_->send_data = pthis_->vptr->send_data;
	pthis_->recv_data = pthis_->vptr->recv_data;
	pthis_->get_state = pthis_->vptr->get_state;

	/**< use super class default realization */
	pthis_->close = pthis_->super.close;        
	pthis_->get_ip = pthis_->super.get_ip;      
	pthis_->get_port = pthis_->super.get_port;  

	pthis_->connect_server = pthis_->vptr->connect_server;

	return HAL_OK;
}

/**< A.1.1.3 */	
struct udp_server* construct_udp_server(const uint8_t *ip, uint16_t port, bool io_mtplx)
{
	struct udp_server* pthis = (struct udp_server*)malloc(sizeof(*pthis));
	if (!pthis)	return NULL;
	if (!ip) return NULL;
	if (!port) return NULL;

	/**< A.1.1.3.1 no check ret */	
	if (udp_server_init(pthis, ip, port, io_mtplx))
		return NULL;
	log_info("construt_udp_server: <%p> [%s:%d]", pthis, ip, port);

	return pthis;
}

void destruct_udp_server(struct udp_server* pthis)
{
	log_info("destruct_udp_server: <%p> [%s:%d]", 
			pthis, pthis->get_ip(pthis), pthis->get_port(pthis));
	free(pthis);
}



