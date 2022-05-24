/**
 * @example test_insgps.c
 * @brief   INSGPS 数据测试用例
 * @author  XXDK, v.manstein@qq.com 
 * @date    Thu 8 Nov 2019 02:56:33 PM CST
 */

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "uil/uil_insgps.h"

static void data_cb(hal_insgps_data_t *data, msg_t* pstate)
{
	(void)pstate;
	fprintf(stdout, "type [%8s] data: addr <%p>, size %d\n",
			(char*)data->priv, data->pvBuf, data->uiLen);
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
	hal_insgps_t* handle;
	char* rtk = "RTK-TEST-0123456789012345678901234567890123456789";
	
	if (geteuid()) {
		fprintf(stderr, "run this demo as root, try: sudo %s!\n", argv[0]);
		return -1;
	}

    /**< 1. 初始化, 注册回调, 并开始 */
	ret = hal_insgps_init(&handle, "insgps.json");
	if (ret) {
		return -1;
	}
	hal_insgps_register_data_callback(handle, data_cb, NULL);
	hal_insgps_register_status_callback(handle, status_cb, NULL);
	fprintf(stdout, ">>>>>>>>>>>>>> get insgps sustain 5s\n");
	hal_insgps_rtk(handle, rtk, strlen(rtk));
	sleep(5000000);
	
    /**< 2. 停止 */
	fprintf(stdout, ">>>>>>>>>>>>>> stop sustain 2s\n");
	hal_insgps_stop(handle, 0);
	sleep(2);

    /**< 3. 开始 */
	fprintf(stdout, ">>>>>>>>>>>>>> start sustain 5s\n");
	hal_insgps_start(handle, 0);
	hal_insgps_rtk(handle, rtk, strlen(rtk));
	sleep(5);

	/**< 4. 退出 */
	fprintf(stdout, ">>>>>>>>>>>>>> deinit\n");
	hal_insgps_deinit(handle);

	return 0;
}

