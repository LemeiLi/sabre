#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/epoll.h>
#include <errno.h>
#include "infra/log/log.h"
#include "dal/dal_insgps.h"
#include "dal/xwgi7660.h"
#include "infra/link/datalink.h"
#include "infra/pack/datapack.h"
#include "infra/utils/utils.h"
#include "infra/serial/serial.h"
#include "uil/uil_insgps.h"
#include "infra/stream/stream.h"
#include "infra/utils/exception.h"

#define BLOCK_SIZE  (1024)

typedef struct com_data {
	int fd;
	size_t nr;        ///< total read
	size_t nl;        ///< total left for block
	char* buf;
	bool block;       ///< format: true block, false line 
	datalink* link;
	struct com_config* conf;
}com_data;

static int read_com_data_block(struct com_data* comx)
{
	ssize_t  nr;
	if ((nr = read(comx->fd, comx->buf + comx->nr, comx->nl)) < 0) {
		if (errno == EINTR || EAGAIN) {
			//log_error("read %d again: %s", fd, strerror(errno));
			nr = 0;          ///< call read_data_block again
			return -1;
		} else {
			//log_error("read %d falied: %s", comx->fd, strerror(errno));
			return -1;
		}
	} else if (nr == 0) {
		return -1; /* EOF */
	}

	comx->nl -= (size_t)nr; ///< left = left - nr;
	comx->nr += (size_t)nr; ///< nr = nr + nr
	//log_error("[%d] read %ld bytes, left %ld", comx->fd, comx->nr, comx->nl);
	if (comx->nl)   ///< read wanted blocks size
		return -1;

	return 0;
}

/**< TODO */
#if 0
static int read_com_data_line(struct com_data* comx)
{
	ssize_t nr = readline(comx->fd, comx->buf + comx->nr, BLOCK_SIZE);
	if (nr > 0)
		comx->nr += (size_t)nr;
	else
		return -1;
	//log_info("[%d] read %ld bytes to upload: %s", 
	//		  comx->fd, comx->nr, comx->buf);
	
	return 0;
}
#endif

static int8_t fd2comx_data(com_data data[], uint8_t act_com, int fd)
{
	for (int8_t i = 0; i < act_com; i++) {
		if (data[i].fd == fd)
			return i;
	}

	return -1;
}


static int rx_loop_epoll(XWGI7660* pthis)
{
	const int max_events = 10;
	struct epoll_event act_events[10];
	uint8_t act_com = pthis->config->com_num;
	int ret = 0, act_num = 0, act_fd = -1;
	datalink* link[act_com];
	com_data cdata[act_com];

	const uint32_t bs[SERIAL_NUM] = {
		BLOCK_SIZE >> 3, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE
	};

	int efd = epoll_create(1);
	if (efd < 0) {
		log_error("epoll create failed: %s", strerror(errno));
		goto exit;
	}

	for (int i = 0; i < act_com; i++) {
		serial* com = pthis->config->coms[i];
		if (!com)
			continue;
		set_non_blocking(com->fd);
		/**< monitor fd readable */
		update_events(efd, com->fd, EPOLLIN|EPOLLET|EPOLLERR, EPOLL_CTL_ADD);
		link[i] = pthis->link[i];

		cdata[i].buf = (char*)malloc(bs[i] + 1);      ///< reserved 1 byte "\0" to printf
		memset(cdata[i].buf, 0, bs[i] + 1);
		cdata[i].link = link[0];                      ///< use common link[0]
		cdata[i].nl = bs[i];
		cdata[i].fd = com->fd;
		cdata[i].conf = pthis->config->com_conf[i];
	}

	log_info("rx loop epoll version");
	while (!get_exit_flag() && pthis->run) {
		act_num = epoll_wait(efd, act_events, max_events, 1000);
		for (int i = 0; i < act_num; i++) {
			act_fd = act_events[i].data.fd;
			int k = fd2comx_data(cdata, act_com, act_fd);
			if (act_events[i].events & EPOLLIN) { ///< fd is readable	
#if 1
				if (-1 == read_com_data_block(&cdata[k])) goto again;
#else 
				if (-1 == read_com_data_line(&cdata[k])) goto again;
#endif
				datapack* dp = construct_datapack(NULL, cdata[k].buf,
						cdata[k].nr);
				if (!dp) {
					log_error("construct datapack failed!");
					ret = HAL_EALLOC;
					goto free;
				}

				//log_info("dal: [%d] read %ld bytes to upload:<%p> %p", 
				//	       act_fd, cdata[k].nr, dp, cdata[k].buf);
				
				dp->priv = (void*)cdata[k].conf;
				ret = link[0]->upload(link[0], dp);
				if (ret) {
					log_error("datalink upload failed!");
					dp->release((void**)&dp);
				}
				cdata[k].nr = 0;
				cdata[k].nl = bs[k];
				memset(cdata[k].buf, 0, bs[k]);
			} else {
				goto free;
			}
		}
again:
		continue;
	}

free:
	for (int i = 0; i < act_com; i++)
		free(cdata[i].buf);

exit:
	close(efd);
	log_error("com rx loop epoll exit!");

	return ret;
}

