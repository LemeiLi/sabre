/*************************************************************************
	> File Name: cam_mcu.h
	> Author: Caesar
	> Email: v.manstein@qq.com 
	> Created Time: Sat 07 Sep 2019 07:33:05 AM PDT
 ************************************************************************/

#ifndef _CAM_MCU_H
#define _CAM_MCU_H

#include <stdint.h>

struct cam_data {
	uint8_t* pdata;
	size_t size;
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

struct mcu_data {
	double timestamp;
	int imu_count;
	int gps_count;
	int odo_count;
	struct mcu_imu imu[10];
	struct mcu_gps gps[10];
	struct mcu_odo odo[10];
};

#endif // _CAM_MCU_H
