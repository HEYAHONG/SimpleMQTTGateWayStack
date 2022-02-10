/** \file libSMGS.h
 *  \brief     简易MQTT网关协议栈头文件
 *  \author    何亚红
 *  \version   20220208
 *  \date      2022
 *  \copyright MIT License.
 */

#ifndef __libSMGS_h__
#define __libSMGS_h__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "libSMGS_DefaultConfig.h"


typedef const char *  SMGS_topic_string_ptr_t; /**< 主题字符串指针类型 */

/*
在解析主题时,将主题中的'/'字符替换成'\0'字符，这样就完成了对字符串的切割。
将主题字符串分为多个子字符串并将子字符串指针存储在一个数组中,通过数组下标访问各子字符串。
*/
typedef enum
{
    SMGS_TOPIC_PLY_DESTADDR=0,/**< 目的地址 */
    SMGS_TOPIC_PLY_SRCADDR,/**< 源地址 */
    SMGS_TOPIC_PLY_COMTYPE,/**< 通信类型 */
    SMGS_TOPIC_PLY_MODULE,/**< 模块 */
    SMGS_TOPIC_PLY_CMD,/**< 命令 */
    SMGS_TOPIC_PLY_CMD_PARA_1,/**< 命令参数1 */
    SMGS_TOPIC_PLY_CMD_PARA_2,/**< 命令参数2 */
    SMGS_TOPIC_PLY_CMD_PARA_3,/**< 命令参数3 */
    SMGS_TOPIC_PLY_END,/**< 枚举结束,没有实际意义,通常用于创建数组,通常等于SMGS_TOPIC_NUMBER_OF_PLIES*/
} SMGS_topic_plies_index_t; /**< 主题中各层的引索*/


/** \brief 主题解码
 *
 * \param plies[] 分层后的子字符串数组,若执行成功，则能得到正确的数组,当数组某个成员不存在，则会被置为NULL。
 * \param max_plies_count 子字符串数组最大大小,指示上一个数组的大小,通常大于等于SMGS_TOPIC_PLY_END。
 * \param buff 缓冲指针,最终实际数据将存放在缓冲中，仅当不再使用plies时才可释放buff。
 * \param buff_size 缓冲大小,需要大于等于topic_length+1。
 * \param topic 主题字符串指针
 * \param topic_length 主题字符串长度(不包含\0字符)
 * \return 是否执行成功
 *
 */
bool SMGS_Topic_Plies_Decode(SMGS_topic_string_ptr_t plies[],size_t max_plies_count,uint8_t *buff,size_t buff_size,const char * topic,size_t topic_length);


/** \brief 主题编码
 *
 * \param plies[] 分层后的子字符串数组,当遇到值为NULL的数组成员时会直接结束。
 * \param plies_count 子字符串数组大小
 * \param buff 缓冲指针,最终的数据将存放在缓冲中,仅当不再使用返回的主题时才可释放buff.
 * \param buff_size 缓冲大小,必须能够容纳所有子字符串。
 * \return 主题指针，失败时返回NULL
 *
 */
const char * SMGS_Topic_Plies_EnCode(SMGS_topic_string_ptr_t plies[],size_t plies_count,uint8_t *buff,size_t buff_size);


typedef enum
{
    SMGS_TOPIC_PLY_COMTYPE_BINREQ=0,/**< 二进制请求 */
    SMGS_TOPIC_PLY_COMTYPE_BINRESP,/**< 二进制回复 */
    SMGS_TOPIC_PLY_COMTYPE_BINEVT,/**< 二进制事件 */
    SMGS_TOPIC_PLY_COMTYPE_END /**< 枚举结束,没有实际意义,通常用于创建数组 */
} SMGS_topic_ply_comtype_t; /**< 通信类型枚举 */

extern const SMGS_topic_string_ptr_t  SMGS_topic_ply_comtype_string[]; /**< 通信类型对应的字符串,通常通过下标访问 */



/** \brief 通过通信类型枚举获取对应通信类型字符串
 *
 * \param index 通信类型枚举
 * \return 成功返回指针,失败返回NULL
 *
 */
SMGS_topic_string_ptr_t SMGS_Get_Topic_Ply_ComType_String(int index);


/** \brief 获取通信类型
 *
 * \param str 通信类型字符串
 * \return 返回通信类型枚举,失败时返回SMGS_TOPIC_PLY_COMTYPE_END
 *
 */
int SMGS_Get_Topic_Ply_ComType(SMGS_topic_string_ptr_t str);

typedef enum
{
    SMGS_TOPIC_PLY_MODULE_GATEWAY=0,/**< 网关模块 */
    SMGS_TOPIC_PLY_MODULE_DEVICE,/**< 设备模块 */
    SMGS_TOPIC_PLY_MODULE_END=SMGS_TOPIC_PLY_MODULE_DEVICE,/**< 枚举结束,没有实际意义,通常用于创建数组,由于默认为设备模块，故设备模块无需字符串 */
} SMGS_topic_ply_module_t; /**< 模块枚举 */

extern const SMGS_topic_string_ptr_t SMGS_topic_ply_module_string[]; /**< 模块对应的字符串 */


/** \brief 通过模块类型获取对应模块字符串,注意:设备模块不能使用此函数。
 *
 * \param index 模块枚举
 * \return 成功返回指针，失败返回NULL
 *
 */