/*
static int rx_loop(XWGI7660* pthis, uint8_t index)
{
	int ret = 0;
	ssize_t size = 0;
	serial* com = pthis->config->coms[index];
	if (!com) {
		log_error("com1 is not active!");
		goto exit;
	}

	//datalink* link = pthis->link[index];
	datalink* link = pthis->link[0];
	struct com_config* conf = pthis->config->com_conf[index];

	char* rx_buf = (char*)malloc(BLOCK_SIZE + 1);
	if (!rx_buf) {
		log_error("allocate rx buffer falied!");
		ret = HAL_EALLOC;
		goto exit;
	}
	memset(rx_buf, 0, BLOCK_SIZE + 1);

	while (!get_exit_flag() && pthis->run) {
		log_info("com%d read %s data......", index, conf->type[0]);
		if (!strcmp(conf->type[0], "raw_data"))
			size = com->rx(com, rx_buf, BLOCK_SIZE);
		else
			size = com->rl(com, rx_buf, BLOCK_SIZE);
		
		if (size < 0) {
			log_error("com%d read error!", index);
			goto exit;
		}

		datapack* dp = construct_datapack(NULL, rx_buf, (size_t)size);
		if (!dp) {
			log_error("construct datapack failed!");
			ret = HAL_EALLOC;
			goto free;
		}
		ret = link->upload(link, dp);
		if (ret) {
			log_error("datalink upload failed!");
			dp->release((void**)&dp);
		}
	}

free:
	free(rx_buf);

exit:
	log_error("com%d rx loop exit!", index);
	return ret;
}
*/

static void* recv_com_epoll(void* args)
{
	XWGI7660* pthis = (XWGI7660*)args;
	int ret = rx_loop_epoll(pthis);

	log_info("recv coms epoll thread exit");
	pthread_exit((void*)(intptr_t)ret);

	return NULL;
}

/*
static void* recv_com0(void* args)
{
	XWGI7660* pthis = (XWGI7660*)args;
	int ret = rx_loop(pthis, 0);

	log_info("recv com0 thread exit");
	pthread_exit((void*)(intptr_t)ret);

	return NULL;
}

static void* recv_com1(void* args)
{
	XWGI7660* pthis = (XWGI7660*)args;
	int ret = rx_loop(pthis, 1);

	log_info("recv com1 thread exit");
	pthread_exit((void*)(intptr_t)ret);

	return NULL;
}

static void* recv_com2(void* args)
{
	XWGI7660* pthis = (XWGI7660*)args;
	int ret = rx_loop(pthis, 2);

	log_info("recv com2 thread exit");
	pthread_exit((void*)(intptr_t)ret);

	return NULL;
}

static void* recv_com3(void* args)
{
	XWGI7660* pthis = (XWGI7660*)args;
	int ret = rx_loop(pthis, 3);

	log_info("recv com3 thread exit");
	pthread_exit((void*)(intptr_t)ret);

	return NULL;
}

static void* (*recv_comx[SERIAL_NUM])(void*) = {
	recv_com0, recv_com1, recv_com2, recv_com3,
};
*/

///< sensor methods realization
static int xwgi7660_write_rtk(void* pthis, char* data, size_t size) 
{
	XWGI7660* pthis_ = (XWGI7660*)pthis; 
	serial* com = pthis_->config->coms[1]; ///< TODO com1 as RTK
	com->tx(com, data, size);

	return 0;
}

