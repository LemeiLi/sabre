#include "rx_main.h"

void parse_persistence(int type, void* data, size_t size)
{
	struct mcu_gps gps = {0}; 
	struct mcu_imu imu = {0}; 
	struct mcu_odo odo = {0}; 
	uint8_t ptxt[1024] = {0};
	size_t nbytes = 0;

	switch(type) {
		case TYPE_GPS:
			memcpy(&gps, data, size);
			//fprintf(stderr, "gps length %ld, timestamp: %lf\n", size, gps.timestamp);
			nbytes = snprintf(ptxt, 1024, 
				"%ld %14.10lf %14.10lf %8.3lf %7.3lf %7.3lf %7.3lf %6.3lf %7.3lf %7.3lf %02d %01d %02d",
				(uint64_t)(gps.timestamp*1000000),
				gps.pos[0],
				gps.pos[1],
				gps.pos[2],
				gps.vel[0],
				gps.vel[1],
				gps.vel[2],
				gps.hdop,
				gps.azi_track,
				gps.speed,
				gps.sat_num,
				gps.status,
				gps.gpgsv.sv_num);

			for (int i = 0; i < MCU_GPS_GSV; i++) {
				nbytes += snprintf(ptxt+nbytes, 1024-nbytes, " %03d ", gps.gpgsv.sv_id[i]);
			}
			for (int i = 0; i < MCU_GPS_GSV; i++) {
				nbytes += snprintf(ptxt+nbytes, 1024-nbytes, " %02d ", gps.gpgsv.sv_elv[i]);
			}
			for (int i = 0; i < MCU_GPS_GSV; i++) {
				nbytes += snprintf(ptxt+nbytes, 1024-nbytes, " %03d ", gps.gpgsv.sv_az[i]);
			}
			for (int i = 0; i < MCU_GPS_GSV; i++) {
				nbytes += snprintf(ptxt+nbytes, 1024-nbytes, " %02d ", gps.gpgsv.sv_cno[i]);
			}	
			nbytes += snprintf(ptxt+nbytes, 1024-nbytes, "\r\n");
			persistence("gps.txt", ptxt, nbytes, O_CREAT|O_WRONLY|O_APPEND);

			break;
		case TYPE_IMU:
			memcpy(&imu, data, size);
			//fprintf(stderr, "imu length %ld, timestamp: %lf\n", size, imu.timestamp);
			size_t nbytes = snprintf(ptxt, 1024, "%ld %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf\r\n",
				(uint64_t)(imu.timestamp*1000000),
				imu.acc[0],
			    imu.acc[1],
				imu.acc[2],                                                                                        
				imu.gyr[0],
				imu.gyr[1],
				imu.gyr[2]);

			persistence("imu.txt", ptxt, nbytes, O_CREAT|O_WRONLY|O_APPEND);
			break;
		case TYPE_ODO:
			memcpy(&odo, data, size);
			//fprintf(stderr, "odo length %ld, timestamp: %lf\n", size, odo.timestamp);
			nbytes = snprintf(ptxt, 1024, "%ld %05d %05d %05d %05d\r\n",
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

void head_dump(uint8_t* rbuf, size_t len, size_t to_recv) 
{
	uint64_t times = 0, timeu = 0, timespec = 0;
	fprintf(stdout, "head check error: ");
	for (int i = 0; i < len; i++)
		fprintf(stdout, "[%d] %x, ", i, rbuf[i]);
	fprintf(stdout, "\n");

	fprintf(stdout, "slice_index = %x\n", rbuf[3]);
	int image_slen = rbuf[7];
	image_slen = (image_slen << 8) + rbuf[6];
	image_slen = (image_slen << 8) + rbuf[5];
	image_slen = (image_slen << 8) + rbuf[4];
	fprintf(stdout, "image_slen = %d\n", image_slen);

	fprintf(stdout, "to_recv = %ld\n", to_recv);

	times = rbuf[11];
	times = (times << 8) + rbuf[10];
	times = (times << 8) + rbuf[9];
	times = (times << 8) + rbuf[8];
	
	timeu = rbuf[15];
	timeu = (timeu << 8) + rbuf[14];
	timeu = (timeu << 8) + rbuf[13];
	timeu = (timeu << 8) + rbuf[12];
	timespec = times*1000000 + timeu;
	fprintf(stdout, "timestamp = %ld\n", timespec);
	fprintf(stdout, "id = %x\n", rbuf[16]);
	fprintf(stdout, "slice flag = %x, %x\n", rbuf[17], rbuf[18]);

	return;
}

void* recv_thread(void* args)
{
	int frame_type, rcount, to_recv;
	uint32_t error_counter = 0;
	struct sockaddr_in srv_addr;
	char filename[20];
	double delta = 0;
	size_t image_slen, gps_len, imu_len, odo_len;
	size_t image_pos = 0, gps_pos = 0, imu_pos = 0, odo_pos = 0;
	uint32_t image_num = 0, imu_num = 0, odo_num = 0, gps_num = 0;
	bool any_slice = false, last_slice = false; ///< any or last slice
	uint64_t times = 0, timeu = 0, timespec = 0;
	volatile bool rcvd_head = false;

	uint8_t *pimage = malloc(IMG_SIZE);	
	memset(pimage, 0, IMG_SIZE);

	uint8_t *pgps = malloc(GPS_SIZE);
	memset(pgps, 0, GPS_SIZE);

	uint8_t *pimu = malloc(IMU_SIZE);
	memset(pimu, 0, IMU_SIZE);

	uint8_t *podo = malloc(ODO_SIZE);
	memset(podo, 0, ODO_SIZE);

	uint8_t *rbuf = malloc(BUF_SIZE);	

	int sock_fd = connect_server(args);
	if (sock_fd == -1) {
		goto exit;	
	}
	
	while (!get_ctrl_c()) {
		rcount = 0;
		to_recv = 19; ///< head lenght is 19 bytes.
		any_slice = 0;

		do {
			rcount = readn(sock_fd, rbuf, to_recv);
			if (rcount < 0) {
				if (errno == EINTR || errno ==EAGAIN) {
					rcount = 0;
					fprintf(stderr, "read error: %s\n", strerror(errno));
					continue;
				} 
			} else if (rcount == 0) {
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

					to_recv = 18 + image_slen - rcount; ///< real image length + 21 - rcount,  image_slen = 1 + 2 + real image len

					times = rbuf[11];
					times = (times << 8) + rbuf[10];
					times = (times << 8) + rbuf[9];
					times = (times << 8) + rbuf[8];
					
					timeu = rbuf[15];
					timeu = (timeu << 8) + rbuf[14];
					timeu = (timeu << 8) + rbuf[13];
					timeu = (timeu << 8) + rbuf[12];
					timespec = times*1000000 + timeu;

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

					memcpy(pgps+gps_pos, &rbuf[0], rcount);
					gps_pos += rcount;

					to_recv = 19 + gps_len - rcount; ///< real gps length + 19 - rcount
				} else if ((rbuf[0] == 0xff) && (rbuf[1] == 0x01) && (rbuf[2] == 0xAA) && (rbuf[3] == 0x66)) {
					rcvd_head = true;
					frame_type = TYPE_IMU;
					imu_len = rbuf[7];
					imu_len = (imu_len << 8) + rbuf[6];
					imu_len = (imu_len << 8) + rbuf[5];
					imu_len = (imu_len << 8) + rbuf[4];

					memcpy(pimu+imu_pos, &rbuf[0], rcount);
					imu_pos += rcount;

					to_recv = 19 + imu_len - rcount; ///< real imu length + 19 - rcount
				} else if ((rbuf[0] == 0xff) && (rbuf[1] == 0x01) && (rbuf[2] == 0xAA) && (rbuf[3] == 0x77)) {
					rcvd_head = true;
					frame_type = TYPE_ODO;
					odo_len = rbuf[7];
					odo_len = (odo_len << 8) + rbuf[6];
					odo_len = (odo_len << 8) + rbuf[5];
					odo_len = (odo_len << 8) + rbuf[4];

					memcpy(podo+odo_pos, &rbuf[0], rcount);
					odo_pos += rcount;

					to_recv = 19 + odo_len - rcount; ///< real odo length + 19 - rcount
				} else {
					fprintf(stderr, "unexpected head[0,1,2]: ");
					head_dump(rbuf, 3, to_recv);
					error_counter++;
					continue;
				}
			} else {
				switch (frame_type) {
					case TYPE_IMAGE:
						to_recv -= rcount;
						if (any_slice == true) {
							memcpy(pimage+image_pos, &rbuf[0], rcount);
							image_pos += rcount;

							if (to_recv == 0) { ///< received a complete slice
								image_pos -= 2; ///< substract 2 bytes checksum
								rcvd_head = false;
								any_slice = false;

								if (last_slice == true) { ///< received a complte image
									frame_type = 0;
									last_slice = false;
									sprintf(filename, "image/%16ld.jpg", timespec);
									persistence(filename, pimage, image_pos, O_CREAT|O_WRONLY);
									image_pos = 0;
									++image_num;

									total_gaps(30, &delta, image_num, imu_num, gps_num, odo_num, error_counter, args);
								}
							}
						} 
						break;
					case TYPE_GPS:
						memcpy(pgps+gps_pos, &rbuf[0], rcount);
						to_recv -= rcount;
						gps_pos += rcount;
						if (to_recv == 0) {
							rcvd_head = false;
							gps_num++;
							parse_persistence(TYPE_GPS, pgps+DATA_POS, gps_len);
							memset(pgps, 0, GPS_SIZE);
							gps_pos = 0;
						}
						break;
					case TYPE_IMU:
						memcpy(pimu+imu_pos, &rbuf[0], rcount);
						to_recv -= rcount;
						if (to_recv == 0) {
							rcvd_head = false;
							imu_num++;
							parse_persistence(TYPE_IMU, pimu+DATA_POS, imu_len);
							memset(pimu, 0, IMU_SIZE);
							imu_pos = 0;
						} else {
							imu_pos += rcount;
						}
						break;
					case TYPE_ODO:
						memcpy(podo+odo_pos, &rbuf[0], rcount);
						to_recv -= rcount;
						if (to_recv == 0) {
							rcvd_head = false;
							odo_num++;
							parse_persistence(TYPE_ODO, podo+DATA_POS, odo_len);
							memset(podo, 0, ODO_SIZE);
							odo_pos = 0;
						} else {
							odo_pos += rcount;
						} 
						break;
				} ///< end switch
			}
		} while (to_recv > 0);
	} 
exit:
	set_ctrl_c();
	free(pimage);
	free(pimu);
	free(pgps);
	free(podo);
	free(rbuf);

	close(sock_fd);
	fprintf(stdout, "recv thread [%s:%d] exit!\r\n", ((struct addr_map*)args)->ip, ((struct addr_map*)args)->port);
	pthread_exit(NULL);

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t recvA, recvB;
	struct addr_map map[2] = {{ .ip = SERVER_IP, .port = 5678,},
		                      { .ip = SERVER_IP, .port = 6789,}};
	signal(SIGINT, sigstop);
	signal(SIGPIPE, sigpipe);

	pthread_create(&recvA, NULL, recv_thread, &map[0]);
	//pthread_create(&recvB, NULL, recv_thread, &map[1]);

	while(!get_ctrl_c()) {
		sleep(2);
	}	

	pthread_join(recvA, NULL);
	//pthread_join(recvB, NULL);

	return 0;
}