SMGS_topic_string_ptr_t SMGS_Get_Topic_Ply_Module_String(int index);


/** \brief 获取模块
 *
 * \param str 模块字符串
 * \return 返回模块枚举,失败时返回SMGS_TOPIC_PLY_MODULE_END。注意:由于默认为设备模块,故失败时应当按设备模块处理。
 *
 */
int SMGS_Get_Topic_Ply_Module(SMGS_topic_string_ptr_t str);


typedef enum
{
    SMGS_TOPIC_PLY_CMD_COMMAND=0,/**< 特殊命令 */
    SMGS_TOPIC_PLY_CMD_READREGISTER,/**< 读单个寄存器 */
    SMGS_TOPIC_PLY_CMD_READMULTIREGISTER,/**< 读多个寄存器 */
    SMGS_TOPIC_PLY_CMD_WRITEREGISTER, /**< 写单个寄存器 */
    SMGS_TOPIC_PLY_CMD_WRITEMULTIREGISTER,/**< 写多个寄存器 */
    SMGS_TOPIC_PLY_CMD_READSENSOR,/**< 读单个传感器 */
    SMGS_TOPIC_PLY_CMD_READMULTISENSOR,/**< 读多个传感器 */
    SMGS_TOPIC_PLY_CMD_END,/**< 枚举结束,没有实际意义,通常用于创建数组 */
} SMGS_topic_ply_cmd_t;/**< 命令枚举 */


extern const SMGS_topic_string_ptr_t SMGS_topic_ply_cmd_string[]; /**< 命令对应的字符串 */


/** \brief 通过命令类型获取对应命令字符串。
 *
 * \param index 命令枚举
 * \return 成功返回指针，失败返回NULL
 *
 */
SMGS_topic_string_ptr_t SMGS_Get_Topic_Ply_CMD_String(int index);


/** \brief 获取命令
 *
 * \param str 命令字符串
 * \return 返回模块枚举,失败时返回SMGS_TOPIC_PLY_CMD_END。
 *
 */
int SMGS_Get_Topic_Ply_CMD(SMGS_topic_string_ptr_t str);


typedef uint8_t SMGS_payload_retcode_t;/**< Payload中返回值类型 */

enum
{
    SMGS_PAYLOAD_RETCODE_SUCCESS=0,/**< 命令执行成功 */
    SMGS_PAYLOAD_RETCODE_UNSUPPORTED_COMTYPE,/**< 通信类型不支持 */
    SMGS_PAYLOAD_RETCODE_UNKOWN_MODULE,/**< 模块不存在 */
    SMGS_PAYLOAD_RETCODE_MODULE_INVALID,/**< 模块存在但处于无效状态(离线) */
    SMGS_PAYLOAD_RETCODE_CMD_ERROR,/**< 命令/命令参数出现错误 */
    SMGS_PAYLOAD_RETCODE_CMD_PAYLOAD_ERROR,/**< 命令负载数据有误 */
    SMGS_PAYLOAD_RETCODE_CMD_DENIED,/**< 命令被拒绝 */
    SMGS_PAYLOAD_RETCODE_COM_ERROR,/**< 通信错误 */
    SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID,/**< 寄存器/传感器资源不存在 */

    SMGS_PAYLOAD_RETCODE_UNKOWN_ERROR=0xFF,/**< 未知错误  */

    SMGS_PAYLOAD_RETCODE_SPECIAL_CODE_START=0x80,/**< 具体命令的错误（起始） */
    SMGS_PAYLOAD_RETCODE_SPECIAL_CODE_STOP=0xFE,/**< 具体命令的错误（结束） */
};

typedef uint16_t SMGS_payload_register_address_t; /**< Payload中寄存器地址 */

typedef union
{
    struct
    {
        uint8_t reglen:2; /**< 寄存器长度。 0=1字节,1=2字节,2=4字节,3=8字节 */
        uint8_t resv0:1;/**< 保留 */
        uint8_t not_use_cache:1;/**< 是否不允许缓存。0=自动(由网关决定),1=不允许 */
        uint8_t resv1:3;/**< 保留 */
        uint8_t readonly:1;/**< 只读位,1=只读,0=可读写 */
    };
    uint8_t val;/**< 数值 */
} SMGS_payload_register_flag_t; /**< Payload中寄存器标志 */

typedef uint16_t SMGS_payload_sensor_address_t; /**< Payload中传感器地址 */


typedef union
{
    struct
    {
        uint8_t sensorlen:2;/**< 传感器数据长度。 0=1字节,1=2字节,2=4字节,3=8字节 */
        uint8_t resv0:1;/**< 保留 */
        uint8_t not_use_cache:1;/**< 是否不允许缓存。0=自动(由网关决定),1=不允许 */
        uint8_t divisor:3;/**< 定点数系数。 表示以10为底的幂。如0=系数为1，1=系数为10，2=系数为100 */
        uint8_t sign:1;/**< 符号位,1=有符号数,0=无符号数 */
    };
    uint8_t val;/**< 数值 */
} SMGS_payload_sensor_flag_t; /**< Payload中传感器标志 */

typedef uint16_t SMGS_payload_cmdid_t; /**< Payload中命令ID */



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __libSMGS_h__
