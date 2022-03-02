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


/*
默认配置
*/
#include "libSMGS_DefaultConfig.h"


/*
====================================================================================================
==========================================MQTT消息主题相关==========================================
====================================================================================================
*/

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
    SMGS_TOPIC_PLY_END=SMGS_TOPIC_NUMBER_OF_PLIES,/**< 枚举结束,没有实际意义,通常用于创建数组,通常等于SMGS_TOPIC_NUMBER_OF_PLIES*/
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



/*
====================================================================================================
==========================================MQTT消息负载相关==========================================
====================================================================================================
*/


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
    SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID,/**< 资源不存在 */

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


/*
====================================================================================================
==========================================设备相关==================================================
====================================================================================================
*/


/** \brief 判断是否是内部命令ID,如果是内部命令,由本协议栈处理,否则调用回调函数
 *
 * \param x 设备命令ID
 * \return 是否是内部命令ID
 *
 */
#define IS_SMGS_DEVICE_INTERNAL_CMDID(x) (((CONFIG_SMGS_DEVICE_INTERNAL_CMDID_1_START-(long)1)<((long)(x))) && ((CONFIG_SMGS_DEVICE_INTERNAL_CMDID_1_STOP+(long)1)>((long)(x))))


enum
{
    SMGS_DEVICE_CMDID_QUERY_DEVICENAME=0,/**< 查询设备名称 */
    SMGS_DEVICE_CMDID_QUERY_DEVICESERIALNUMBER,/**< 查询设备序列号 */
    SMGS_DEVICE_CMDID_QUERY_ISONLINE,/**< 查询设备是否在线 */
};


/** \brief 判断是否是内部设备寄存器地址,如果是内部设备寄存器地址,由本协议栈处理，否则调用回调函数
 *
 * \param x 设备寄存器地址
 * \return 是否是内部设备寄存器地址
 *
 */
#define IS_SMGS_DEVICE_INTERNAL_REGISTER_ADDRESS(x) ((((CONFIG_SMGS_DEVICE_INTERNAL_REGISTER_1_START-(long)1)<((long)(x))) && ((CONFIG_SMGS_DEVICE_INTERNAL_REGISTER_1_STOP+(long)1)>((long)(x)))) || \
                                                     (((CONFIG_SMGS_DEVICE_INTERNAL_REGISTER_2_START-(long)1)<((long)(x))) && ((CONFIG_SMGS_DEVICE_INTERNAL_REGISTER_2_STOP+(long)1)>((long)(x)))))

typedef struct __SMGS_device_context_t
{
    const char * DeviceName;/**< 设备名称字符串 */
    const char * DeviceSerialNumber; /**< 序列号字符串,不可为空且长度满足设备序列号要求,否则表示设备不存在*/
    uint8_t      DevicePosNumber; /**< 位置号,0为无效值 */
    void *        UserPtr; /**< 用户指针,用户自行定义使用 */

    /*
    回调函数,需要根据功能用户自行实现。注意:这并不是给用户调用的，而是用于协议栈内部调用。
    */

    /** \brief 是否在线,当此回调为NULL时表示设备不存在
     *
     * \param ctx 设备上下文指针。
     * \return 是否在线
     *
     */
    bool (*IsOnline)(struct __SMGS_device_context_t *ctx);


    /** \brief 执行特殊命令,可为NULL
     *
     * \param ctx 设备上下文指针。
     * \param plies 主题分层后的数组,用户不可修改除命令参数以外的内容
     * \param cmdid 命令ID,用户可修改命令ID。
     * \param cmddata 命令数据。
     * \param cmddata_length 命令数据长度。
     * \param retbuff 命令数据返回缓冲。
     * \param retbuff_length 命令数据返回长度。将传入最大长度，用户需要修改为实际长度。
     * \param ret 命令返回值
     * \return 是否执行成功
     *
     */
    bool (*Command)(struct __SMGS_device_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_cmdid_t *cmdid,uint8_t *cmddata,size_t cmddata_length,uint8_t *retbuff,size_t *retbuff_length,SMGS_payload_retcode_t *ret);


    /** \brief 读寄存器,可为NULL
     *
     * \param ctx 设备上下文指针。
     * \param plies 主题分层后的数组,用户不可修改除命令参数以外的内容
     * \param addr 寄存器地址。
     * \param dat 数据。
     * \param flag 标志。
     * \return 是否执行成功。
     *
     */
    bool (*ReadRegister)(struct __SMGS_device_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag);

    /** \brief 写寄存器,可为NULL
     *
     * \param ctx 设备上下文指针。
     * \param plies 主题分层后的数组,用户不可修改除命令参数以外的内容
     * \param addr 寄存器地址。
     * \param dat 数据,用户可修改此数据。
     * \param flag 标志,用户可修改此数据。
     * \return 是否执行成功。
     *
     */
    bool (*WriteRegister)(struct __SMGS_device_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag);


    /** \brief 读传感器,可为NULL
     *
     * \param ctx 设备上下文指针。
     * \param plies 主题分层后的数组,用户不可修改除命令参数以外的内容
     * \param addr 传感器地址。
     * \param dat 数据。
     * \param flag 标志
     * \return 是否执行成功
     *
     */
    bool (*ReadSensor)(struct __SMGS_device_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_sensor_address_t addr,uint64_t *dat,SMGS_payload_sensor_flag_t *flag);



} SMGS_device_context_t; /**< 设备上下文 */

