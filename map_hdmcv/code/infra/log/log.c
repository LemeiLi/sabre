/*************************************************************************
	> File Name: log.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Fri 01 Nov 2019 10:19:40 AM CST
 ************************************************************************/

#include <stdio.h>
#include "infra/log/log.h"
#include "3dparty/zlog/zlog.h"


zlog_category_t *g_zc = NULL;

int log_init(const char* conf_path)
{
	if (g_zc) return 0;
	int rc = zlog_init(conf_path);
	if (rc) {
		fprintf(stderr, "zlog init failed\n");
		return -1;
	}

	g_zc = zlog_get_category("C_INFO");
	if (!g_zc) {
		fprintf(stderr, "zlog get category failed\n");
		zlog_fini();
		return -2;
	}

	zlog_info(g_zc, "log init success.");
	//zlog_error(g_zc, "log init success.");
	return 0;
}

void log_fini(void)
{
	zlog_fini();
}


