#ifndef _RX_REFACT_H
#define _RX_REFACT_H

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <linux/sockios.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**< image protocal format: 
 * 3 + 1 + 4 + 8 + 1 + 2 + data + 2 = 21 + data
 * 3 head + 1 slice index + 4 data length + 8 timestamp + 1 id + 2 slice flags + data + 2 check sum
 **/
#define TYPE_IMU 0x1
#define TYPE_GPS 0x2
#define TYPE_ODO 0x3
#define TYPE_IMAGE 0x4
#define DATA_POS 17 ///< 4 head + 4 length + 8 timestamp + 1 id + [data] + 2 checksum

#define SERVER_PORTA 	5678
#define SERVER_PORTB 	6789

#define SERVER_IP       "10.0.0.2"
#define NET_PROTOCOL    SOCK_STREAM

#define IMG_SIZE (10*1024*1024)
#define GPS_SIZE (5*1024)
#define IMU_SIZE (5*1024)
#define ODO_SIZE (5*1024)

#define BUF_SIZE (200*1024 + 1024)

struct addr_map 
{
	char* ip;
	short port;
};

struct mcu_imu 
{
	double timestamp;	///< s
	float acc[3];       ///< g
	float gyr[3];       ///< rad/s
}__attribute__((packed));

#define MCU_GPS_GSV 30
struct mcu_gps_gsv 
{
	int sv_num;
	int sv_id[MCU_GPS_GSV];   
	int sv_elv[MCU_GPS_GSV];  
	int sv_az[MCU_GPS_GSV];   
	int sv_cno[MCU_GPS_GSV];  
}__attribute__((packed)); 
                      
struct mcu_gps 
{     
	double timestamp;   // s  
	double pos[3];      // rad, rad, m
	float  vel[3];      // m/s, m/s, m/s
	float  hdop;     
	float  azi_track;   // rad
	float  speed;       // m/s
	int    sat_num;  
	unsigned int status;

	struct mcu_gps_gsv gpgsv; 
}__attribute__((packed)); 

struct mcu_odo {
	double timestamp;
	int FrontLeftSpeed;
	int FrontRightSpeed;
    int RearLeftSpeed;
    int RearRightSpeed;
}__attribute__((packed));


static volatile uint8_t ctrl_c_flag = 0;
uint8_t get_ctrl_c(void) 
{
	return ctrl_c_flag;
}

void set_ctrl_c(void) 
{
	ctrl_c_flag = 1;

	return;
}

void sigstop(int sig)
{
	fprintf(stderr, "capture CTRL+C signal, system exit!\n");
	ctrl_c_flag = 1;
}

void sigpipe(int sig)
{
	fprintf(stdout, "Capture a SIG_PIPE signal.\n");
	return;
}

uint8_t xor8(uint8_t* data, size_t size)
{
	uint8_t check_sum = 0;
	
	for(int i = 0; i < size; i=i+1024)
	//for(int i = 0; i < size; i++)
		check_sum ^= data[i];

	return check_sum;
}

void set_scheduler(void)
{
	struct sched_param param; 
	int maxpri; 
	maxpri = sched_get_priority_max(SCHED_FIFO);
	if(maxpri == -1) 
	{ 
		perror("sched_get_priority_max() failed"); 
		exit(1); 
	} 
	param.sched_priority = maxpri; 
	if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) 
	{ 
		perror("sched_setscheduler() failed"); 
		exit(1); 
	} 
}

void persistence(char* name, void* buf, size_t size, int flag) 
{
	char filename[100];
	//sprintf(filename, "%s/%s", "/media/lijianhua/08C9068C115CF85E/data", name);
	sprintf(filename, "%s/%s", "./data", name);
	
	int fdw = open(filename, flag, 0666);
	if(fdw < 0){
		fprintf(stderr, "open %s failed\n", filename);
		return;
	}
	size_t wcount = 0; 
	while(wcount < size){
		ssize_t ret = write(fdw, buf+wcount, size-wcount);
		if (ret > 0)   
			wcount += ret;
		else if (ret < 0) {
			fprintf(stderr, "write error: %s\n", strerror(errno));
			close(fdw);
			return;
		}
	}
	close(fdw);

	return;
}

int connect_server(struct addr_map* addr)
{
	int sock_fd;
	struct sockaddr_in srv_addr;
	struct addr_map map = {
		.ip = addr->ip,
		.port = addr->port,
	};

	sock_fd = socket(AF_INET, NET_PROTOCOL, 0);
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port   = htons(map.port);  
 	if (inet_aton(map.ip, &srv_addr.sin_addr) == 0) {
		fprintf(stderr, "invalid server_ip\n");
		return -1;
	}
	memset(&srv_addr.sin_zero, 0, sizeof(srv_addr.sin_zero));

	if (connect(sock_fd,(struct sockaddr *)(&srv_addr),sizeof(struct sockaddr)) == -1) {
		fprintf(stderr,"connect server [%s:%d] error %s\a\n", map.ip, map.port, strerror(errno));
		return -1;	
	} else {
		fprintf(stdout, "connect server channel [%s:%d] ok.\n", map.ip, map.port);
	}

	return sock_fd;
	
}
/* Read "n" bytes from a descriptor */
ssize_t readn(int fd, void *ptr, size_t n)
{
    size_t  nleft;
    ssize_t nread;

    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return(-1); /* error, return -1 */
            else
                break;      /* error, return amount read so far */
        } else if (nread == 0) {
            break;          /* EOF */
        }
        nleft -= nread;
        ptr += nread;
    }
    return(n - nleft);      /* return >= 0 */
}

/* Write "n" bytes to a descriptor */
ssize_t writen(int fd, const void *ptr, size_t n)             
{
    size_t  nleft;
    ssize_t nwritten;

    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return(-1); /* error, return -1 */
            else
                break;      /* error, return amount written so far */
        } else if (nwritten == 0) {
            break;
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n - nleft);      /* return >= 0 */
}

void total_gaps(uint8_t fps, double* delta, uint32_t image_num, uint32_t imu, uint32_t gps, uint32_t odo, uint32_t error_counter, void* args) 
{
	static struct timeval ssbegin, prebegin;

	gettimeofday(&ssbegin, NULL);
	uint32_t timett_image = ssbegin.tv_sec*1000000 + ssbegin.tv_usec - prebegin.tv_sec*1000000 - prebegin.tv_usec;
	fprintf(stderr, "[%s:%d] gap = %.3fms, error counter = %d, rx images = %d, imu = %d, gps = %d, odo = %d\n", 
		((struct addr_map*)args)->ip, ((struct addr_map*)args)->port, (double)timett_image/1000, error_counter, image_num, imu, gps, odo);
	*delta += timett_image;
	if (!(image_num % (fps*10))) {
		fprintf(stdout, "total = %d, %d fps = %fs\n", image_num, fps, (*delta/1000000)/10);
		*delta = 0;
	}
		
	prebegin = ssbegin;

	return;
}
#ifdef __cplusplus
}
#endif

#endif // _RX_REFACT_H