/** \brief 将设备上下文初始化,在填写设备上下文成员之前需要调用此函数
 *
 * \param ctx 设备上下文指针
 *
 */
void SMGS_Device_Context_Init(SMGS_device_context_t *ctx);


/** \brief 判断设备上下文是否有效
 *
 * \param ctx 设备上下文
 * \return 是否有效
 *
 */
bool SMGS_Is_Device_Context_OK(SMGS_device_context_t *ctx);

/*
====================================================================================================
==========================================网关相关==================================================
====================================================================================================
*/

/** \brief 判断是否是内部命令ID,如果是内部命令,由本协议栈处理,否则调用回调函数
 *
 * \param x 网关命令ID
 * \return 是否是内部命令ID
 *
 */
#define IS_SMGS_GATEWAY_INTERNAL_CMDID(x) (((CONFIG_SMGS_GATEWAY_INTERNAL_CMDID_1_START-(long)1)<((long)(x))) && ((CONFIG_SMGS_GATEWAY_INTERNAL_CMDID_1_STOP+(long)1)>((long)(x))))

enum
{
    SMGS_GATEWAY_CMDID_ONLINE=0,/**< 网关上线 */
    SMGS_GATEWAY_CMDID_OFFLINE, /**< 网关下线 */
    SMGS_GATEWAY_CMDID_QUERY_GATEWAYNAME,/**< 查询网关名称 */
    SMGS_GATEWAY_CMDID_QUERY_GATEWAYSERIALNUMBER,/**< 查询网关序列号 */
    SMGS_GATEWAY_CMDID_REPORT_DEVICETABLE_ONLINE,/**< 网关上线上报设备表 */
    SMGS_GATEWAY_CMDID_REPORT_DEVICETABLE_NEW,/**< 网关上报设备表变动 */
    SMGS_GATEWAY_CMDID_QUERY_DEVICETABLE,/**< 网关查询设备表 */
    SMGS_GATEWAY_CMDID_QUERY_DEVICETABLE_LIST,/**< 网关查询设备表列表 */
};


/** \brief 判断是否是内部网关寄存器地址,如果是内部网关寄存器地址,由本协议栈处理，否则调用回调函数
 *
 * \param x 网关寄存器地址
 * \return 是否是内部网关寄存器地址
 *
 */
#define IS_SMGS_GATEWAY_INTERNAL_REGISTER_ADDRESS(x) (((CONFIG_SMGS_GATEWAY_INTERNAL_REGISTER_1_START-(long)1)<((long)(x))) && ((CONFIG_SMGS_GATEWAY_INTERNAL_REGISTER_1_STOP+(long)1)>((long)(x))))

