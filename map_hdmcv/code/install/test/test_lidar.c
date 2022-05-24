/**
 * @example test_lidar.c
 * @brief   Lidar 数据测试用例
 * @author  XXDK, v.manstein@qq.com 
 * @date    Fri 22 Nov 2019 07:35:37 PM CST
 */

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pcap.h>
#include <string.h>
#include <stdint.h>
#include "uil/uil_lidar.h"

static pcap_t *pcapfile;
static pcap_dumper_t *pcap_dumper;

static void data_cb(hal_lidar_data_t *data, msg_t* pstate)
{
	(void)pstate;
	struct timeval cur_time;
	struct pcap_pkthdr header;

	gettimeofday(&cur_time, NULL);
	header.ts = cur_time;
	header.caplen = data->uiLen;
	header.len = data->uiLen;

	fprintf(stdout, "lidar data: addr <%p>, size %d, ts %ld\n",
	        data->pvBuf, data->uiLen,
			data->stTimeStamp.tv_sec*1000000 + data->stTimeStamp.tv_usec);
	pcap_dump((unsigned char*)pcap_dumper, &header, (const uint8_t *)data->pvBuf);

	free(data->pvBuf);
	free(data);
}

static void status_cb(const char *pcStatusTagJson, msg_t* pstate)
{
	fprintf(stdout, "info: %s, number %d\n", pcStatusTagJson, *pstate);
}

int main(int argc, const char* argv[])
{
	(void)argc;
	msg_t ret;
	hal_lidar_t* handle;

	if (geteuid()) {
		fprintf(stderr, "run this demo as root, try: sudo %s!\n", argv[0]);
		return -1;
	}

	/**< 1. 准备 pcap */
	pcapfile = pcap_open_dead(DLT_EN10MB, 65535) 	;
	pcap_dumper = pcap_dump_open(pcapfile, "lidar.pcap");
	if (!pcap_dumper) {
		pcap_geterr(pcapfile);
		pcap_close(pcapfile);

		return -1;
	}

    /**< 1. 初始化, 注册回调, 并开始接收lidar数据 */
	ret = hal_lidar_init(&handle, "lidar.json");
	if (ret) {
		return -1;
	}
	hal_lidar_register_data_callback(handle, data_cb, NULL);
	hal_lidar_register_status_callback(handle, status_cb, NULL);
	fprintf(stdout, ">>>>>>>>>>>>>> get data lidar sustain 20s\n");
	sleep(60);

    /**< 2. 停止收数据 */
	fprintf(stdout, ">>>>>>>>>>>>>> stop sustain 2s\n");
	hal_lidar_stop(handle, 0);
	sleep(2);

    /**< 3. 开始收数据 */
	fprintf(stdout, ">>>>>>>>>>>>>> start sustain 5s\n");

	hal_lidar_start(handle, 0);
	sleep(5);

	/**< 4. 退出 */
	hal_lidar_deinit(handle);
	pcap_close(pcapfile);

	return 0;
}



