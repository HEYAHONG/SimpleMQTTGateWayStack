#ifndef LIBSMGSUTIL_H_INCLUDED
#define LIBSMGSUTIL_H_INCLUDED

/** \file libSMGSUtil.h
 *  \brief     简易MQTT网关协议栈工具头文件
 *  \author    何亚红
 *  \version   20220208
 *  \date      2022
 *  \copyright MIT License.
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"

typedef struct
{
    uint8_t *start; /**< 起始地址 */
    size_t total; /**< 总长度 */
    size_t used; /**< 已用长度 */
} SMGS_buff_t; /**< 缓冲区，此缓冲区只分配不回收,非线程安全 */

/** \brief 通过数组创建缓冲区
 *
 * \param buff uint8_t* 数组
 * \param size size_t 数组长度
 * \return SMGS_buff_t 缓冲区
 *
 */
SMGS_buff_t SMGS_build_buff(uint8_t *buff,size_t size);

/** \brief 从缓冲区里分配空间
 *
 * \param buff SMGS_buff_t 缓冲区
 * \param size size_t 待获取的空间大小,为0时表示获取剩余空间的首地址
 * \return uint8_t* 指针，失败返回NULL
 *
 */
uint8_t *SMGS_buff_alloc(SMGS_buff_t *buff,size_t size);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LIBSMGSUTIL_H_INCLUDED