typedef struct __SMGS_gateway_context_t
{
    const char * GateWayName; /**< 网关名称 */
    const char * GateWaySerialNumber;/**< 网关序列号,不可为空且长度满足网关序列号要求 */
    void *        UserPtr; /**< 用户指针,用户自行定义使用 */

    /*
    网关模块回调函数,需要根据功能用户自行实现。注意:这并不是给用户调用的，而是用于协议栈内部调用。
    */

    /** \brief 执行特殊命令,可为NULL
     *
     * \param ctx 网关上下文指针。
     * \param cmdid 命令ID,用户可修改命令ID。
     * \param cmddata 命令数据。
     * \param cmddata_length 命令数据长度。
     * \param retbuff 命令数据返回缓冲。
     * \param retbuff_length 命令数据返回长度。将传入最大长度，用户需要修改为实际长度。
     * \param ret 命令返回值
     * \return 是否执行成功
     *
     */
    bool (*Command)(struct __SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_cmdid_t *cmdid,uint8_t *cmddata,size_t cmddata_length,uint8_t *retbuff,size_t *retbuff_length,SMGS_payload_retcode_t *ret);


    /** \brief 读寄存器,可为NULL
     *
     * \param ctx 网关上下文指针。
     * \param plies 主题分层后的数组,用户不可修改除命令参数以外的内容
     * \param addr 寄存器地址。
     * \param dat 数据。
     * \param flag 标志。
     * \return 是否执行成功。
     *
     */
    bool (*ReadRegister)(struct __SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag);

    /** \brief 写寄存器,可为NULL
     *
     * \param ctx 网关上下文指针。
     * \param plies 主题分层后的数组,用户不可修改除命令参数以外的内容
     * \param addr 寄存器地址。
     * \param dat 数据,用户可修改此数据。
     * \param flag 标志,用户可修改此数据。
     * \return 是否执行成功。
     *
     */
    bool (*WriteRegister)(struct __SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag);


    /** \brief 读传感器,可为NULL
     *
     * \param ctx 网关上下文指针。
     * \param plies 主题分层后的数组,用户不可修改除命令参数以外的内容
     * \param addr 传感器地址。
     * \param dat 数据。
     * \param flag 标志
     * \return 是否执行成功
     *
     */
    bool (*ReadSensor)(struct __SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],SMGS_payload_sensor_address_t addr,uint64_t *dat,SMGS_payload_sensor_flag_t *flag);


    /*
    设备模块回调函数,需要根据功能用户自行实现。注意:这并不是给用户调用的，而是用于协议栈内部调用。
    */


    /** \brief 获取下一个设备模块的上下文,可为NULL
     *
     * \param ctx 网关上下文指针。
     * \param devctx 当前设备的上下文指针,可为NULL
     * \return 设备上下文指针,可为NULL
     *
     */
    SMGS_device_context_t * (*Device_Next)(struct __SMGS_gateway_context_t *ctx,SMGS_device_context_t * devctx);


    /** \brief 通过序列号查找设备上下文,不可为NULL,但有通过Device_Next实现的默认函数
     *
     * \param ctx 网关上下文指针。
     * \param SerialNumber 序列号
     * \return 设备上下文指针,可为NULL
     *
     */
    SMGS_device_context_t * (*Device_Find_By_SerialNumber)(struct __SMGS_gateway_context_t *ctx,const char* SerialNumber);

    /** \brief 通过位置号查找设备上下文,不可为NULL,但有通过Device_Next实现的默认函数
     *
     * \param ctx 网关上下文指针。
     * \param Pos 位置号
     * \return 设备上下文指针,可为NULL
     *
     */
    SMGS_device_context_t * (*Device_Find_By_Pos)(struct __SMGS_gateway_context_t *ctx,uint8_t Pos);


    /*
    MQTT消息回调函数,需要根据功能用户自行实现。注意:这并不是给用户调用的，而是用于协议栈内部调用。
    */

    /** \brief 发布MQTT消息
     *
     * \param ctx 网关上下文指针。
     * \param topic 主题
     * \param payload 负载
     * \param payloadlen 负载长度
     * \param qos MQTT的QOS标志
     * \param retain Retain标志
     * \return 是否成功
     *
     */
    bool (*MessagePublish)(struct __SMGS_gateway_context_t *ctx,const char * topic,void * payload,size_t payloadlen,uint8_t qos,int retain);




} SMGS_gateway_context_t; /**< 网关上下文 */


/** \brief 网关上下文初始化
 *
 * \param  ctx 网关上下文指针。
 * \param  SerialNumber 序列号指针,此指针应当在网关上下文的整个生命周期内有效
 * \param  MessagePublish MQTT消息发布函数
 *
 */
void SMGS_GateWay_Context_Init(SMGS_gateway_context_t *ctx,const char *SerialNumber, bool (*MessagePublish)(SMGS_gateway_context_t *,const char *,void *,size_t,uint8_t,int ));


