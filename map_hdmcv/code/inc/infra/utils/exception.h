/*************************************************************************
	> File Name: exception.h
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Sat 16 Nov 2019 11:23:01 AM CST
 ************************************************************************/

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct emsg {
	char desc[64];
	int val;	
}emsg;

extern volatile int ecode;

#define HAL_OK               (0)

#define HAL_EALLOC           (-1)   ///< -1 ~ -127
#define HAL_EDEVCONF         (-2)
#define HAL_ELOGCONF         (-3)

#define HAL_EUDP             (-128) ///< -128 ~ -255
#define HAL_EUDPBIND         (-129)
#define HAL_EUDPRAW          (-130)

#define HAL_ETCP             (-256) ///< -256 ~ -383

#define HAL_ELIDAR           (-384) ///< -364 ~ -511

#define HAL_ESERIAL          (-512) ///< -512 ~ -639

#define HLA_EIMAGE           (-640)

extern emsg g_emsg;

#ifdef __cplusplus
}
#endif

#endif // _EXCEPTION_H
