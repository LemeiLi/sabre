/*************************************************************************
	> Created Time: Thu 25 Apr 2019 07:44:31 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include "infra/link/fifo.h"
#include "infra/utils/utils.h"
#include "infra/log/log.h"
#include "uil/uil_image.h"

fifo_t* fifo_new(void)
{
	fifo_t* fifo = (fifo_t *)malloc(sizeof(fifo_t));
	if (fifo == NULL) {
		zlog_error(g_zc, "malloc fifo_t object error");
		return NULL;
	}

	fifo->rear = fifo->front = NULL;
	fifo->nr_active = 0;
	pthread_mutex_init(&fifo->mutex, NULL);
	pthread_cond_init(&fifo->cond, NULL);

	return fifo;
}

int fifo_push(fifo_t* fifo, void* data)
{
	node_t* node = (node_t *)malloc(sizeof(node_t));
	if (node == NULL) {
		zlog_error(g_zc, "malloc node_t object error");
		return HAL_IMAGE_RET_EALLOC;
	}

	node->next = NULL;
	node->data = data;	

//	if (fifo->nr_active > 500) {
		//error("warning: net work blocked!!!\n");
//	}
	
	{ pthread_mutex_lock(&fifo->mutex);
		if (fifo_is_empty(fifo)) {
			fifo->rear = node;		
			fifo->front = node;		
		} else {
			fifo->rear->next = node;
			fifo->rear = node;
		}
		fifo->nr_active++;
	} pthread_mutex_unlock(&fifo->mutex);
	pthread_cond_broadcast(&fifo->cond);

	return HAL_IMAGE_RET_OK;
}
/**<
 * asume that fifo is not empty.
 */
void* fifo_pop(fifo_t* fifo, int32_t timeout_sec)
{
	void* data = NULL;
	node_t* node;
	struct timespec ts;
	struct timeval tp;

	gettimeofday(&tp, NULL);
	ts.tv_sec  = tp.tv_sec;
	ts.tv_nsec = tp.tv_usec * 1000;
	ts.tv_sec += timeout_sec;

	{ pthread_mutex_lock(&fifo->mutex);
		while (fifo_is_empty(fifo)) {
			int ret = pthread_cond_timedwait(&fifo->cond, &fifo->mutex, &ts);
			if (ret == ETIMEDOUT) {
				pthread_mutex_unlock(&fifo->mutex);
				return NULL;
			}
		}

		node = fifo->front;
		if (fifo->front == fifo->rear) { ///< only one element
			fifo->front = fifo->rear = NULL;	
		} else {
			fifo->front = fifo->front->next;		
		}
		fifo->nr_active--;
	} pthread_mutex_unlock(&fifo->mutex);

	data = node->data;
	free(node);

	return data;
}

void fifo_free(fifo_t* fifo)
{
	pthread_mutex_destroy(&fifo->mutex);
	pthread_cond_destroy(&fifo->cond);
	free(fifo);
	fifo = NULL;

	return;
}

int fifo_is_empty(fifo_t* fifo)
{
	return ((fifo->rear == NULL) && (fifo->front == NULL)) ? 1 : 0;
}

uint32_t get_nr_active(fifo_t* fifo)
{
	return fifo->nr_active;	
}