static void* xwgi7660_read_raw(void* pthis) 
{
	XWGI7660* pthis_ = (XWGI7660*)pthis; 

	return pthis_->link[0]->download(pthis_->link[0], 1);
}

///< sensor methods realization
static void* xwgi7660_read_cooked(void* pthis) 
{
	(void)pthis;
	log_info("reserved"); return NULL;
}

///< insgps methods realization
static int xwgi7660_set_sensitivity(void* pthis, float *sp)
{
	(void)pthis; (void)sp;
	log_info("reserved"); return 0;
}

///< ins methods realization
static int xwgi7660_reset_sensitivity(void* pthis)
{
	(void)pthis;
	log_info("reserved"); return 0;
}

///< xwgi7660 methods realization
static int xwgi7660_set_full_scale(XWGI7660* pthis, int fs)
{
	(void)pthis; (void)fs;
	log_info("reserved"); return 0;
}

///< 枚举所有的方法并赋值, 包括采用基类的实现的
static struct XWGI7660VMT vmt = {
	.get_object_address = NULL, ///< use parent realization
	.read_raw = xwgi7660_read_raw,
	.read_cooked = xwgi7660_read_cooked,

	.set_sensitivity = xwgi7660_set_sensitivity,
	.reset_sensitivity = xwgi7660_reset_sensitivity,
	.write_rtk = xwgi7660_write_rtk,

	.xwgi7660_set_full_scale = xwgi7660_set_full_scale,
};

int XWGI7660Init(XWGI7660* pthis, XWGI7660Config* pconf)
{
	pthis->vmt = &vmt;	
	BaseINSGPSInit(&pthis->super, pthis->vmt);

	/**< object 使用父类的实现, 赋值为指向父类的方法 */
	pthis->get_object_address = pthis->super.get_object_address;
	/**< sensor */
	pthis->read_raw = pthis->vmt->read_raw;
	pthis->read_cooked = pthis->vmt->read_cooked;
	/**< insgps */
	pthis->set_sensitivity = pthis->vmt->set_sensitivity;
	pthis->reset_sensitivity = pthis->vmt->reset_sensitivity;
	pthis->write_rtk = pthis->vmt->write_rtk;
	/**< xwgi7660 */
	pthis->xwgi7660_set_full_scale = pthis->vmt->xwgi7660_set_full_scale;
	
	pthis->config = pconf;
	
	pthis->accaxes = 3;
	pthis->state = 0;

#if 0
	for (int i = 0; i < pconf->com_num; i++) {
		pthis->link[i] = construct_datalink(NULL);
		if (!pthis->link[i]) {
			for (int j = 0; j <= i; j++) {
				destruct_datalink(pthis->link[j]);
			}
			return HAL_INSGPS_RET_EALLOC;
		}
		pthread_create(&pthis->rx_tid[i], NULL, recv_comx[i], (void*)pthis);
	}
#endif	
	pthis->link[0] = construct_datalink(NULL);
	pthis->run = true;
	pthread_create(&pthis->rx_tid[0], NULL, recv_com_epoll, (void*)pthis);

	return 0;
}

int XWGI7660DeInit(void* pthis)
{
	XWGI7660* pthis_ = (XWGI7660*)pthis;

	pthis_->run = false;
	//for (int i = 0; i < SERIAL_NUM; i++)
	//pthread_join(pthis_->rx_tid[i], NULL); 
	//destruct_datalink(pthis_->link[i]);

	pthread_join(pthis_->rx_tid[0], NULL); 
	destruct_datalink(pthis_->link[0]);

	return 0;	
}

XWGI7660* construct_xwgi7660(void* conf)
{
	XWGI7660* pthis = (XWGI7660*)malloc(sizeof(*pthis));
	if (!pthis) {
		log_error("construct xwgi7660 object failed!");
		return NULL;
	}
	XWGI7660Init(pthis, (XWGI7660Config *)conf);

	return pthis;
}

int destruct_xwgi7660(void* pthis)
{
	if (pthis) {
		XWGI7660DeInit(pthis);
		free(pthis);
	}

	return 0;
}

