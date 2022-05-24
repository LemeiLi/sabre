/*************************************************************************
	> File Name: cv22_data.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Mon 21 Oct 2019 02:37:17 PM CST
 ************************************************************************/

#ifndef _CV22_DATA_H
#define _CV22_DATA_H

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

extern void* recv_parse_cv22_data(void* args);

#ifdef __cplusplus
}
#endif

#endif // _CV22_DATA_H
