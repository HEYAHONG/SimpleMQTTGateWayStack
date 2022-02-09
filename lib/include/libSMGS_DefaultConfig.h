/** \file libSMGS_DefaultConfig.h
 *  \brief     简易MQTT网关协议栈默认配置
 *  \author    何亚红
 *  \version   20220208
 *  \date      2022
 *  \copyright MIT License.
 */

#ifndef __libSMGS_DefaultConfig_h__
#define __libSMGS_DefaultConfig_h__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"

/*
导入config.h的配置
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifndef CONFIG_SMGS_MAX_TOPIC_LENGTH
#define CONFIG_SMGS_MAX_TOPIC_LENGTH 128 /**< 主题最大长度 */
#endif // CONFIG_SMGS_MAX_TOPIC_LENGTH

#ifndef CONFIG_SMGS_TOPIC_MAX_NUMBER_OF_CMD_PARAMETERS
#define CONFIG_SMGS_TOPIC_MAX_NUMBER_OF_CMD_PARAMETERS 3 /**< 主题最大命令参数个数 */
#endif // CONFIG_SMGS_TOPIC_MAX_NUMBER_OF_CMD_PARAMETERS

#ifndef SMGS_TOPIC_NUMBER_OF_PLIES
#define SMGS_TOPIC_NUMBER_OF_PLIES (CONFIG_SMGS_TOPIC_MAX_NUMBER_OF_CMD_PARAMETERS+5) /**< 主题消息层数 */
#endif // SMGS_TOPIC_NUMBER_OF_PLIES

#ifndef CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH
#define CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH 16 /**< 网关序列号的最小长度 */
#endif // CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __libSMGS_DefaultConfig_h__
