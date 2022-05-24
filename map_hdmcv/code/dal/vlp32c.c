#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdbool.h>
#include "dal/dal_lidar.h"
#include "dal/vlp32c.h"
#include "infra/net/net.h"
#include "infra/net/udp_raw.h"
#include "infra/log/log.h"
#include "infra/utils/utils.h"
#include "infra/utils/exception.h"
#include "infra/pack/datapack.h"
#include "infra/link/datalink.h"
#include "infra/net/udp_service.h"
#include "infra/net/net_utils.h"

static void* capture_lidar_data(void* args)
{
	int ret = HAL_OK;
	vlp32c *lidar = (vlp32c *)args;
	ssize_t rcount;
	size_t to_recv = LIDAR_DATA_PACK_SIZE + ETH_IP_UDP_HEAD_SIZE;
	net *recver = (net *)lidar->config->dhnet;
	udp_raw *raw_net = (udp_raw *)lidar->priv; 

	uint8_t *pdata_buf = (uint8_t*)malloc(LIDAR_DATA_PACK_SIZE + ETH_IP_UDP_HEAD_SIZE);	
	if (!pdata_buf) {
		log_info("Allocate lidar data packet falied!");
		return NULL;
	}
	memset(pdata_buf, 0, LIDAR_DATA_PACK_SIZE + ETH_IP_UDP_HEAD_SIZE);

again:	
	rcount = (ssize_t)raw_net->recv_data(raw_net, pdata_buf, to_recv);
	if (raw_net->raw_filter(lidar->config->mac, pdata_buf))
		goto again;
	raw_net->close(raw_net);
	destruct_udp_raw(raw_net);

	to_recv = LIDAR_DATA_PACK_SIZE;
#if 0 /**< blocking IO */
	while (!get_exit_flag() && lidar->run) {

		do {
			rcount = (ssize_t)recver->recv_data(recver, 
					                   pdata_buf+ETH_IP_UDP_HEAD_SIZE, 
									   to_recv);
			if (rcount < 0) {
				if (errno == EINTR || errno == EAGAIN) {
					rcount = 0;
					continue;
				}
			} else if (rcount == 0){
				log_info("recver recv return 0");
				ret = HAL_EUDP;
				goto exit;	
			}
			to_recv -= (size_t)rcount;
		} while(to_recv > 0);
#else /**< nonblocking IO */
	set_non_blocking(recver->get_fd(recver));
	const int max_evts = 1;
	struct epoll_event act_evts[max_evts];
	int efd = epoll_create(1);
	update_events(efd, recver->get_fd(recver), EPOLLIN, EPOLL_CTL_ADD);

	while (!get_exit_flag() && lidar->run) {
		int num = epoll_wait(efd, act_evts, max_evts, 3000);
		for (int i = 0; i < num; i++) {
			if (act_evts[i].events & EPOLLIN) {
				rcount = recver->recv_data(recver, 
										   pdata_buf+ETH_IP_UDP_HEAD_SIZE, 
										   to_recv);
				if (rcount < 0) {
					if (errno == EINTR || errno == EAGAIN) {
						rcount = 0;
						continue;
					}
				} else if (rcount == 0){
					log_info("recver recv return 0");
					ret = HAL_EUDP;
					goto exit;	
				}
				to_recv -= (size_t)rcount;
			}
		} 
		if(to_recv) continue;
#endif
		struct datapack* pdata = construct_datapack(NULL, pdata_buf, 
			                     LIDAR_DATA_PACK_SIZE+ETH_IP_UDP_HEAD_SIZE);
		if (!pdata) {
			ret = HAL_EALLOC;
			goto exit;
		}
		uint8_t* ts = (uint8_t *)pdata_buf + 
			          LIDAR_DATA_TS_OFFSET + ETH_IP_UDP_HEAD_SIZE;
		/**< little endian: least significant byte first */
		uint32_t ts_usec = (uint32_t)((*(ts + 3) << 24) +
						   (*(ts + 2) << 16) +
						   (*(ts + 1) <<  8) +
						   (*(ts + 0) <<  0)) ;
		lidar->vlp32c_data_crash_probe(lidar, ts_usec);
		pdata->set_dts(pdata, ts_usec);
		ret = lidar->link->upload(lidar->link, pdata);
		if (ret) {
			pdata->release((void**)&pdata);
			goto exit;
		}		
		to_recv = LIDAR_DATA_PACK_SIZE;
		memset(pdata_buf+ETH_IP_UDP_HEAD_SIZE, 0, LIDAR_DATA_PACK_SIZE);
	}

exit:
	free(pdata_buf);
	recver->close(recver);
	log_info("dal lidar thread [%d %s:%d] exit!",
			recver->fd,
			recver->get_ip(recver), recver->get_port(recver));

	pthread_exit((void*)(intptr_t)ret);

	return NULL;
}

/**< object method override */
static void* vlp32c_get_object_address(void* pthis)
{
	log_info("vlp32c get_object_address override: %p", pthis);
	return pthis;
}

/**< sensor methods realization */
static void* vlp32c_read_raw(void* pthis) 
{
	vlp32c* pthis_ = (vlp32c*)pthis;

	return pthis_->link->download(pthis_->link, 1);
}

static void* vlp32c_read_cooked(void* pthis) 
{
	(void)pthis;
	log_info("%s", __func__); return NULL;
}

/**< lidar methods realization */
static int vlp32c_set_sensitivity(void* pthis, float *sp)
{
	(void)pthis; (void)sp;
	log_info("%s", __func__); return 0;
}

static int vlp32c_reset_sensitivity(void* pthis)
{
	(void)pthis;
	log_info("%s", __func__); return 0;
}

/**< vlp32c method */
static int vlp32c_set_full_scale(void* pthis, int fs)
{
	(void)pthis; (void)fs;
	log_info("%s", __func__); return 0;
}

static int vlp32c_data_crash_probe(void* pthis, uint32_t ts_usec)
{
	(void)pthis;
	static uint32_t packet_count;
	static uint32_t p1_ts, p2_ts, p3_ts, p4_ts, p5_ts;
	static uint32_t n_ts[11];
	static uint32_t n, etotal;
	static bool except = false;

	int32_t delta = 0;

	packet_count++;
	if (!p1_ts)
		p1_ts = ts_usec;
	else {
		if (ts_usec > p1_ts)
			delta = ts_usec - p1_ts;
		else 
			delta = (int32_t)(ts_usec + (3600000000 - p1_ts));
		if (delta > 700 || delta < 600) {
			fprintf(stderr, "p1: %u(%d)", p1_ts, ts_usec-p1_ts);
			fprintf(stderr, ", p2: %u(%d)", p2_ts, p1_ts-p2_ts);
			fprintf(stderr, ", p3: %u(%d)", p3_ts, p2_ts-p3_ts);
			fprintf(stderr, ", p4: %u(%d)", p4_ts, p3_ts-p4_ts);
			fprintf(stderr, ", p5: %u(%d)\n", p5_ts, p4_ts-p5_ts);
			fprintf(stderr, "--->>> <%u>exceptional ts: %u\n", ts_usec, packet_count);
			except = true;
			etotal++;
			n = 0;
		}

		if (except && n < 11) {
			n_ts[n]	= ts_usec;
			n++;
			if (n == 11) {
				fprintf(stderr, "n1: %u(%d)", n_ts[1], n_ts[1]-n_ts[0]);
				fprintf(stderr, ", n2: %u(%d)", n_ts[2], n_ts[2]-n_ts[1]);
				fprintf(stderr, ", n3: %u(%d)", n_ts[3], n_ts[3]-n_ts[2]);
				fprintf(stderr, ", n4: %u(%d)", n_ts[4], n_ts[4]-n_ts[3]);
				fprintf(stderr, ", n5: %u(%d)", n_ts[5], n_ts[5]-n_ts[4]);
				fprintf(stderr, ", n6: %u(%d)", n_ts[6], n_ts[6]-n_ts[5]);
				fprintf(stderr, ", n7: %u(%d)", n_ts[7], n_ts[7]-n_ts[6]);
				fprintf(stderr, ", n8: %u(%d)", n_ts[8], n_ts[8]-n_ts[7]);
				fprintf(stderr, ", n9: %u(%d)", n_ts[9], n_ts[9]-n_ts[8]);
				fprintf(stderr, ", n10: %u(%d)\n", n_ts[10], n_ts[10]-n_ts[9]);
				except = false;
			}
		}
		p5_ts = p4_ts;
		p4_ts = p3_ts;
		p3_ts = p2_ts;
		p2_ts = p1_ts;
		p1_ts = ts_usec;
	}

	return 0;
}

static struct VLP32CVMT vmt_dev = {
	/**< realization for BaseObject */
	.get_object_address = vlp32c_get_object_address,  ///< override

	/**< realization for BaseSensor */
	.read_raw = vlp32c_read_raw,
	.read_cooked = vlp32c_read_cooked,

	/**< realization for BaseLidar */
	.set_sensitivity = vlp32c_set_sensitivity,
	.reset_sensitivity =  vlp32c_reset_sensitivity,
		
	/**< realization for vlp32c */
	.vlp32c_set_full_scale = vlp32c_set_full_scale,
	.vlp32c_data_crash_probe = vlp32c_data_crash_probe,
};

int VLP32CInit(vlp32c* pthis, VLP32CConfig* pconf)
{
	pthis->vmt = &vmt_dev;	
	BaseLidarInit(&pthis->super, pthis->vmt);

	///< object	
	pthis->get_object_address = pthis->vmt->get_object_address;
	///< sensor
	pthis->read_raw = pthis->vmt->read_raw;
	pthis->read_cooked = pthis->vmt->read_cooked;
	///< lidar 
	pthis->set_sensitivity = pthis->vmt->set_sensitivity;
	pthis->reset_sensitivity = pthis->vmt->reset_sensitivity;
	///< vlp32c-liser-lidar
	pthis->vlp32c_set_full_scale = pthis->vmt->vlp32c_set_full_scale;
	pthis->vlp32c_data_crash_probe = pthis->vmt->vlp32c_data_crash_probe;

	pthis->config = pconf;
	pthis->state = 0;

	/**< A.1.1.4.1.1 */	
	pthis->link = construct_datalink(NULL);

	if (!pthis->link) return HAL_EALLOC;
	pthis->run = true;

	/**< A.1.1.4.1.2 */	
	udp_raw *raw_net = construct_udp_raw(pconf->dhitf, (const uint8_t*)"0.0.0.0", 0, false);
	if (!raw_net) {
		log_info("construct_udp_raw falied!");
		return HAL_EUDPRAW;
	}
	pthis->priv = raw_net;
	pthread_create(&pthis->rx_tid, NULL, capture_lidar_data, pthis);
	log_info("lidar capture init success!");

	return HAL_OK;
}

int VLP32CDeinit(void* pthis)
{ 
	int *ret;
	vlp32c* pthis_ = (vlp32c*)pthis;
	pthis_->run = false;
	pthread_join(pthis_->rx_tid, (void**)&ret);
	log_info("vlp32c lidar exit: %d", (int)(intptr_t)ret);
	datalink_deinit(pthis_->link);

	return (int)(intptr_t)ret;
}

/**< A.1.1.4 */	
vlp32c* construct_vlp32c(VLP32CConfig *pconf)
{
	vlp32c *pthis = (vlp32c*)malloc(sizeof(*pthis));
	if (!pthis) 
		return NULL;

	/**< A.1.1.4.1 */	
	if(VLP32CInit(pthis, pconf)) {
		free(pthis);
		return NULL;
	}
	log_info("construct vlp32c!");

	return pthis;
}

int destruct_vlp32c(vlp32c *pthis)
{
	int *ret = NULL;
	pthis->run = false;

	pthread_join(pthis->rx_tid, (void**)&ret);
	log_info("get capture_lidar_data exit id: %d", (int)(intptr_t)ret);
	destruct_datalink(pthis->link);
	free(pthis);
	zlog_info(g_zc, "destruct vlp32c!");

	return (int)(intptr_t)ret;
}









