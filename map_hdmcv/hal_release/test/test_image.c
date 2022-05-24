/**
 * @example test_image.c
 * @brief   图像数据测试用例
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
#include "uil_image.h"

static void data_cb(hal_image_data_t *data, msg_t* pstate)
{
	(void)pstate;
	fprintf(stdout, "image data[%u]:  addr <%p>, size %d, ts %ld\n",
			data->type, data->pvBuf, data->uiLen, 
			data->stTimeStamp.tv_sec*1000000 + data->stTimeStamp.tv_usec);

	free(data->pvBuf);
	free(data);
}

static void status_cb(const char *pcStatusTagJson, msg_t* pstate)
{
	fprintf(stdout, "info: %s, number %d\n", pcStatusTagJson, *pstate);
}

int main(void)
{
	msg_t ret;
	hal_image_t* handle;

    /**< 1. 初始化, 注册回调, 并开始收图 */
	ret = hal_image_init(&handle, "image.json");
	if (ret) {
		return -1;
	}
	hal_image_register_data_callback(handle, data_cb, NULL);
	hal_image_register_status_callback(handle, status_cb, NULL);
	fprintf(stdout, ">>>>>>>>>>>>>> get image sustain 5s\n");
	sleep(5);
	
    /**< 2. 停止收图 */
	fprintf(stdout, ">>>>>>>>>>>>>> stop sustain 2s\n");
	hal_image_stop(handle, 0);
	sleep(2);

    /**< 3. 开始收图 */
	fprintf(stdout, ">>>>>>>>>>>>>> start sustain 5s\n");
	hal_image_start(handle, 0);
	sleep(5);

	/**< 4. 退出 */
	hal_image_deinit(handle);

	return 0;
}



