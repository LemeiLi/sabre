/*************************************************************************
	> File Name: test_flc.c
	> Author: Caesar
	> Email: v.manstein@qq.com 
	> Created Time: Sat 07 Sep 2019 06:53:03 AM PDT
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "consumer/flc/flc.h"
#include "utils/utils.h"

int main()
{
	struct mcu_data mcu = {
		.imu_count = 6,
		.gps_count = 1,
		.odo_count = 5,
	};
	setup_flc_service();	
	while(!get_ctrl_c()) {
		handle_map_data_flc(NULL, &mcu);
		usleep(33*1000);
	}
	shutdown_flc_service();

	return 0;	
}