/** \brief 判断网关上下文是否满足最低要求
 *
 * \param ctx 网关上下文指针
 * \return 是否满足
 *
 */
bool SMGS_Is_GateWay_Context_OK(SMGS_gateway_context_t *ctx);

/** \brief 上报网关模块事件
 *
 * \param ctx 网关上下文
 * \param cmd_para_1 命令参数1,可为NULL
 * \param cmd_para_2 命令参数2,可为NULL
 * \param cmd_para_3 命令参数3,可为NULL
 * \param cmdid 命令ID
 * \param cmddata 命令附加数据,可为NULL
 * \param cmddata_length 命令附加数据长度,可为0
 * \param buff 缓冲
 * \param buff_size 缓冲长度,缓冲应该足够容纳主题加payload
 * \param qos MQTT的QOS标志
 * \param retain Retain标志
 * \return 是否成功执行
 *
 */
bool SMGS_GateWay_Send_GateWay_Event(SMGS_gateway_context_t *ctx,const char *cmd_para_1,const char * cmd_para_2,const char * cmd_para_3,SMGS_payload_cmdid_t cmdid,void *cmddata,size_t cmddata_length,uint8_t *buff,size_t buff_size,uint8_t qos,int retian);


/** \brief 网关上线,每次MQTT连接成功后用户应当调用此消息发送上线命令
 *
 * \param ctx 网关上下文
 * \param buff 缓冲
 * \param buff_size 缓冲长度,缓冲应该足够容纳主题加payload
 * \param qos MQTT的QOS标志
 * \param retain Retain标志
 * \return 是否成功执行
 *
 */
bool SMGS_GateWay_Online(SMGS_gateway_context_t *ctx,uint8_t *buff,size_t buff_size,uint8_t qos,int retian);


/** \brief 上报设备模块事件
 *
 * \param ctx 网关上下文
 * \param devctx 设备上下文
 * \param cmd_para_1 命令参数1,可为NULL
 * \param cmd_para_2 命令参数2,可为NULL
 * \param cmd_para_3 命令参数3,可为NULL
 * \param cmdid 命令ID
 * \param cmddata 命令附加数据,可为NULL
 * \param cmddata_length 命令附加数据长度,可为0
 * \param buff 缓冲
 * \param buff_size 缓冲长度,缓冲应该足够容纳主题加payload
 * \param qos MQTT的QOS标志
 * \param retain Retain标志
 * \return 是否成功执行
 *
 */
bool SMGS_GateWay_Send_Device_Event(SMGS_gateway_context_t *ctx,SMGS_device_context_t *devctx,const char *cmd_para_1,const char * cmd_para_2,const char * cmd_para_3,SMGS_payload_cmdid_t cmdid,void *cmddata,size_t cmddata_length,uint8_t *buff,size_t buff_size,uint8_t qos,int retian);

typedef struct
{
    const char *topic;/**< 主题 */
    void * payload;/**< 负载 */
    size_t payloadlen;/**< 负载长度 */
    uint8_t qos;/**< QOS */
    int ratain;/**< 保留消息 */

} SMGS_gateway_will_t; /**< 网关遗嘱 */


/** \brief MQTT网关接收消息,当有MQTT消息时必须调用此函数回应消息
 *
 * \param ctx 网关上下文
 * \param topic MQTT主题
 * \param topiclen MQTT主题长度
 * \param payload MQTT负载
 * \param payloadlen MQTT负载长度
 * \param qos MQTT的QOS标志
 * \param retain Retain标志
 * \param buff 缓冲
 * \param buff_size 缓冲长度
 * \return 是否执行成功
 *
 */
bool SMGS_GateWay_Receive_MQTT_MSG(SMGS_gateway_context_t *ctx,const char *topic,size_t topiclen,uint8_t *payload,size_t payloadlen,uint8_t qos,int retain,uint8_t *buff,size_t buff_size);


/** \brief 网关遗嘱编码
 *
 * \param ctx 网关上下文
 * \param will 遗嘱指针
 * \param buff 缓冲(实际数据将存放在缓冲中)
 * \param buff_size 缓冲大小
 * \return 是否执行成功
 *
 */
bool SMGS_GateWay_Will_Encode(SMGS_gateway_context_t *ctx,SMGS_gateway_will_t *will,uint8_t *buff,size_t buff_size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __libSMGS_h__
