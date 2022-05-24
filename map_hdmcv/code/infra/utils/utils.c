/*************************************************************************
	> File Name: utils/utils.c
	> Created Time: Tue 09 Apr 2019 03:44:31 PM CST
 ************************************************************************/

#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/epoll.h>
#include <string.h>
#include "infra/utils/utils.h"
#include "infra/log/log.h"

static volatile uint8_t exit_flag = 0;

uint8_t get_exit_flag(void) 
{
	return exit_flag;
}

void set_exit_flag(void) 
{
	exit_flag = 1;

	return;
}

void clear_exit_flag(void) 
{
	exit_flag = 0;

	return;
}

void signal_stop(int sig)
{
	(void)sig;
	log_warn("capture CTRL+C signal, system exit!");
	set_exit_flag();

	return;
}

/**<
 * Ignore broken pipe signal.
 */
void signal_pipe(int sig)
{
	(void)sig;
	log_warn("capture a SIG_PIPE signal.");
	set_exit_flag();

	return;
}

void signal_bus(int sig)
{
	(void)sig;
	log_warn("capture a SIG_BUS signal.");
	set_exit_flag();

	return;
}

void signal_segv(int sig)
{
	(void)sig;
	log_warn("capture a SIG_SEGV signal.");
	set_exit_flag();

	return;
}

int get_cpu_count(void)
{
	return (int)sysconf(_SC_NPROCESSORS_ONLN);
}

int set_non_blocking(int fd) 
{ 
	int opts; 
	opts = fcntl(fd,F_GETFL); 
	if (opts < 0) { 
		return -1; 
	} 

	opts = opts | O_NONBLOCK; 

	if (fcntl(fd, F_SETFL,opts) < 0) { 
		return -1; 
	} 

	return 0;
}

void update_events(int efd, int fd, uint32_t events, int op)
{
	//bool in_flag = false;
	//bool out_flag = false;
	struct epoll_event ev;

	memset(&ev, 0, sizeof(ev));
	ev.events = events;
	ev.data.fd = fd;
	//if(ev.events & EPOLLIN) in_flag = true;
	//if(ev.events & EPOLLOUT) out_flag = true;
	/*			
	log_info("%s [%d], events read [%s] write [%s]",
		op == EPOLL_CTL_MOD ? "mod" : (op == EPOLL_CTL_ADD ? "add" : "del"),
		fd,
		in_flag ? "SET" : "UNSET",
		out_flag ? "SET" : "UNSET");
	*/
	if (epoll_ctl(efd, op, fd, &ev)) 
		log_error("epoll_ctl failed: %s", strerror(errno));
}

/*
int set_thread_cpu_affinity(pthread_attr_t* pattr, int cpu_index)
{
	int cpu_num = 0;
	cpu_num = get_cpu_count();
	logger("The number of cpu is %d\n", cpu_num);

	cpu_set_t cpu_info;
	CPU_ZERO(&cpu_info);
	CPU_SET(cpu_index, &cpu_info);

	if (0 != pthread_attr_setaffinity_np(pattr, sizeof(cpu_set_t), &cpu_info)) {
		error("set affinity failed\n");
		return -1;
	}

	return 0;
}
*/
//累加和运算
uint8_t check_sum_add8(const uint8_t buf[], size_t size) 
{
	uint8_t temp = 0;
	for (size_t i = 0; i < size; i++) {
		temp = (uint8_t)(temp + buf[i]);
	}

	return temp;
}

//累加和运算
uint16_t check_sum_add16(const uint8_t buf[], size_t size) 
{
	uint16_t temp = 0;
	for (size_t i = 0; i < size; i++) {
		temp = (uint8_t)(temp + buf[i]);
	}

	return temp;
}

//异或校验运算
uint8_t check_sum_xor8(const uint8_t buf[], size_t size) 
{
	uint8_t temp = 0;
	for (size_t i = 0; i < size; i++) {
		temp ^= buf[i];
	}

	return temp;
}

/**
 * eg.
 * x = 1024, return 10
 */
uint8_t bit2shift(uint32_t x)
{
	uint8_t shift = 0;

	for (x = x >> 1; x != 0; x = x >> 1) {
		shift++;
	}

	return shift;
}



