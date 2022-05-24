/*************************************************************************
	> File Name: utils/utils.c
	> Created Time: Tue 09 Apr 2019 03:44:31 PM CST
 ************************************************************************/

#include<stdint.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include"utils/utils.h"

static volatile uint8_t ctrl_c_flag = 0;

uint8_t get_ctrl_c(void) 
{
	return ctrl_c_flag;
}

void set_ctrl_c(void) 
{
	ctrl_c_flag = 1;
}

void sigstop(int sig)
{
	fprintf(stderr, "capture CTRL+C signal, system exit!\n");
	set_ctrl_c();
}

/**<
 * Ignore broken pipe signal.
 */
void sigpipe(int sig)
{
	fprintf(stdout, "Capture a SIG_PIPE signal.\n");
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

int set_thread_cpu_affinity(pthread_attr_t* pattr, int cpu_index)
{
	int cpu_num = 0;
	cpu_num = get_cpu_count();
	fprintf(stdout, "The number of cpu is %d\n", cpu_num);

	cpu_set_t cpu_info;
	//CPU_ZERO(&cpu_info);
	//CPU_SET(cpu_index, &cpu_info);

	if (0 != pthread_attr_setaffinity_np(pattr, sizeof(cpu_set_t), &cpu_info)) {
		fprintf(stderr, "set affinity failed\n");
		return -1;
	}

	return 0;
}

//累加和运算
uint8_t check_sum_add8(const uint8_t buf[], size_t size) 
{
	uint8_t temp = 0;
	for (int i = 0; i < size; i++) {
		temp = (temp + buf[i]);
	}

	return temp;
}

//累加和运算
uint16_t check_sum_add16(const uint8_t buf[], size_t size) 
{
	uint16_t temp = 0;
	for (int i = 0; i < size; i++) {
		temp = temp + buf[i];
	}

	return temp;
}

//异或校验运算
uint8_t check_sum_xor8(const uint8_t buf[], size_t size) 
{
	uint8_t temp = 0;
	for (int i = 0; i < size; i++) {
		temp ^= buf[i];
	}

	return temp;
}

