/**
 * @copyright Copyright (c) [2018 - 2019] MOMENTA Incorporated. All rights reserved.
 *
 * @author    XXDK, v.manstein@qq.com
 *
 * @date      Thu 07 Nov 2019 07:43:17 PM CST
 *
 * @file  uil_insgps.h
 *
 * @brief 该文件提供HAL INSGPS模块的接口函数以及相应的结构体变量类型
 *
 * @details <h2 style="color:Blue;">使用方法简介：</h2>
 * - 1、初始化使用hal_insgps_init，输入参数配置文件及句柄指针（不需申请空间）；
 *      配置文件格式参考insgps JSON配置文档；
 *      注意：数据缓存区在HAL层申请，应用不需要申请。
 * - 2、被动方式获取数据，使用hal_insgps_register_data_callback设置回调函数(推荐);
 *      被动方式获取状态，使用hal_insgps_register_status_callback设置回调函数(推荐);
 *      主动方式获取数据，使用hal_insgps_pop_data读取，注不需要申请数据帧空间。
 * - 3、应用运行过程中，使用hal_insgps_get获取状态及读取参数，注JSON格式；
 *      使用hal_insgps_set调整参数，注JSON格式。
 * - 4、被动接收方式，若需要暂停及恢复insgps数据获取工作，使用hal_insgps_stop和hal_insgps_start
 * - 5、注销及关闭insgps，使用hal_insgps_deinit，释放资源。
 * @addtogroup INSGPS
 * @{
 */

#ifndef _UIL_INSGPS_H
#define _UIL_INSGPS_H

#include <stdint.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
*  HAL insgps macros
*******************************************************************************/
/**
 * @name Macro Constants
 * @{
 */
/**
 * @brief 表示程序执行正确
 */
#define HAL_INSGPS_RET_OK                (0)
/**
 * @brief 表示发生了丢帧
 */
#define HAL_INSGPS_RET_EFRAMEDROP        (-1) ///< frame drop
/**
 * @brief 表示串口初始化错误
 */
#define HAL_INSGPS_RET_ESERINIT          (-2) ///< serial init error 
/**
 * @brief 表示内存错误
 */
#define HAL_INSGPS_RET_EALLOC            (-3) ///< memory error
/**
 * @brief 表示日志配置错误
 */
#define HAL_INSGPS_RET_ELOGCONF          (-4) ///< log config error
/**
 * @brief 表示INSGPS配置错误
 */
#define HAL_INSGPS_RET_ECONF             (-5) ///< insgps config file error
/**
 * @brief 预留 
 */
#define HAL_INSGPS_RET_UNKNOWN           (-30)
/** @} */  

/*******************************************************************************
 * Data structures and types
 *******************************************************************************/
/**
 * @brief 定义HAL INSGPS 句柄，定义为uint32_t
 */
typedef uint32_t hal_insgps_t;

/**
 * @brief 定义msg_t类型，定义为uint32_t
 */
typedef int32_t msg_t;

/**
 * @brief 定义hal_insgps_data_t数据结构体变量类型
 * @note hal_insgps_data_t的数据空间由HAL层申请, 
 * 用户使用完毕后需要释放pvBuf 和该数据本身
 */
typedef struct _hal_insgps_data_t
{
    /**
     * @brief insgps 数据时间戳(没有使用)
     */
    struct timeval  stTimeStamp;       
	/**
     * @brief insgps 数据长度
     */
    unsigned int    uiLen;            
	/**
     * @brief insgps 数据空间指针
     */
    void            *pvBuf;          
	/**
	 * @brief insgps 私有数据，此处用于返回数据类型
	 */
    void            *priv;
}hal_insgps_data_t;

/**
 * @brief 定义用户获取INSGPS数据的回调函数类型
 * @param[out] pstInsgpsData 由 hal_insgps_data_t 结构体指针带出数据
 * @param[out] pstate 由 msg_t指针，带出数据状态
 */
typedef void (*hal_insgps_data_cb)(hal_insgps_data_t *pstInsgpsData, 
	msg_t* pstate);

/**
 * @brief 定义用户获取状态的回调函数类型
 * @param[out] pcStatusTagJson 返回的状态，字符串表示，JSON格式
 * @param[out] pstate 由 msg_t指针，带出异常状态
 */
typedef void (*hal_insgps_status_cb)(const char *pcStatusTagJson, msg_t* pstate);

/*******************************************************************************
 * External declarations. 
 *******************************************************************************/
/**
 * @brief  hal层insgps sensor库，版本号输出
 *
 * @return 版本号字符串指针
 */
const char *hal_insgps_version(void);

/**
 * @brief insgps hal层初始化, 配置初始化参数
 *
 * @note  配置文件的格式为JSON，可以一次性配置多个insgps sensor，
 * *pphInsgpsHandle是第一个insgps句柄的指针，数量由返回值确定;
 * 默认接收方式是回调获取数据，可以通过hal_insgps_register_callback进行设置
 *
 * @param[out] pphInsgpsHandle 带出打开设备的句柄
 * @param[in] pcCfgFileName 初始化insgps的配置文件
 *
 * @return 成功返回打开的insgps设备数量, 失败返回<0
 * @retval > 0                      设备数量 
 * @retval HAL_INSGPS_RET_ECONF     INSGPS配置错误
 * @retval HAL_INSGPS_RET_ELOGCONF  日志配置错误
 * @retval HAL_INSGPS_RET_ESERINIT  串口初始化错误
 */
msg_t hal_insgps_init(hal_insgps_t **pphInsgpsHandle, const char *pcCfgFileName);

