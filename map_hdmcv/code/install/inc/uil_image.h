/**
 * @copyright Copyright (c) [2018 - 2019] MOMENTA Incorporated. All rights reserved.
 */
  
/**
 *
 * @file  uil_image.h
 *
 * @brief 该文件提供HAL IMAGE模块的接口函数以及相应的结构体变量类型
 *
 * @details <h2 style="color:Blue;">使用方法简介：</h2>
 * - 1、初始化使用hal_image_init，输入参数配置文件及句柄指针（不需申请空间）；
 *      配置文件格式参考image JSON配置文档；
 *      注意：数据缓存区在HAL层申请，应用不需要申请。
 * - 2、被动方式获取数据，使用hal_image_register_data_callback设置回调函数(推荐);
 *      被动方式获取状态，使用hal_image_register_status_callback设置回调函数(推荐);
 *      主动方式获取数据，使用hal_image_pop_data读取，注不需要申请数据帧空间。
 * - 3、应用运行过程中，使用hal_image_get获取状态及读取参数，注JSON格式；
 *      使用hal_image_set调整参数，注JSON格式。
 * - 4、被动接收方式，若需要暂停及恢复image工作，使用hal_image_stop和hal_image_start
 * - 5、注销及关闭image，使用hal_image_deinit，将申请的空间释放。
 * @addtogroup IMAGE
 * @{
 */
  
#ifndef __HAL_IMAGE_H__
#define __HAL_IMAGE_H__

#include <stdint.h>
#include <sys/time.h>
  
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*******************************************************************************
*  HAL image macros
*******************************************************************************/
/**
 * @name Macro Constants
 * @{
 */
/**
 * @brief 表示程序执行正确
 */
#define HAL_IMAGE_RET_OK                (0)
/**
 * @brief 表示发生了丢帧
 */
#define HAL_IMAGE_RET_EFRAMEDROP        (-1) ///< frame drop
/**
 * @brief 表示网络连接被拒绝，需要检查对端网络
 */
#define HAL_IMAGE_RET_ECONREF           (-2) ///< connection refused
/**
 * @brief 表示相机初始化错误
 */
#define HAL_IMAGE_RET_ECAMINIT          (-3) ///< camera init error 
/**
 * @brief 表示内存错误
 */
#define HAL_IMAGE_RET_EALLOC            (-4) ///< memory error
/**
 * @brief 表示日志配置错误
 */
#define HAL_IMAGE_RET_ELOGCONF          (-5) ///< log config error
/**
 * @brief 表示相机配置错误
 */
#define HAL_IMAGE_RET_ECAMCONF          (-6) ///< camera config error
/**
 * @brief 预留 
 */
#define HAL_IMAGE_RET_UNKNOWN           (-30) ///< reserved
/** @} */  

/*******************************************************************************
 * Data structures and types
 *******************************************************************************/
/**
 * @brief 定义HAL IMAGE 句柄，定义为uint32_t
 */
typedef uint32_t hal_image_t;

/**
 * @brief 定义msg_t类型，定义为uint32_t
 */
typedef int32_t msg_t;

/**
 * @brief 定义图像类型枚举标识
 * @enum image_type
 */
typedef enum image_type 
{
	IMG_TYPE_MJPEG_FRAME       = 0, /**< for jpg                    */
	IMG_TYPE_IDR_FRAME         = 1, /**< for H264 IDR frame         */
	IMG_TYPE_I_FRAME           = 2, /**< for H264 I frame           */ 
	IMG_TYPE_P_FRAME           = 3, /**< for H264 P frame           */
	IMG_TYPE_B_FRAME           = 4, /**< for H264 B frame           */
	IMG_TYPE_P_FAST_SEEK_FRAME = 5, /**< for H264 P fast seek frame */
	IMG_TYPE_END_FRAME
} image_type_t;
 
/**
 * @brief 定义hal_image_data_t数据结构体变量类型
 * @note hal_image_data_t的数据空间由HAL层申请, 
 * 用户使用完毕后需要释放pvBuf 和该数据本身
 */
typedef struct _hal_image_data_t
{
    /**
     * @brief image 数据时间戳
     */
    struct timeval  stTimeStamp;       
	/**
     * @brief image 数据长度
     */
    unsigned int    uiLen;            
	/**
     * @brief image 数据空间指针
     */
    void            *pvBuf;          
	/**
     * @brief image 图像数据类型
     */
	enum image_type type;
	/**
	 * @brief image 私有数据，预留
	 */
    void            *priv;
 } hal_image_data_t;

/**
 * @brief 定义用户获取图像数据的回调函数类型
 * @param[out] pstImageData 由 hal_image_data_t 结构体指针带出数据
 * @param[out] pstate 由 msg_t指针，带出数据状态
 */
typedef void (*hal_image_data_cb)(hal_image_data_t *pstImageData, msg_t* pstate);

/**
 * @brief 定义用户获取状态的回调函数类型
 * @param[out] pcStatusTagJson 返回的状态，字符串表示，JSON格式
 * @param[out] pstate 由 msg_t指针，带出异常状态
 */
typedef void (*hal_image_status_cb)(const char *pcStatusTagJson, msg_t* pstate);
   
/*******************************************************************************
 * External declarations. 
 *******************************************************************************/
/**
 * @brief  hal层image sensor库，版本号输出
 *
 * @return 版本号字符串指针
 */
const char *hal_image_version(void);
  
