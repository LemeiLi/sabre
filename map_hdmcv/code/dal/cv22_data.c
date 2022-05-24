/*************************************************************************
	> File Name: cv22_data.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Mon 21 Oct 2019 02:10:57 PM CST
 ************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include "dal/cv22_data.h"
#include "dal/cv22_4cam.h"
#include "infra/utils/utils.h"
#include "infra/pack/datapack.h"
#include "infra/link/datalink.h"
#include "infra/stream/persistence.h"
#include "infra/net/tcp_service.h"
#include "infra/log/log.h"
#include "uil/uil_image.h"

static void parse_persistence(int type, void* data, size_t size)
{
	struct mcu_gps gps = {0}; 
	struct mcu_imu imu = {0}; 
	struct mcu_odo odo = {0}; 
	int8_t ptxt[1024] = {0};
	size_t nbytes = 0;

	switch(type) {
		case TYPE_GPS:
			memcpy(&gps, data, size);
			//zlog_info(g_zc, "gps length %ld, timestamp: %lf", size, gps.timestamp);
			nbytes = (size_t)snprintf((char *__restrict)ptxt, 1024, 
				"%ld %14.10lf %14.10lf %8.3lf %7.3lf %7.3lf %7.3lf %6.3lf %7.3lf %7.3lf %02d %01d %02d",
				(uint64_t)(gps.timestamp*1000000),
				gps.pos[0],
				gps.pos[1],
				gps.pos[2],
				(double)gps.vel[0],
				(double)gps.vel[1],
				(double)gps.vel[2],
				(double)gps.hdop,
				(double)gps.azi_track,
				(double)gps.speed,
				gps.sat_num,
				gps.status,
				gps.gpgsv.sv_num);

			for (int i = 0; i < MCU_GPS_GSV; i++) {
				nbytes += (size_t)snprintf((char* __restrict)(ptxt+nbytes), 1024-nbytes, " %03d ", gps.gpgsv.sv_id[i]);
			}
			for (int i = 0; i < MCU_GPS_GSV; i++) {
				nbytes += (size_t)snprintf((char* __restrict)(ptxt+nbytes), 1024-nbytes, " %02d ", gps.gpgsv.sv_elv[i]);
			}
			for (int i = 0; i < MCU_GPS_GSV; i++) {
				nbytes += (size_t)snprintf((char* __restrict)(ptxt+nbytes), 1024-nbytes, " %03d ", gps.gpgsv.sv_az[i]);
			}
			for (int i = 0; i < MCU_GPS_GSV; i++) {
				nbytes += (size_t)snprintf((char* __restrict)(ptxt+nbytes), 1024-nbytes, " %02d ", gps.gpgsv.sv_cno[i]);
			}	
			nbytes += (size_t)snprintf((char* __restrict)(ptxt+nbytes), 1024-nbytes, "\r\n");
			persistence("gps.txt", ptxt, nbytes, O_CREAT|O_WRONLY|O_APPEND);
			break;

		case TYPE_IMU:
			memcpy(&imu, data, size);
			//zlog_info(g_zc, "imu length %ld, timestamp: %lf", size, imu.timestamp);
			nbytes = (size_t)snprintf((char* __restrict)ptxt, 1024, "%ld %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf\r\n",
				(uint64_t)(imu.timestamp*1000000),
				(double)imu.acc[0],
			    (double)imu.acc[1],
			    (double)imu.acc[2],
				(double)imu.gyr[0],
				(double)imu.gyr[1],
				(double)imu.gyr[2]);
			persistence("imu.txt", ptxt, nbytes, O_CREAT|O_WRONLY|O_APPEND);
			break;

		case TYPE_ODO:
			memcpy(&odo, data, size);
			//zlog_info(g_zc, "odo length %ld, timestamp: %lf", size, odo.timestamp);
			nbytes = (size_t)snprintf((char* __restrict)ptxt, 1024, "%ld %05d %05d %05d %05d\r\n",
				(uint64_t)(odo.timestamp*1000000),
				odo.FrontLeftSpeed,
				odo.FrontRightSpeed,
				odo.RearLeftSpeed,
				odo.RearRightSpeed);
			persistence("odo.txt", ptxt, nbytes, O_CREAT|O_WRONLY|O_APPEND);
			break;

		default:
			break;
	}

}

static void head_dump(uint8_t* rbuf, size_t len, size_t to_recv) 
{
	uint64_t times = 0, timeu = 0, timespec = 0;
	log_info("head check error: ");
	for (size_t i = 0; i < len; i++)
		log_info("[%ld] %x, ", i, rbuf[i]);
	log_info(" ");

	log_info("slice_index = %x", rbuf[3]);
	int image_slen = rbuf[7];
	image_slen = (image_slen << 8) + rbuf[6];
	image_slen = (image_slen << 8) + rbuf[5];
	image_slen = (image_slen << 8) + rbuf[4];
	log_info("image_slen = %d", image_slen);

	log_info("to_recv = %ld", to_recv);

	times = rbuf[11];
	times = (times << 8) + rbuf[10];
	times = (times << 8) + rbuf[9];
	times = (times << 8) + rbuf[8];
	
	timeu = rbuf[15];
	timeu = (timeu << 8) + rbuf[14];
	timeu = (timeu << 8) + rbuf[13];
	timeu = (timeu << 8) + rbuf[12];
	timespec = times*1000000 + timeu;
	log_info("timestamp = %ld", timespec);
	log_info("id = %x", rbuf[16]);
	log_info("slice flag = %x, %x", rbuf[17], rbuf[18]);

	return;
}

static void total_gaps(uint8_t fps, double* delta, uint32_t image_num, uint32_t imu, 
		        uint32_t gps, uint32_t odo, uint32_t error_counter, void* args) 
{
	(void)imu; (void)gps; (void)odo;
	static struct timeval ssbegin, prebegin;
	struct CV224Cam* cam = (struct CV224Cam*)args;
	struct net* client = (struct net*)cam->config->net;

	gettimeofday(&ssbegin, NULL);
	int64_t timett_image = ssbegin.tv_sec*1000000 + ssbegin.tv_usec - \
		                    prebegin.tv_sec*1000000 - prebegin.tv_usec;
#if 1
	zlog_info(g_zc, "[%s:%d] gap = %.3fms, error counter = %d, rx images = %d",
		      client->get_ip(client), client->get_port(client),
			  (double)timett_image/1000, error_counter, image_num);
#endif
	*delta += (double)timett_image;
	if (!(image_num % ((uint32_t)fps*10))) {
		zlog_info(g_zc, "total = %d, %d fps = %fs", image_num, fps, (*delta/1000000)/10);
		*delta = 0;
	}
		
	prebegin = ssbegin;

	return;
	
}
extern hal_image_status_cb g_status_cb;
/**< A.1.1.4.1.2 */	
void* recv_parse_cv22_data(void* args)
{
	uint8_t img_type = 0;
	int frame_type = 0, ret = 0;
	uint32_t error_counter = 0;
	char filename[64];
	double delta = 0; 
	ssize_t rcount = 0; size_t to_recv = 0;
	size_t image_slen = 0, gps_len = 0, imu_len = 0, odo_len = 0;
	size_t image_pos = 0, gps_pos = 0, imu_pos = 0, odo_pos = 0;
	uint32_t image_num = 0, imu_num = 0, odo_num = 0, gps_num = 0;
	bool any_slice = false, last_slice = false; ///< any or last slice
	uint64_t times = 0, timeu = 0, timespec = 0;
	volatile bool rcvd_head = false;
	struct CV224Cam* cam = (struct CV224Cam*)args;
	struct net* client = (struct net*)cam->config->net;

	uint8_t *pimage = (uint8_t *)malloc(IMG_SIZE);	
	memset(pimage, 0, IMG_SIZE);

	uint8_t *pgps =(uint8_t *) malloc(GPS_SIZE);
	memset(pgps, 0, GPS_SIZE);

	uint8_t *pimu =(uint8_t *) malloc(IMU_SIZE);
	memset(pimu, 0, IMU_SIZE);

	uint8_t *podo =(uint8_t *) malloc(ODO_SIZE);
	memset(podo, 0, ODO_SIZE);

	uint8_t *rbuf =(uint8_t *) malloc(BUF_SIZE);	

	/**< A.1.1.4.1.2.1 */	
	if ((ret = client->connect_server(client)) ) {
		//g_status_cb("connect server error", &ret);
		goto exit;	
	}
	
	log_info("enter recv_parse_cv22_data loop: %s, run = %s", 
			strerror(errno), cam->run?"true":"false");
	while (!get_exit_flag() && cam->run) {
		rcount = 0;
		to_recv = 19; ///< head lenght is 19 bytes.
		any_slice = 0;

		do {
			rcount = (ssize_t)client->recv_data(client, rbuf, to_recv);
			if (rcount < 0) {
				if (errno == EINTR || errno == EAGAIN) {
					rcount = 0;
					zlog_info(g_zc, "read error: %s", strerror(errno));
					continue;
				} 
			} else if (rcount == 0) {
				log_info("recved count = 0: %s", strerror(errno));
				//client->close(client);
				ret = HAL_IMAGE_RET_ECONREF;
				//g_status_cb("recved count is zero", &ret);
				goto exit;
			} 

			if (rcvd_head == false) { ///< recv and parse frame head first!
				if ((rbuf[0] == 0xff) && (rbuf[1] == 0xd8) && (rbuf[2] == 0xd9)) {
					rcvd_head = true;
					frame_type = TYPE_IMAGE;
					image_slen = rbuf[7];
					image_slen = (image_slen << 8) + rbuf[6];
					image_slen = (image_slen << 8) + rbuf[5];
					image_slen = (image_slen << 8) + rbuf[4];

					to_recv = 18 + image_slen - (size_t)rcount; ///< real image length + 21 - rcount,  image_slen = 1 + 2 + real image len

					times = rbuf[11];
					times = (times << 8) + rbuf[10];
					times = (times << 8) + rbuf[9];
					times = (times << 8) + rbuf[8];
					
					timeu = rbuf[15];
					timeu = (timeu << 8) + rbuf[14];
					timeu = (timeu << 8) + rbuf[13];
					timeu = (timeu << 8) + rbuf[12];
					timespec = times*1000000 + timeu;

					img_type = rbuf[16];

					if ((rbuf[17] == 0xff) && (rbuf[18] == 0xd8)) {
						any_slice = true;
					} else if ((rbuf[17] == 0xff) && (rbuf[18] == 0xd9)) {
						any_slice = true;
						last_slice = true;
					} else {
						head_dump(rbuf, 19, to_recv);
						error_counter++;
					}
				} else if ((rbuf[0] == 0xff) && (rbuf[1] == 0x01) && (rbuf[2] == 0xAA) && (rbuf[3] == 0x88)) {
					rcvd_head = true;
					frame_type = TYPE_GPS;
					gps_len = rbuf[7];
					gps_len = (gps_len << 8) + rbuf[6];
					gps_len = (gps_len << 8) + rbuf[5];
					gps_len = (gps_len << 8) + rbuf[4];

					memcpy(pgps+gps_pos, &rbuf[0], (size_t)rcount);
					gps_pos += (size_t)rcount;

					to_recv = 19 + gps_len - (size_t)rcount; ///< real gps length + 19 - rcount
				} else if ((rbuf[0] == 0xff) && (rbuf[1] == 0x01) && (rbuf[2] == 0xAA) && (rbuf[3] == 0x66)) {
					rcvd_head = true;
					frame_type = TYPE_IMU;
					imu_len = rbuf[7];
					imu_len = (imu_len << 8) + rbuf[6];
					imu_len = (imu_len << 8) + rbuf[5];
					imu_len = (imu_len << 8) + rbuf[4];

					memcpy(pimu+imu_pos, &rbuf[0], (size_t)rcount);
					imu_pos += (size_t)rcount;

					to_recv = 19 + imu_len - (size_t)rcount; ///< real imu length + 19 - rcount
				} else if ((rbuf[0] == 0xff) && (rbuf[1] == 0x01) && (rbuf[2] == 0xAA) && (rbuf[3] == 0x77)) {
					rcvd_head = true;
					frame_type = TYPE_ODO;
					odo_len = rbuf[7];
					odo_len = (odo_len << 8) + rbuf[6];
					odo_len = (odo_len << 8) + rbuf[5];
					odo_len = (odo_len << 8) + rbuf[4];

					memcpy(podo+odo_pos, &rbuf[0], (size_t)rcount);
					odo_pos += (size_t)rcount;

					to_recv = 19 + odo_len - (size_t)rcount; ///< real odo length + 19 - rcount
				} else {
					zlog_info(g_zc, "unexpected head[0,1,2]: ");
					head_dump(rbuf, 3, to_recv);
					error_counter++;
					continue;
				}
			} else {
				switch (frame_type) {
					case TYPE_IMAGE:
						to_recv -= (size_t)rcount;
						if (any_slice == true) {
							memcpy(pimage+image_pos, &rbuf[0], (size_t)rcount);
							image_pos += (size_t)rcount;

							if (to_recv == 0) { ///< received a complete slice
								image_pos -= 2; ///< substract 2 bytes checksum
								rcvd_head = false;
								any_slice = false;

								if (last_slice == true) { ///< received a complte image
									frame_type = 0;
									last_slice = false;
									sprintf(filename, "image/%16ld.jpg", timespec);
									struct datapack* pdata = construct_datapack(NULL, pimage, image_pos);
									if (!pdata) {
										ret = HAL_IMAGE_RET_EALLOC;
										goto exit;
									}
									pdata->set_dts(pdata, timespec);
									pdata->set_type(pdata, img_type);

									ret = cam->link->upload(cam->link, pdata);
									if (ret) {
										pdata->release((void**)&pdata);
										//g_status_cb((char*)"allocate memory error", &ret);
										goto exit;
									}
									//persistence(filename, pimage, image_pos, O_CREAT|O_WRONLY);
									image_pos = 0;
									++image_num;
									total_gaps(15, &delta, image_num, imu_num, gps_num, odo_num, error_counter, args);
								}
							}
						} 
						break;
					case TYPE_GPS:
						memcpy(pgps+gps_pos, &rbuf[0], (size_t)rcount);
						to_recv -= (size_t)rcount;
						gps_pos += (size_t)rcount;
						if (to_recv == 0) {
							rcvd_head = false;
							gps_num++;
							parse_persistence(TYPE_GPS, pgps+DATA_POS, gps_len);
							memset(pgps, 0, GPS_SIZE);
							gps_pos = 0;
						}
						break;
					case TYPE_IMU:
						memcpy(pimu+imu_pos, &rbuf[0], (size_t)rcount);
						to_recv -= (size_t)rcount;
						if (to_recv == 0) {
							rcvd_head = false;
							imu_num++;
							parse_persistence(TYPE_IMU, pimu+DATA_POS, imu_len);
							memset(pimu, 0, IMU_SIZE);
							imu_pos = 0;
						} else {
							imu_pos += (size_t)rcount;
						}
						break;
					case TYPE_ODO:
						memcpy(podo+odo_pos, &rbuf[0], (size_t)rcount);
						to_recv -= (size_t)rcount;
						if (to_recv == 0) {
							rcvd_head = false;
							odo_num++;
							parse_persistence(TYPE_ODO, podo+DATA_POS, odo_len);
							memset(podo, 0, ODO_SIZE);
							odo_pos = 0;
						} else {
							odo_pos += (size_t)rcount;
						} 
						break;
					default:
						break;
				} ///< end switch
			}
		} while (to_recv > 0);
	} 
exit:
	//set_exit_flag();
	free(pimage);
	free(pimu);
	free(pgps);
	free(podo);
	free(rbuf);

	client->close(client);
	zlog_info(g_zc, "recv thread [%d %s:%d] exit!",
			client->fd,
			client->get_ip(client), client->get_port(client));

	pthread_exit((void*)(intptr_t)ret);

	return NULL;
}