/**
 * @brief insgps hal层反初始化, 关闭设备，回收系统资源
 *
 * @param[in] phInsgpsHandle 要反初始化的设备句柄
 *
 * @return 总是成功返回 0 
 */
msg_t hal_insgps_deinit(hal_insgps_t* phInsgpsHandle);

/**
 * @brief 应用层注册数据回调函数接口，设置后hal层会启动线程接收数据，调用回调函数返回数据/状态
 * @note: hal_insgps_data_t 的数据空间由HAL层申请
 *
 * @param[in] phInsgpsHandle insgps接口句柄
 * @param[in] pfInsgpsDataCb 调用者自己实现的callback函数指针
 * @param[in] user          保留字段，可以作为多次回调注册的区别参数
 *
 * @return 总是成功返回 0 
 */
msg_t hal_insgps_register_data_callback(hal_insgps_t *phInsgpsHandle, 
	hal_insgps_data_cb pfInsgpsDataCb, void *user);


/**
 * @brief 应用层注册状态回调函数接口，设置后hal层将调用回调函数返回状态
 *
 * @param[in] phInsgpsHandle insgps接口句柄
 * @param[in] pfStatusCb    调用者自己实现的callback函数指针
 * @param[in] user          保留字段，可以作为多次回调注册的区别参数
 *
 * @return 总是成功返回 0 
 */
msg_t hal_insgps_register_status_callback(hal_insgps_t *phInsgpsHandle, 
	hal_insgps_status_cb pfStatusCb, void *user);

/**
 * @brief hal层暂停insgps sensor数据接收工作，注，没有释放资源
 *
 * @note: 接口没有释放申请的资源，关闭请使用hal_insgps_deinit接口
 *
 * @param[in] phInsgpsHandle insgps接口句柄
 * @param[in] ucStopMode 暂停模式 0 表示不中断接收数据到缓存队列, 1 表示中断接收数据，不再向缓存队列存入数据，但缓存队列未清空
 *
 * @return 总是成功返回 0 
 */
msg_t hal_insgps_stop(hal_insgps_t *phInsgpsHandle, uint8_t ucStopMode);

/**
 * @brief hal层恢复insgps数据接收工作，注，hal_insgps_stop对应的恢复操作
 *
 * @param[in] phInsgpsHandle insgps接口句柄
 * @param[in] ucStartMode  0 表示不清空缓存队列，在现有缓存队列上开始接收数据, 1 表示先清空缓存队列，重新接收数据，可以保证数据最新
 *
 * @return 成功返回 0, 失败返回 < 0
 * @retval HAL_INSGPS_RET_OK        成功
 * @retval HAL_INSGPS_RET_ESERINIT  串口初始化错误
 */
msg_t hal_insgps_start(hal_insgps_t *phInsgpsHandle, uint8_t ucStartMode);

/**
 * @brief 向nsgps注入RTK数据, 该函数只能在初始化完成后调用
 *
 * @param[in] phInsgpsHandle insgps接口句柄
 * @param[in] pcRtkJson      RTK数据首地址 
 * @param[in] size           RTK数据长度 
 *
 * @return 总是成功返回 0
 */
msg_t hal_insgps_rtk(hal_insgps_t *phInsgpsHandle, char *pcRtkJson, size_t size);

/**
 * @brief 从HAL层读取数据buffer
 *
 * @note  hal_insgps_data_t的数据空间由HAL层申请
 *
 * @param[in]  phInsgpsHandle insgps接口句柄
 * @param[in]  iWaitTimeMS   -1 等待直到获得数据; 0 尝试一次数据获取; > 0 等待对应的时间直到超时
 * @param[in]  user          保留字段，可以作为多个设备的区别
 * @param[out] ppstInsgpsData  读取insgps的数据指针，注：应用层定义指针即可，不需要申请hal_insgps_data_t空间
 *
 * @return 成功返回 0, 失败返回 < 0
 */
msg_t hal_insgps_pop_data (hal_insgps_t *phInsgpsHandle, 
	hal_insgps_data_t **ppstInsgpsData, int iWaitTimeMS, void *user);

/**
 * @brief 向HAL层归还数据buffer
 *
 * @param[in] phInsgpsHandle insgps接口句柄
 * @param[in] ppstInsgpsData 待归还的insgps的数据指针
 * @param[in] user           保留字段，可以作为多个设备的区别
 *
 * @return 成功返回 0, 失败返回 < 0
 */
msg_t hal_insgps_push_data(hal_insgps_t *phInsgpsHandle, 
	hal_insgps_data_t **ppstInsgpsData, void *user);

/**
 * @brief 获取insgps sensor状态及各种参数
 *
 * @param[in] phInsgpsHandle insgps接口句柄
 * @param[in] pcGetTagJson   具体要查询的参数名，字符串表示，JSON格式；NULL代表返回所有状态
 * @param[out] pcValueJson   insgps状态及参数的返回值，字符串表示，JSON格式
 *
 * @return 成功返回0, 失败返回<0
 */
msg_t hal_insgps_get(hal_insgps_t *phInsgpsHandle, char *pcGetTagJson, 
	char *pcValueJson);

/**
 * @brief 设置insgps sensor状态及各种参数
 *
 * @param[in] phInsgpsHandle  insgps接口句柄
 * @param[in] pcSetTagJson   具体要设置的参数名，字符串表示，JSON格式
 *
 * @return 成功返回 0, 失败返回 < 0
 */
msg_t hal_insgps_set(hal_insgps_t *phInsgpsHandle, char *pcSetTagJson);

#ifdef __cplusplus
}
#endif

#endif // _UIL_INSGPS_H
/** @} */