/**
 * @brief image hal层初始化, 配置初始化参数
 *
 * @note  配置文件的格式为JSON，可以一次性配置多个image sensor，
 * *pphImageHandle是第一个image句柄的指针，数量由返回值确定;
 * 默认接收方式是回调获取数据，可以通过hal_image_register_callback进行设置
 *
 * @param[out] pphImageHandle 带出打开设备的句柄
 * @param[in] pcCfgFileName 初始化image的配置文件
 *
 * @return 成功返回成功初始化打开的image设备数量, 失败返回<0
 * @retval > 0                    设备数量 
 * @retval HAL_IMAGE_RET_ECAMCONF CAMERA配置错误
 * @retval HAL_IMAGE_RET_ELOGCONF LOG配置错误
 * @retval HAL_IMAGE_RET_ECAMINIT CAMERA初始化错误
 */
msg_t hal_image_init(hal_image_t **pphImageHandle, const char *pcCfgFileName);

/**
 * @brief image hal层反初始化, 关闭设备，回收系统资源
 *
 * @param[in] phImageHandle 要反初始化的设备句柄
 *
 * @return 总是成功返回 0
 */
msg_t hal_image_deinit(hal_image_t *phImageHandle);
  
/**
 * @brief 应用层注册数据回调函数接口，设置后hal层会启动线程接收数据，调用回调函数返回数据/状态
 * @note: hal_image_data_t 的数据空间由HAL层申请
 *
 * @param[in] phImageHandle image接口句柄
 * @param[in] pfImageDataCb 调用者自己实现的callback函数指针
 * @param[in] user          保留字段，可以作为多次回调注册的区别参数
 *
 * @return 总是成功返回 0 
 */
msg_t hal_image_register_data_callback(hal_image_t *phImageHandle, hal_image_data_cb pfImageDataCb, void *user);

/**
 * @brief 应用层注册状态回调函数接口，设置后hal层将调用回调函数返回状态
 *
 * @param[in] phImageHandle image接口句柄
 * @param[in] pfStatusCb    调用者自己实现的callback函数指针
 * @param[in] user          保留字段，可以作为多次回调注册的区别参数
 *
 * @return 总是成功返回 0
 */
msg_t hal_image_register_status_callback(hal_image_t *phImageHandle, hal_image_status_cb pfStatusCb, void *user);
  
/**
 * @brief 从HAL层读取数据buffer
 *
 * @note  hal_image_data_t的数据空间由HAL层申请
 *
 * @param[in]  phImageHandle image接口句柄
 * @param[in]  iWaitTimeMS   -1 等待直到获得数据; 0 尝试一次数据获取; > 0 等待对应的时间直到超时
 * @param[in]  user          保留字段，可以作为多个设备的区别
 * @param[out] ppstImageData  读取image的数据指针，注：应用层定义指针即可，不需要申请hal_image_data_t空间
 *
 * @return 成功返回 0, 失败返回 < 0
 */
msg_t hal_image_pop_data (hal_image_t *phImageHandle, hal_image_data_t **ppstImageData, int iWaitTimeMS, void *user);

/**
 * @brief 向HAL层归还数据buffer
 *
 * @param[in] phImageHandle image接口句柄
 * @param[in] ppstImageData 待归还的image的数据指针
 * @param[in] user          保留字段，可以作为多个设备的区别
 *
 * @return 成功返回 0, 失败返回 < 0
 */
msg_t hal_image_push_data(hal_image_t *phImageHandle, hal_image_data_t **ppstImageData, void *user);
  
/**
 * @brief 获取image sensor状态及各种参数
 *
 * @param[in] phImageHandle  image接口句柄
 * @param[in] pcGetTagJson   具体要查询的参数名，字符串表示，JSON格式；NULL代表返回所有状态
 * @param[out] pcValueJson   image状态及参数的返回值，字符串表示，JSON格式
 *
 * @return 成功返回0, 失败返回<0
 */
msg_t hal_image_get(hal_image_t *phImageHandle, char *pcGetTagJson, char *pcValueJson);

/**
 * @brief 设置image sensor状态及各种参数
 *
 * @param[in] phImageHandle  image接口句柄
 * @param[in] pcSetTagJson   具体要设置的参数名，字符串表示，JSON格式
 *
 * @return 成功返回 0, 失败返回 < 0
 */
msg_t hal_image_set(hal_image_t *phImageHandle, char *pcSetTagJson);
  
/**
 * @brief hal层暂停image sensor数据接收工作，注，没有释放资源
 *
 * @note: 接口没有释放申请的资源，关闭请使用hal_image_deinit接口
 *
 * @param[in] phImageHandle image接口句柄
 * @param[in] ucStopMode 暂停模式 0 表示不中断接收数据到缓存队列, 1 表示中断接收数据，不再向缓存队列存入数据，但缓存队列未清空
 *
 * @return 总是成功返回 0
 */
msg_t hal_image_stop(hal_image_t *phImageHandle, uint8_t ucStopMode);
  
/**
 * @brief hal层恢复image数据接收工作，注，hal_image_stop对应的恢复操作
 *
 * @param[in] phImageHandle image接口句柄
 * @param[in] ucStartMode  0 表示不清空缓存队列，在现有缓存队列上开始接收数据, 1 表示先清空缓存队列，重新接收数据，可以保证数据最新
 *
 * @return 成功返回 0, 失败返回 < 0
 * @retval HAL_IMAGE_RET_OK       成功
 * @retval HAL_IMAGE_RET_ECAMINIT CAMERA初始化错误
 */
msg_t hal_image_start(hal_image_t *phImageHandle, uint8_t ucStartMode);
  
#ifdef __cplusplus
}
#endif // __cplusplus
  
#endif //__HAL_IMAGE_H__
/** @} */



