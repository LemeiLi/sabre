/*************************************************************************
	> File Name: transmitter.c
	> Created Time: Mon 01 Jul 2019 07:22:19 PM CST
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "consumer/common/channel.h"
#include "consumer/common/transmitter.h"
#include "net/nrb_imu.h"
#include "net/nrb_gps.h"
#include "net/nrb_odo.h"
#include "net/nrb_image.h"
#include "utils/utils.h"

static void* live_stream_thread(void *args);
struct transmitter* create_transmitter_thread(struct liver* pliver, struct channel* pch)
{
	struct transmitter* pthis = malloc(sizeof(struct transmitter));
	pthis->pliver = pliver;
	pthis->data_ch = pch;
	pthread_create(&pthis->tthread, NULL, live_stream_thread, pthis);

	return pthis;
}

int destroy_transmitter(struct transmitter* pthis)
{
	void* status;
	if (pthis) {
		pthread_join(pthis->tthread, &status);
		log("[%ld] %s\n", pthis->tthread, (char*)status);
		free(pthis);
	}

	return 0;
}

static void* live_stream_thread(void *args)
{
	struct transmitter* pthis = (struct transmitter*)args;
	struct liver* pliver = pthis->pliver;

	const int max_events = 1;
	struct epoll_event activeEvs[max_events];
	int client_fd = 0;
	int epoll_fd = epoll_create(1);

	while (!get_ctrl_c()) {
		struct nrb_desc* pbase = fifo_pop(pthis->data_ch->fifo_nrb, 2);
		if (pbase == NULL) {
			continue;
		}
		size_t to_send = pbase->to_send;
		uint8_t* pdata = pbase->rfmap;

		if (!(client_fd = get_client_state(pliver))) {
			goto recycle;
		}

		if (client_fd != get_client_fd(pliver)) { ///< new client connected.
			client_fd = get_client_fd(pliver);
			set_non_blocking(client_fd);
			update_events(epoll_fd, client_fd, EPOLLOUT|EPOLLIN, EPOLL_CTL_ADD); 
		}

		while ((to_send > 0) && get_client_state(pliver)) {
			int num = epoll_wait(epoll_fd, activeEvs, max_events, 10000);
			for (int i = 0; i < num; i++) {
				if (activeEvs[i].events & EPOLLOUT) { ///< fd can write 
					int ret = send(client_fd, pdata, to_send, MSG_NOSIGNAL); 
					if (ret > 0) {
						to_send -= ret;			
						pdata += ret;
						continue;
					} else if (ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
						error("try again!\n"); 
						//errcouter++;
						continue;
					} else if (ret < 0 && (errno == ECONNRESET || errno == EBADF)){
						error("write image to client failed!\n");
						//errcouter++;
						close(client_fd);
						break;
					} else {
						error("error occur!\n");
						close(client_fd);
						break;
					}
				} 
			}
		}
recycle:
		fifo_push(pthis->data_ch->fifo_nrb_rcc, pbase);	
		//log("send total image blocks = %d, error counter = %d\n", total, errcouter);
	}
	close(epoll_fd);
	pthread_exit("live_stream_thread exit!");

	return NULL;
}


