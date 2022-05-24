/*************************************************************************
	> File Name: net.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Thu 17 Oct 2019 05:41:20 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "infra/net/net.h"
#include "infra/log/log.h"
#include "infra/utils/exception.h"

static int32_t net_get_fd(void* pthis)
{
	struct net *pthis_ = (struct net*)pthis;

	if (pthis_->vptr->get_fd == NULL) 
		return pthis_->fd;
	else 
		return pthis_->vptr->get_fd(pthis); ///< call derived override method
}

static uint16_t net_get_port(void* pthis)
{
	struct net *pthis_ = (struct net*)pthis;

	if (pthis_->vptr->get_port == NULL) 
		return pthis_->port;
	else 
		return pthis_->vptr->get_port(pthis); ///< call derived override method
}

static uint8_t* net_get_ip(void* pthis)
{
	struct net *pthis_ = (struct net*)pthis;
	if (pthis_->vptr->get_ip == NULL) 
		return pthis_->ip;
	else 
		return pthis_->vptr->get_ip(pthis); ///< call derived override method
}

static uint8_t* net_get_itf(void* pthis)
{
	struct net *pthis_ = (struct net*)pthis;
	if (pthis_->vptr->get_itf == NULL) 
		return pthis_->itf;
	else 
		return pthis_->vptr->get_itf(pthis); ///< call derived override method
}

static uint8_t* net_get_mac(void* pthis)
{
	struct net *pthis_ = (struct net*)pthis;
	if (pthis_->vptr->get_mac == NULL) 
		return pthis_->mac;
	else 
		return pthis_->vptr->get_mac(pthis); ///< call derived override method
}

static int32_t net_close(void* pthis)
{
	struct net *pthis_ = (struct net*)pthis;

	if (pthis_->vptr->close == NULL) 
		return close(pthis_->fd);     ///< call default method
	else 
		return pthis_->vptr->close(pthis); ///< call derived override method
}

int net_init(void* pthis, void* vptr, const uint8_t *itf, 
		     const uint8_t *mac, const uint8_t* ip, uint16_t port, bool io_mtplx)
{
	(void)io_mtplx;
	struct net *pthis_ = (struct net*)pthis;
	pthis_->vptr = (struct net_vmt*)vptr;
	if (ip) strncpy((char* )pthis_->ip, (const char* )ip, 16);
	if (itf) strncpy((char*)pthis_->itf, (const char* )itf, 16);
	if (mac) strncpy((char*)pthis_->mac, (const char* )mac, 18);
	pthis_->port = port;
	//log_info("net->connect_server = %p", pthis_->vptr->connect_server);

	pthis_->connect_server = pthis_->vptr->connect_server;
	pthis_->recv_data = pthis_->vptr->recv_data;
	pthis_->send_data = pthis_->vptr->send_data;
	pthis_->get_state = pthis_->vptr->get_state;

	pthis_->close = net_close;
	pthis_->get_ip = net_get_ip;
	pthis_->get_port = net_get_port;
	pthis_->get_mac = net_get_mac;
	pthis_->get_itf = net_get_itf;
	pthis_->get_fd = net_get_fd;

	return HAL_OK;
}


