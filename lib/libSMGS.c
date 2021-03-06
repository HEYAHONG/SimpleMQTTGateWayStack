/** \file libSMGS.c
 *  \brief     简易MQTT网关协议栈C源代码
 *  \author    何亚红
 *  \version   20220208
 *  \date      2022
 *  \copyright MIT License.
 */


#include "libSMGS.h"


bool SMGS_Topic_Plies_Decode(SMGS_topic_string_ptr_t plies[],size_t max_plies_count,SMGS_buff_t *Buff,const char * topic,size_t topic_length)
{
    //参数检查
    if(plies==NULL || max_plies_count==0 || Buff ==NULL || topic==NULL || topic_length == 0)
    {
        return false;
    }

    uint8_t *buff=SMGS_buff_alloc(Buff,topic_length+1);

    if(buff==NULL)
    {
        return false;
    }

    memset((char *)buff,0,topic_length+1);
    memset(plies,0,max_plies_count);
    memcpy((char *)buff,topic,topic_length);

    size_t current_index=0;

    plies[current_index]=(const char *)buff;

    current_index++;

    for(size_t i=0; i<topic_length; i++)
    {
        if(buff[i]=='\0')
        {
            return true;
        }
        if(buff[i]=='/')
        {
            //将'/'替换成'\0'
            buff[i]='\0';

            plies[current_index]=(const char *)&buff[i+1];

            current_index++;

            if(current_index >= max_plies_count)
            {
                return true;
            }
        }

    }

    return true;
}



const char * SMGS_Topic_Plies_EnCode(SMGS_topic_string_ptr_t plies[],size_t plies_count,SMGS_buff_t *Buff)
{
    const char * ret=NULL;

    if(plies==NULL || plies[0]==NULL || plies_count == 0 || Buff == NULL)
    {
        return ret;
    }

    uint8_t *buff=NULL;

    //检查buff的大小是否足够
    {
        size_t need_buff_size=0;
        for(size_t i=0; i<plies_count; i++)
        {
            if(plies[i]==NULL)
            {
                break;
            }
            need_buff_size+=(strlen(plies[i])+1);
        }

        buff=SMGS_buff_alloc(Buff,need_buff_size);

        if(buff==NULL)
        {
            return ret;
        }

        memset(buff,0,need_buff_size);
    }

    //填写字符串
    {
        strcat((char *)buff,plies[0]);

        ret=(const char *)buff;

        for(size_t i=1; i<plies_count; i++)
        {
            if(plies[i]==NULL)
            {
                break;
            }

            strcat((char *)buff,"/");

            strcat((char *)buff,plies[i]);
        }
    }

    return ret;
}


const SMGS_topic_string_ptr_t  SMGS_topic_ply_comtype_string[SMGS_TOPIC_PLY_COMTYPE_END]=
{
    "BRQ",
    "BRP",
    "BE",
};


SMGS_topic_string_ptr_t SMGS_Get_Topic_Ply_ComType_String(int index)
{
    if(index>=0 && index < SMGS_TOPIC_PLY_COMTYPE_END)
    {
        return SMGS_topic_ply_comtype_string[index];
    }
    else
    {
        return NULL;
    }
}


int SMGS_Get_Topic_Ply_ComType(SMGS_topic_string_ptr_t str)
{

    if(str==NULL)
    {
        return SMGS_TOPIC_PLY_COMTYPE_END;
    }

    for(size_t i=0; i<SMGS_TOPIC_PLY_COMTYPE_END; i++)
    {
        if(strcmp(SMGS_topic_ply_comtype_string[i],str)==0)
        {
            return i;
        }
    }

    return SMGS_TOPIC_PLY_COMTYPE_END;
}


const SMGS_topic_string_ptr_t SMGS_topic_ply_module_string[SMGS_TOPIC_PLY_MODULE_END]=
{
    "GW",
};

SMGS_topic_string_ptr_t SMGS_Get_Topic_Ply_Module_String(int index)
{
    if(index>=0 && index < SMGS_TOPIC_PLY_MODULE_END)
    {
        return SMGS_topic_ply_module_string[index];
    }
    else
    {
        return NULL;
    }
}


int SMGS_Get_Topic_Ply_Module(SMGS_topic_string_ptr_t str)
{

    if(str==NULL)
    {
        return SMGS_TOPIC_PLY_MODULE_END;
    }

    for(size_t i=0; i<SMGS_TOPIC_PLY_MODULE_END; i++)
    {
        if(strcmp(SMGS_topic_ply_module_string[i],str)==0)
        {
            return i;
        }
    }

    return SMGS_TOPIC_PLY_MODULE_END;
}


const SMGS_topic_string_ptr_t SMGS_topic_ply_cmd_string[SMGS_TOPIC_PLY_CMD_END]=
{
    "CMD",
    "RR",
    "RMR",
    "WR",
    "WMR",
    "RS",
    "RMS",
};

SMGS_topic_string_ptr_t SMGS_Get_Topic_Ply_CMD_String(int index)
{
    if(index>=0 && index < SMGS_TOPIC_PLY_CMD_END)
    {
        return SMGS_topic_ply_cmd_string[index];
    }
    else
    {
        return NULL;
    }
}

int SMGS_Get_Topic_Ply_CMD(SMGS_topic_string_ptr_t str)
{
    if(str==NULL)
    {
        return SMGS_TOPIC_PLY_CMD_END;
    }

    for(size_t i=0; i<SMGS_TOPIC_PLY_CMD_END; i++)
    {
        if(strcmp(SMGS_topic_ply_cmd_string[i],str)==0)
        {
            return i;
        }
    }

    return SMGS_TOPIC_PLY_CMD_END;
}


void SMGS_Device_Context_Init(SMGS_device_context_t *ctx)
{
    if(ctx==NULL)
    {
        return;
    }

    memset(ctx,0,sizeof(SMGS_device_context_t));
}

bool SMGS_Is_Device_Context_OK(SMGS_device_context_t *ctx)
{
    if(ctx==NULL)
    {
        return false;
    }

    if(ctx->DeviceSerialNumber==NULL)
    {
        return false;
    }

    if(ctx->IsOnline==NULL)
    {
        return false;
    }

    return true;
}

bool SMGS_Is_GateWay_Context_OK(SMGS_gateway_context_t *ctx)
{
    if(ctx==NULL)
    {
        return false;
    }


    //检查序列号
    if(ctx->GateWaySerialNumber==NULL || strlen(ctx->GateWaySerialNumber)<CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH)
    {
        return false;
    }

    //检查 MQTT消息发布
    if(ctx->MessagePublish==NULL)
    {
        return false;
    }

    //检查 设备模块相关函数
    if(ctx->Device_Find_By_Pos == NULL || ctx->Device_Find_By_SerialNumber==NULL )
    {
        return false;
    }

    return true;
}


static SMGS_device_context_t * Device_Find_By_SerialNumber(SMGS_gateway_context_t *ctx,const char* SerialNumber)
{
    if(ctx==NULL || ctx->Device_Next==NULL || SerialNumber==NULL)
    {
        return NULL;
    }

    SMGS_device_context_t * dev=NULL;

    while((dev=ctx->Device_Next(ctx,dev))!=NULL)
    {
        if(SMGS_Is_Device_Context_OK(dev))
        {
            if(strcmp(dev->DeviceSerialNumber,SerialNumber)==0)
            {
                break;
            }
        }
    }

    return dev;

}

static SMGS_device_context_t * Device_Find_By_Pos(struct __SMGS_gateway_context_t *ctx,uint8_t Pos)
{
    if(ctx==NULL || ctx->Device_Next==NULL)
    {
        return NULL;
    }

    SMGS_device_context_t * dev=NULL;

    while((dev=ctx->Device_Next(ctx,dev))!=NULL)
    {
        if(SMGS_Is_Device_Context_OK(dev))
        {
            if(dev->DevicePosNumber==Pos)
            {
                break;
            }
        }
    }

    return dev;
}

void SMGS_GateWay_Context_Init(SMGS_gateway_context_t *ctx,const char *SerialNumber, bool (*MessagePublish)(SMGS_gateway_context_t *,const char *,void *,size_t,uint8_t,int ))
{
    if(ctx==NULL)
    {
        return;
    }

    if(SerialNumber==NULL || strlen(SerialNumber)<CONFIG_SMGS_MIN_GATEWAY_SERIALNUMBER_LENGTH)
    {
        return;
    }

    if(MessagePublish==NULL)
    {
        return;
    }

    memset(ctx,0,sizeof(SMGS_gateway_context_t));

    ctx->GateWaySerialNumber=SerialNumber;
    ctx->MessagePublish=MessagePublish;
    ctx->Device_Find_By_SerialNumber=Device_Find_By_SerialNumber;
    ctx->Device_Find_By_Pos=Device_Find_By_Pos;
}


bool SMGS_GateWay_Send_GateWay_Event(SMGS_gateway_context_t *ctx,const char *cmd_para_1,const char * cmd_para_2,const char * cmd_para_3,SMGS_payload_cmdid_t cmdid,void *cmddata,size_t cmddata_length,SMGS_buff_t *Buff,uint8_t qos,int retian)
{
    if(Buff==NULL)
    {
        return false;
    }

    if(!SMGS_Is_GateWay_Context_OK(ctx))
    {
        return false;
    }

    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END];
    memset(plies,0,sizeof(plies));

    plies[SMGS_TOPIC_PLY_DESTADDR]=CONFIG_SMGS_SERVER_DEFAULT_NAME; //目的地址
    plies[SMGS_TOPIC_PLY_SRCADDR]=ctx->GateWaySerialNumber;
    plies[SMGS_TOPIC_PLY_COMTYPE]=SMGS_Get_Topic_Ply_ComType_String(SMGS_TOPIC_PLY_COMTYPE_BINEVT);
    plies[SMGS_TOPIC_PLY_MODULE]=SMGS_Get_Topic_Ply_Module_String(SMGS_TOPIC_PLY_MODULE_GATEWAY);
    plies[SMGS_TOPIC_PLY_CMD]=SMGS_Get_Topic_Ply_CMD_String(SMGS_TOPIC_PLY_CMD_COMMAND);
    plies[SMGS_TOPIC_PLY_CMD_PARA_1]=cmd_para_1;
    plies[SMGS_TOPIC_PLY_CMD_PARA_2]=cmd_para_2;
    plies[SMGS_TOPIC_PLY_CMD_PARA_3]=cmd_para_3;

    const char *topic=SMGS_Topic_Plies_EnCode(plies,SMGS_TOPIC_PLY_END,Buff);
    if(topic==NULL)
    {
        return false;
    }

    uint8_t *buff=SMGS_buff_alloc(Buff,(sizeof(cmdid)+cmddata_length));
    if(buff==NULL)
    {
        return false;//buff不够
    }
    uint8_t * payload=&buff[0];
    {
        payload[0]=(cmdid&0xFF);
        payload[1]=((cmdid>>8)&0xFF);
        if(cmddata!=NULL &&cmddata_length!=0)
        {
            memcpy(&payload[2],cmddata,cmddata_length);
        }
    }
    size_t payload_length=sizeof(cmdid)+cmddata_length;

    return ctx->MessagePublish(ctx,topic,payload,payload_length,qos,retian);
}


bool SMGS_GateWay_Online(SMGS_gateway_context_t *ctx,SMGS_buff_t *Buff,uint8_t qos,int retian)
{
    if(Buff==NULL)
    {
        return false;
    }
    SMGS_buff_t o=*Buff;//保存最初状态
    //发送上线消息
    if(!SMGS_GateWay_Send_GateWay_Event(ctx,"online",NULL,NULL,SMGS_GATEWAY_CMDID_ONLINE,NULL,0,Buff,qos,retian))
    {
        return false;
    }

    (*Buff)=o;//还原状态

    //上报设备表
    if(ctx->Device_Next!=NULL)
    {
        SMGS_device_context_t *devctx=NULL;
        while((devctx=ctx->Device_Next(ctx,devctx))!=NULL)
        {
            if(SMGS_Is_Device_Context_OK(devctx))
            {
                (*Buff)=o;//还原缓存状态
                size_t cmddatalen=strlen(devctx->DeviceSerialNumber)+2;
                uint8_t *buff=SMGS_buff_alloc(Buff,cmddatalen);
                if(buff==NULL)
                {
                    continue;
                }
                memset(buff,0,cmddatalen);

                //填写位置号
                buff[0]=devctx->DevicePosNumber;

                //复制字符串
                strcpy((char *)&buff[1],devctx->DeviceSerialNumber);

                if(!SMGS_GateWay_Send_GateWay_Event(ctx,NULL,NULL,NULL,SMGS_GATEWAY_CMDID_REPORT_DEVICETABLE_ONLINE,buff,cmddatalen,Buff,qos,retian))
                {
                    return false;
                }


            }
        }
    }


    return true;
}

bool SMGS_GateWay_Send_Device_Event(SMGS_gateway_context_t *ctx,SMGS_device_context_t *devctx,const char *cmd_para_1,const char * cmd_para_2,const char * cmd_para_3,SMGS_payload_cmdid_t cmdid,void *cmddata,size_t cmddata_length,SMGS_buff_t *Buff,uint8_t qos,int retian)
{
    if(Buff==NULL )
    {
        return false;
    }

    if(!SMGS_Is_GateWay_Context_OK(ctx))
    {
        return false;
    }

    if(!SMGS_Is_Device_Context_OK(devctx))
    {
        return false;
    }

    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END];
    memset(plies,0,sizeof(plies));

    plies[SMGS_TOPIC_PLY_DESTADDR]=CONFIG_SMGS_SERVER_DEFAULT_NAME; //目的地址
    plies[SMGS_TOPIC_PLY_SRCADDR]=ctx->GateWaySerialNumber;
    plies[SMGS_TOPIC_PLY_COMTYPE]=SMGS_Get_Topic_Ply_ComType_String(SMGS_TOPIC_PLY_COMTYPE_BINEVT);
    plies[SMGS_TOPIC_PLY_MODULE]=devctx->DeviceSerialNumber;
    plies[SMGS_TOPIC_PLY_CMD]=SMGS_Get_Topic_Ply_CMD_String(SMGS_TOPIC_PLY_CMD_COMMAND);
    plies[SMGS_TOPIC_PLY_CMD_PARA_1]=cmd_para_1;
    plies[SMGS_TOPIC_PLY_CMD_PARA_2]=cmd_para_2;
    plies[SMGS_TOPIC_PLY_CMD_PARA_3]=cmd_para_3;

    const char *topic=SMGS_Topic_Plies_EnCode(plies,SMGS_TOPIC_PLY_END,Buff);
    if(topic==NULL)
    {
        return false;
    }

    uint8_t *buff=SMGS_buff_alloc(Buff,(sizeof(cmdid)+cmddata_length));

    //检查剩余buff大小
    if(buff==NULL)
    {
        return false;//buff不够
    }

    uint8_t * payload=&buff[0];
    {
        payload[0]=(cmdid&0xFF);
        payload[1]=((cmdid>>8)&0xFF);
        if(cmddata!=NULL &&cmddata_length!=0)
        {
            memcpy(&payload[2],cmddata,cmddata_length);
        }
    }
    size_t payload_length=sizeof(cmdid)+cmddata_length;

    return ctx->MessagePublish(ctx,topic,payload,payload_length,qos,retian);
}

/*
网关回复设备模块的BinReq。参数必须有效
*/
static bool SMGS_GateWay_Reply_Comtype_BinReq(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t src_plies[],size_t src_plies_count,uint8_t *retpayload,size_t retpayloadlen,uint8_t qos,int retain,SMGS_buff_t *Buff)
{
    src_plies[SMGS_TOPIC_PLY_DESTADDR]=src_plies[SMGS_TOPIC_PLY_SRCADDR];//目的地址为源地址
    src_plies[SMGS_TOPIC_PLY_SRCADDR]=ctx->GateWaySerialNumber;//源地址为网关序列号
    src_plies[SMGS_TOPIC_PLY_COMTYPE]=SMGS_Get_Topic_Ply_ComType_String(SMGS_TOPIC_PLY_COMTYPE_BINRESP);//通信类型为二进制回复

    const char * topic=SMGS_Topic_Plies_EnCode(src_plies,src_plies_count,Buff);

    if(topic!=NULL && ctx->MessagePublish!=NULL)
    {
        return ctx->MessagePublish(ctx,topic,retpayload,retpayloadlen,qos,retain);
    }

    return false;
}

/*
网关处理设备模块的BinReq的内部命令。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device_CMD_Internal_Command(SMGS_device_context_t *ctx,SMGS_payload_cmdid_t *cmdid,uint8_t *cmddata,size_t cmddata_length,uint8_t *retbuff,size_t *retbuff_length,SMGS_payload_retcode_t *retcode)
{
    bool ret=false;

    switch(*cmdid)
    {
    case SMGS_DEVICE_CMDID_QUERY_DEVICENAME:
    {
        if(ctx->IsOnline(ctx) && retbuff!=NULL && ctx->DeviceName!=NULL && (*retbuff_length)> (strlen(ctx->DeviceName)+1))
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_SUCCESS;
            memset(retbuff,0,(strlen(ctx->DeviceName)+1));
            strcpy((char *)retbuff,ctx->DeviceName);
            ret=true;
            (*retbuff_length)=(strlen(ctx->DeviceName)+1);
        }
        else
        {
            (*retcode)=(ctx->IsOnline(ctx)?SMGS_PAYLOAD_RETCODE_UNKOWN_ERROR:SMGS_PAYLOAD_RETCODE_MODULE_INVALID);
            ret=true;
            (*retbuff_length)=0;
        }

    }
    break;
    case SMGS_DEVICE_CMDID_QUERY_DEVICESERIALNUMBER:
    {
        if(ctx->IsOnline(ctx) && retbuff!=NULL && ctx->DeviceSerialNumber!=NULL && (*retbuff_length)> (strlen(ctx->DeviceSerialNumber)+1))
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_SUCCESS;
            memset(retbuff,0,(strlen(ctx->DeviceSerialNumber)+1));
            strcpy((char *)retbuff,ctx->DeviceSerialNumber);
            ret=true;
            (*retbuff_length)=(strlen(ctx->DeviceSerialNumber)+1);
        }
        else
        {
            (*retcode)=(ctx->IsOnline(ctx)?SMGS_PAYLOAD_RETCODE_UNKOWN_ERROR:SMGS_PAYLOAD_RETCODE_MODULE_INVALID);
            ret=true;
            (*retbuff_length)=0;
        }

    }
    break;
    case SMGS_DEVICE_CMDID_QUERY_ISONLINE:
    {
        if(ctx->IsOnline(ctx))
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_SUCCESS;
            ret=true;
            (*retbuff_length)=1;
            if(retbuff!=NULL)
            {
                retbuff[0]=1;
            }

        }
        else
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_MODULE_INVALID;
            ret=true;
            (*retbuff_length)=1;
            if(retbuff!=NULL)
            {
                retbuff[0]=0;
            }
        }
    }
    break;
    default:
        break;
    }

    return ret;
}

/*
网关处理设备模块的BinReq的读寄存器。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device_CMD_Internal_ReadRegister(SMGS_device_context_t *ctx,SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag)
{
    bool ret=false;

    return ret;
}

/*
网关处理设备模块的BinReq的写寄存器。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device_CMD_Internal_WriteRegister(SMGS_device_context_t *ctx,SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag)
{
    bool ret=false;

    return ret;
}

/*
网关处理设备模块的BinReq。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device(SMGS_gateway_context_t *ctx,SMGS_device_context_t *devctx,SMGS_topic_string_ptr_t plies[],size_t plies_count,uint8_t *payload,size_t payloadlen,uint8_t qos,int retain,SMGS_buff_t *Buff)
{
    bool ret=false;

    uint8_t *buff=SMGS_buff_alloc(Buff,0);
    size_t  buff_size=Buff->total-Buff->used;
    if(Buff==NULL || buff==NULL || buff_size==0)
    {
        return ret;
    }

    switch(SMGS_Get_Topic_Ply_CMD(plies[SMGS_TOPIC_PLY_CMD]))
    {
    case SMGS_TOPIC_PLY_CMD_COMMAND:
    {
        SMGS_payload_cmdid_t cmdid=0;
        if(payloadlen<2 || payload==NULL)
        {
            break;
        }
        cmdid+=payload[1];
        cmdid<<=8;
        cmdid+=payload[0];

        if(IS_SMGS_DEVICE_INTERNAL_CMDID(cmdid))
        {
            //处理内部命令
            uint8_t *free_buff=buff;
            size_t  free_buff_size=buff_size;

            uint8_t *retbuff=free_buff;
            size_t retbufflen=free_buff_size;
            SMGS_payload_retcode_t retcode=0;
            if(SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device_CMD_Internal_Command(devctx,&cmdid,&payload[2],payloadlen-2,retbuff,&retbufflen,&retcode))
            {
                if(retbufflen < free_buff_size)
                {
                    //减去占用的字节数
                    free_buff+=retbufflen;
                    free_buff_size-=retbufflen;

                    size_t retpaylodlen=sizeof(SMGS_payload_cmdid_t)+sizeof(SMGS_payload_retcode_t)+retbufflen;
                    uint8_t *retpayload=free_buff;

                    if(retpaylodlen < free_buff_size)
                    {
                        retpayload[0]=cmdid&0xFF;
                        retpayload[1]=((cmdid>>8)&0xFF);
                        retpayload[2]=retcode;
                        memcpy(&retpayload[3],retbuff,retbufflen);

                        //减去占用的字节数
                        free_buff+=retpaylodlen;
                        free_buff_size-=retpaylodlen;

                        SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间

                        ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpaylodlen,qos,retain,Buff);

                    }

                }
            }
        }
        else
        {
            //调用回调函数
            if(devctx->Command!=NULL)
            {
                uint8_t *free_buff=buff;
                size_t  free_buff_size=buff_size;

                uint8_t *retbuff=free_buff;
                size_t retbufflen=free_buff_size;
                SMGS_payload_retcode_t retcode=0;
                if(devctx->Command(devctx,plies,&cmdid,&payload[2],payloadlen-2,retbuff,&retbufflen,&retcode))
                {
                    if(retbufflen < free_buff_size)
                    {
                        //减去占用的字节数
                        free_buff+=retbufflen;
                        free_buff_size-=retbufflen;

                        size_t retpaylodlen=sizeof(SMGS_payload_cmdid_t)+sizeof(SMGS_payload_retcode_t)+retbufflen;
                        uint8_t *retpayload=free_buff;

                        if(retpaylodlen < free_buff_size)
                        {
                            retpayload[0]=cmdid&0xFF;
                            retpayload[1]=((cmdid>>8)&0xFF);
                            retpayload[2]=retcode;
                            memcpy(&retpayload[3],retbuff,retbufflen);

                            //减去占用的字节数
                            free_buff+=retpaylodlen;
                            free_buff_size-=retpaylodlen;

                            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
                            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpaylodlen,qos,retain,Buff);

                        }

                    }
                }
            }
        }

    }
    break;
    case SMGS_TOPIC_PLY_CMD_READREGISTER:
    {
        if(payloadlen < 2 || payload==NULL)
        {
            break;
        }
        SMGS_payload_register_address_t addr=0;

        addr+=payload[1];
        addr<<=8;
        addr+=payload[0];

        bool IsReadSuccess=false;
        uint64_t dat=0;
        SMGS_payload_register_flag_t flag;
        memset(&flag,0,sizeof(flag));

        if(IS_SMGS_DEVICE_INTERNAL_REGISTER_ADDRESS(addr))
        {
            IsReadSuccess=SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device_CMD_Internal_ReadRegister(devctx,addr,&dat,&flag);
        }
        else
        {
            if(devctx->ReadRegister!=NULL)
            {
                IsReadSuccess=devctx->ReadRegister(devctx,plies,addr,&dat,&flag);
            }
        }


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        if(IsReadSuccess && devctx->IsOnline(devctx))
        {
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_register_address_t)+sizeof(SMGS_payload_register_flag_t)+pow(2,flag.reglen);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }
            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);
            retpayload[3]=flag.val;

            for(size_t i=0; i<pow(2,flag.reglen); i++)
            {
                retpayload[4+i]=((dat>>(8*i))&0xFF);
            }

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            //读取失败
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_register_address_t);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }

            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=(devctx->IsOnline(devctx)?SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID:SMGS_PAYLOAD_RETCODE_MODULE_INVALID);
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);

        }

    }
    break;
    case SMGS_TOPIC_PLY_CMD_READMULTIREGISTER:
    {
        if(payloadlen < 4 || payload==NULL)
        {
            break;
        }
        SMGS_payload_register_address_t addr=0;
        size_t length=0;

        addr+=payload[1];
        addr<<=8;
        addr+=payload[0];

        length+=payload[3];
        length<<=8;
        length+=payload[2];

        size_t success_count=0;


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        uint8_t *retpayload=free_buff;
        size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t);

        if(free_buff_size<retpayloadlen)
        {
            break;
        }
        free_buff_size-=retpayloadlen;
        free_buff+=retpayloadlen;

        {
            //读取寄存器
            for(size_t i=0; i<length; i++)
            {
                bool IsReadSuccess=false;
                uint64_t dat=0;
                SMGS_payload_register_flag_t flag;
                memset(&flag,0,sizeof(flag));

                if(IS_SMGS_DEVICE_INTERNAL_REGISTER_ADDRESS(addr+i))
                {
                    IsReadSuccess=SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device_CMD_Internal_ReadRegister(devctx,addr+i,&dat,&flag);
                }
                else
                {
                    if(devctx->ReadRegister!=NULL)
                    {
                        IsReadSuccess=devctx->ReadRegister(devctx,plies,addr+i,&dat,&flag);
                    }
                }

                if(IsReadSuccess)
                {
                    uint8_t *databuff=&retpayload[retpayloadlen];
                    size_t  databufflen=sizeof(SMGS_payload_register_address_t)+sizeof(SMGS_payload_register_flag_t)+pow(2,flag.reglen);

                    if(free_buff_size<databufflen)
                    {
                        break;
                    }
                    free_buff_size-=databufflen;
                    free_buff+=databufflen;

                    databuff[0]=((addr+i)&0xFF);
                    databuff[1]=(((addr+i)>>8)&0xFF);
                    databuff[2]=flag.val;

                    for(size_t j=0; j<pow(2,flag.reglen); j++)
                    {
                        databuff[3+j]=((dat>>(8*j))&0XFF);
                    }

                    retpayloadlen+=databufflen;

                    success_count++;

                }
            }
        }

        if(success_count>0 && devctx->IsOnline(devctx))
        {
            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            retpayload[0]=(devctx->IsOnline(devctx)?SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID:SMGS_PAYLOAD_RETCODE_MODULE_INVALID);
            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_WRITEREGISTER:
    {
        if(payloadlen < 4 || payload==NULL)
        {
            break;
        }
        SMGS_payload_register_address_t addr=0;

        addr+=payload[1];
        addr<<=8;
        addr+=payload[0];

        bool IsWriteSuccess=false;
        uint64_t dat=0;
        SMGS_payload_register_flag_t flag;
        memset(&flag,0,sizeof(flag));

        {
            //填写数据
            flag.val=payload[2];
            for(size_t i=0; i< (payloadlen-3) && i< sizeof(dat); i++)
            {
                dat|= (((uint64_t)payload[3+i])<<(8*i));
            }
        }

        if(IS_SMGS_DEVICE_INTERNAL_REGISTER_ADDRESS(addr))
        {
            IsWriteSuccess=SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device_CMD_Internal_WriteRegister(devctx,addr,&dat,&flag);
        }
        else
        {
            if(devctx->WriteRegister!=NULL)
            {
                IsWriteSuccess=devctx->WriteRegister(devctx,plies,addr,&dat,&flag);
            }
        }


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        if(IsWriteSuccess & devctx->IsOnline(devctx))
        {
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_register_address_t)+sizeof(SMGS_payload_register_flag_t)+pow(2,flag.reglen);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }
            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);
            retpayload[3]=flag.val;

            for(size_t i=0; i<pow(2,flag.reglen); i++)
            {
                retpayload[4+i]=((dat>>(8*i))&0xFF);
            }

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            //读取失败
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_register_address_t);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }

            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=(devctx->IsOnline(devctx)?SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID:SMGS_PAYLOAD_RETCODE_MODULE_INVALID);;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);

        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_WRITEMULTIREGISTER:
    {
        if(payloadlen < 4 || payload==NULL)
        {
            break;
        }
        size_t success_count=0;


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        uint8_t *retpayload=free_buff;
        size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t);

        if(free_buff_size<retpayloadlen)
        {
            break;
        }
        free_buff_size-=retpayloadlen;
        free_buff+=retpayloadlen;

        {
            //写入寄存器
            for(size_t i=0; i<payloadlen;)
            {
                bool IsWriteSuccess=false;
                uint64_t dat=0;
                SMGS_payload_register_flag_t flag;
                memset(&flag,0,sizeof(flag));
                SMGS_payload_register_address_t addr=0;

                addr+=payload[i+1];
                addr<<=8;
                addr+=payload[i+0];

                flag.val=payload[i+2];

                if(payloadlen-i >= (pow(2,flag.reglen)+3))
                {
                    for(size_t j=0; j<pow(2,flag.reglen); j++)
                    {
                        dat|=(((uint64_t)payload[i+3+j]) << (j*8));
                    }
                }
                else
                {
                    break;
                }

                i+=(3+pow(2,flag.reglen));

                if( i > payloadlen || payloadlen-i < 4)
                {
                    break;
                }

                if(IS_SMGS_DEVICE_INTERNAL_REGISTER_ADDRESS(addr))
                {
                    IsWriteSuccess=SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device_CMD_Internal_WriteRegister(devctx,addr,&dat,&flag);
                }
                else
                {
                    if(devctx->WriteRegister!=NULL)
                    {
                        IsWriteSuccess=devctx->WriteRegister(devctx,plies,addr,&dat,&flag);
                    }
                }

                if(IsWriteSuccess)
                {
                    uint8_t *databuff=&retpayload[retpayloadlen];
                    size_t  databufflen=sizeof(SMGS_payload_register_address_t)+sizeof(SMGS_payload_register_flag_t)+pow(2,flag.reglen);

                    if(free_buff_size<databufflen)
                    {
                        break;
                    }
                    free_buff_size-=databufflen;
                    free_buff+=databufflen;

                    databuff[0]=((addr+i)&0xFF);
                    databuff[1]=(((addr+i)>>8)&0xFF);
                    databuff[2]=flag.val;

                    for(size_t j=0; j<pow(2,flag.reglen); j++)
                    {
                        databuff[3+j]=((dat>>(8*j))&0XFF);
                    }

                    retpayloadlen+=databufflen;

                    success_count++;

                }
            }
        }

        SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
        if(success_count>0 && devctx->IsOnline(devctx))
        {
            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            retpayload[0]=(devctx->IsOnline(devctx)?SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID:SMGS_PAYLOAD_RETCODE_MODULE_INVALID);
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_READSENSOR:
    {
        if(payloadlen < 3 || payload==NULL)
        {
            break;
        }
        SMGS_payload_sensor_address_t addr=0;
        SMGS_payload_sensor_flag_t flag;
        memset(&flag,0,sizeof(flag));

        addr+=payload[1];
        addr<<=8;
        addr+=payload[0];

        flag.val=payload[2];

        bool IsReadSuccess=false;
        uint64_t dat=0;


        {
            if(devctx->ReadSensor!=NULL)
            {
                IsReadSuccess=devctx->ReadSensor(devctx,plies,addr,&dat,&flag);
            }
        }


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        if(IsReadSuccess)
        {
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_sensor_address_t)+sizeof(SMGS_payload_sensor_flag_t)+pow(2,flag.sensorlen);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }
            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);
            retpayload[3]=flag.val;

            for(size_t i=0; i<pow(2,flag.sensorlen); i++)
            {
                retpayload[4+i]=((dat>>(8*i))&0xFF);
            }

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            //读取失败
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_sensor_address_t);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }

            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=(devctx->IsOnline(devctx)?SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID:SMGS_PAYLOAD_RETCODE_MODULE_INVALID);
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);

        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_READMULTISENSOR:
    {
        if(payloadlen < 3 || payloadlen%3 != 0 || payload==NULL)
        {
            break;
        }

        size_t success_count=0;


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        uint8_t *retpayload=free_buff;
        size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t);

        if(free_buff_size<retpayloadlen)
        {
            break;
        }
        free_buff_size-=retpayloadlen;
        free_buff+=retpayloadlen;

        {
            //读取传感器
            for(size_t i=0; i< (payloadlen/3) ; i++)
            {
                bool IsReadSuccess=false;
                uint64_t dat=0;
                SMGS_payload_sensor_address_t addr= 0;
                SMGS_payload_sensor_flag_t flag;
                memset(&flag,0,sizeof(flag));

                addr+=payload[3*i+1];
                addr<<=8;
                addr+=payload[3*i+0];

                flag.val=payload[3*i+2];

                {
                    if(devctx->ReadSensor!=NULL)
                    {
                        IsReadSuccess=devctx->ReadSensor(devctx,plies,addr,&dat,&flag);
                    }
                }

                if(IsReadSuccess)
                {
                    uint8_t *databuff=&retpayload[retpayloadlen];
                    size_t  databufflen=sizeof(SMGS_payload_sensor_address_t)+sizeof(SMGS_payload_sensor_flag_t)+pow(2,flag.sensorlen);

                    if(free_buff_size<databufflen)
                    {
                        break;
                    }
                    free_buff_size-=databufflen;
                    free_buff+=databufflen;

                    databuff[0]=((addr)&0xFF);
                    databuff[1]=(((addr)>>8)&0xFF);
                    databuff[2]=flag.val;

                    for(size_t j=0; j<pow(2,flag.sensorlen); j++)
                    {
                        databuff[3+j]=((dat>>(8*j))&0XFF);
                    }

                    retpayloadlen+=databufflen;

                    success_count++;

                }
            }
        }

        SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
        if(success_count>0 && devctx->IsOnline(devctx))
        {
            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            retpayload[0]=(devctx->IsOnline(devctx)?SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID:SMGS_PAYLOAD_RETCODE_MODULE_INVALID);
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_END:
    default:
        break;
    }

    return ret;
}

/*
网关处理网关模块的BinReq的内部命令。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_Command(SMGS_gateway_context_t *ctx,SMGS_payload_cmdid_t *cmdid,uint8_t *cmddata,size_t cmddata_length,uint8_t *retbuff,size_t *retbuff_length,SMGS_payload_retcode_t *retcode)
{
    bool ret=false;
    switch(*cmdid)
    {
    case SMGS_GATEWAY_CMDID_QUERY_GATEWAYNAME:
    {
        if(retbuff!=NULL && ctx->GateWayName!=NULL && (*retbuff_length)> (strlen(ctx->GateWayName)+1))
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_SUCCESS;
            memset(retbuff,0,(strlen(ctx->GateWayName)+1));
            strcpy((char *)retbuff,ctx->GateWayName);
            ret=true;
            (*retbuff_length)=(strlen(ctx->GateWayName)+1);
        }
        else
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_UNKOWN_ERROR;
            ret=true;
            (*retbuff_length)=0;
        }
    }
    break;
    case SMGS_GATEWAY_CMDID_QUERY_GATEWAYSERIALNUMBER:
    {
        if(retbuff!=NULL && ctx->GateWaySerialNumber!=NULL && (*retbuff_length)> (strlen(ctx->GateWaySerialNumber)+1))
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_SUCCESS;
            memset(retbuff,0,(strlen(ctx->GateWaySerialNumber)+1));
            strcpy((char *)retbuff,ctx->GateWaySerialNumber);
            ret=true;
            (*retbuff_length)=(strlen(ctx->GateWaySerialNumber)+1);
        }
        else
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_UNKOWN_ERROR;
            ret=true;
            (*retbuff_length)=0;
        }
    }
    break;
    case SMGS_GATEWAY_CMDID_QUERY_DEVICETABLE:
    {
        if(cmddata_length<1)
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_CMD_PAYLOAD_ERROR;
            (*retbuff_length)=0;
        }
        else
        {
            uint8_t pos=cmddata[0];

            SMGS_device_context_t *devctx=ctx->Device_Find_By_Pos(ctx,pos);
            if(devctx==NULL)
            {
                (*retcode)=SMGS_PAYLOAD_RETCODE_UNKOWN_MODULE;
                (*retbuff_length)=0;
            }
            else
            {
                if( retbuff!=NULL && devctx->DeviceName!=NULL && (*retbuff_length) > (strlen(devctx->DeviceSerialNumber)+3) )
                {
                    (*retcode)=SMGS_PAYLOAD_RETCODE_SUCCESS;
                    memset(retbuff,0,(strlen(devctx->DeviceSerialNumber)+3));
                    retbuff[0]=pos;
                    retbuff[1]=(devctx->IsOnline(devctx)?1:0);
                    strcpy((char *)&retbuff[2],devctx->DeviceSerialNumber);
                    (*retbuff_length)=(strlen(devctx->DeviceSerialNumber)+3);
                }
                else
                {
                    (*retcode)=SMGS_PAYLOAD_RETCODE_UNKOWN_ERROR;
                    (*retbuff_length)=0;
                }
            }
        }

        ret=true;
    }
    break;
    case SMGS_GATEWAY_CMDID_QUERY_DEVICETABLE_LIST:
    {
        if((*retbuff_length) > 32 )
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_SUCCESS;
            ret=true;
            (*retbuff_length)=32;
            memset(retbuff,0,(*retbuff_length));

            for(uint16_t i=0; i< 256 ; i++)
            {
                SMGS_device_context_t *devctx=ctx->Device_Find_By_Pos(ctx,i);
                if(devctx!=NULL)
                {
                    retbuff[devctx->DevicePosNumber/8] |= (0x01<<(devctx->DevicePosNumber%8));
                }
            }
        }
        else
        {
            (*retcode)=SMGS_PAYLOAD_RETCODE_UNKOWN_ERROR;
            ret=true;
            (*retbuff_length)=0;
        }
    }
    break;


    default:
        break;
    }
    return ret;
}

/*
网关处理网关模块的BinReq的内部读寄存器。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_ReadRegister(SMGS_gateway_context_t *ctx,SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag)
{
    bool ret=false;

    return ret;
}

/*
网关处理网关模块的BinReq的内部写寄存器。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_WriteRegister(SMGS_gateway_context_t *ctx,SMGS_payload_register_address_t addr,uint64_t *dat,SMGS_payload_register_flag_t *flag)
{
    bool ret=false;

    return ret;
}

/*
网关处理网关模块的BinReq。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],size_t plies_count,uint8_t *payload,size_t payloadlen,uint8_t qos,int retain,SMGS_buff_t *Buff)
{
    bool ret=false;

    uint8_t *buff=SMGS_buff_alloc(Buff,0);
    size_t  buff_size=Buff->total-Buff->used;
    if(Buff==NULL || buff==NULL || buff_size==0)
    {
        return ret;
    }

    switch(SMGS_Get_Topic_Ply_CMD(plies[SMGS_TOPIC_PLY_CMD]))
    {

    case SMGS_TOPIC_PLY_CMD_COMMAND:
    {
        SMGS_payload_cmdid_t cmdid=0;
        if(payloadlen<2 || payload==NULL)
        {
            break;
        }
        cmdid+=payload[1];
        cmdid<<=8;
        cmdid+=payload[0];

        if(IS_SMGS_GATEWAY_INTERNAL_CMDID(cmdid))
        {
            //处理内部命令
            uint8_t *free_buff=buff;
            size_t  free_buff_size=buff_size;

            uint8_t *retbuff=free_buff;
            size_t retbufflen=free_buff_size;
            SMGS_payload_retcode_t retcode=0;
            if(SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_Command(ctx,&cmdid,&payload[2],payloadlen-2,retbuff,&retbufflen,&retcode))
            {
                if(retbufflen < free_buff_size)
                {
                    //减去占用的字节数
                    free_buff+=retbufflen;
                    free_buff_size-=retbufflen;

                    size_t retpaylodlen=sizeof(SMGS_payload_cmdid_t)+sizeof(SMGS_payload_retcode_t)+retbufflen;
                    uint8_t *retpayload=free_buff;

                    if(retpaylodlen < free_buff_size)
                    {
                        retpayload[0]=cmdid&0xFF;
                        retpayload[1]=((cmdid>>8)&0xFF);
                        retpayload[2]=retcode;
                        memcpy(&retpayload[3],retbuff,retbufflen);

                        //减去占用的字节数
                        free_buff+=retpaylodlen;
                        free_buff_size-=retpaylodlen;

                        SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
                        ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpaylodlen,qos,retain,Buff);

                    }

                }
            }
        }
        else
        {
            //调用回调函数
            if(ctx->Command!=NULL)
            {
                uint8_t *free_buff=buff;
                size_t  free_buff_size=buff_size;

                uint8_t *retbuff=free_buff;
                size_t retbufflen=free_buff_size;
                SMGS_payload_retcode_t retcode=0;
                if(ctx->Command(ctx,plies,&cmdid,&payload[2],payloadlen-2,retbuff,&retbufflen,&retcode))
                {
                    if(retbufflen < free_buff_size)
                    {
                        //减去占用的字节数
                        free_buff+=retbufflen;
                        free_buff_size-=retbufflen;

                        size_t retpaylodlen=sizeof(SMGS_payload_cmdid_t)+sizeof(SMGS_payload_retcode_t)+retbufflen;
                        uint8_t *retpayload=free_buff;

                        if(retpaylodlen < free_buff_size)
                        {
                            retpayload[0]=cmdid&0xFF;
                            retpayload[1]=((cmdid>>8)&0xFF);
                            retpayload[2]=retcode;
                            memcpy(&retpayload[3],retbuff,retbufflen);

                            //减去占用的字节数
                            free_buff+=retpaylodlen;
                            free_buff_size-=retpaylodlen;

                            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
                            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpaylodlen,qos,retain,Buff);

                        }

                    }
                }
            }
        }

    }
    break;
    case SMGS_TOPIC_PLY_CMD_READREGISTER:
    {
        if(payloadlen < 2 || payload==NULL)
        {
            break;
        }
        SMGS_payload_register_address_t addr=0;

        addr+=payload[1];
        addr<<=8;
        addr+=payload[0];

        bool IsReadSuccess=false;
        uint64_t dat=0;
        SMGS_payload_register_flag_t flag;
        memset(&flag,0,sizeof(flag));

        if(IS_SMGS_GATEWAY_INTERNAL_REGISTER_ADDRESS(addr))
        {
            IsReadSuccess=SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_ReadRegister(ctx,addr,&dat,&flag);
        }
        else
        {
            if(ctx->ReadRegister!=NULL)
            {
                IsReadSuccess=ctx->ReadRegister(ctx,plies,addr,&dat,&flag);
            }
        }


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        if(IsReadSuccess)
        {
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_register_address_t)+sizeof(SMGS_payload_register_flag_t)+pow(2,flag.reglen);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }
            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);
            retpayload[3]=flag.val;

            for(size_t i=0; i<pow(2,flag.reglen); i++)
            {
                retpayload[4+i]=((dat>>(8*i))&0xFF);
            }

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            //读取失败
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_register_address_t);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }

            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);

        }

    }
    break;
    case SMGS_TOPIC_PLY_CMD_READMULTIREGISTER:
    {
        if(payloadlen < 4 || payload==NULL)
        {
            break;
        }
        SMGS_payload_register_address_t addr=0;
        size_t length=0;

        addr+=payload[1];
        addr<<=8;
        addr+=payload[0];

        length+=payload[3];
        length<<=8;
        length+=payload[2];

        size_t success_count=0;


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        uint8_t *retpayload=free_buff;
        size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t);

        if(free_buff_size<retpayloadlen)
        {
            break;
        }
        free_buff_size-=retpayloadlen;
        free_buff+=retpayloadlen;

        {
            //读取寄存器
            for(size_t i=0; i<length; i++)
            {
                bool IsReadSuccess=false;
                uint64_t dat=0;
                SMGS_payload_register_flag_t flag;
                memset(&flag,0,sizeof(flag));

                if(IS_SMGS_GATEWAY_INTERNAL_REGISTER_ADDRESS(addr+i))
                {
                    IsReadSuccess=SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_ReadRegister(ctx,addr+i,&dat,&flag);
                }
                else
                {
                    if(ctx->ReadRegister!=NULL)
                    {
                        IsReadSuccess=ctx->ReadRegister(ctx,plies,addr+i,&dat,&flag);
                    }
                }

                if(IsReadSuccess)
                {
                    uint8_t *databuff=&retpayload[retpayloadlen];
                    size_t  databufflen=sizeof(SMGS_payload_register_address_t)+sizeof(SMGS_payload_register_flag_t)+pow(2,flag.reglen);

                    if(free_buff_size<databufflen)
                    {
                        break;
                    }
                    free_buff_size-=databufflen;
                    free_buff+=databufflen;

                    databuff[0]=((addr+i)&0xFF);
                    databuff[1]=(((addr+i)>>8)&0xFF);
                    databuff[2]=flag.val;

                    for(size_t j=0; j<pow(2,flag.reglen); j++)
                    {
                        databuff[3+j]=((dat>>(8*j))&0XFF);
                    }

                    retpayloadlen+=databufflen;

                    success_count++;

                }
            }
        }

        SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
        if(success_count>0)
        {
            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            retpayload[0]=SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID;
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_WRITEREGISTER:
    {
        if(payloadlen < 4 || payload==NULL)
        {
            break;
        }
        SMGS_payload_register_address_t addr=0;

        addr+=payload[1];
        addr<<=8;
        addr+=payload[0];

        bool IsWriteSuccess=false;
        uint64_t dat=0;
        SMGS_payload_register_flag_t flag;
        memset(&flag,0,sizeof(flag));

        {
            //填写数据
            flag.val=payload[2];
            for(size_t i=0; i< (payloadlen-3) && i< sizeof(dat); i++)
            {
                dat|= (((uint64_t)payload[3+i])<<(8*i));
            }
        }

        if(IS_SMGS_GATEWAY_INTERNAL_REGISTER_ADDRESS(addr))
        {
            IsWriteSuccess=SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_WriteRegister(ctx,addr,&dat,&flag);
        }
        else
        {
            if(ctx->WriteRegister!=NULL)
            {
                IsWriteSuccess=ctx->WriteRegister(ctx,plies,addr,&dat,&flag);
            }
        }


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        if(IsWriteSuccess)
        {
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_register_address_t)+sizeof(SMGS_payload_register_flag_t)+pow(2,flag.reglen);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }
            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);
            retpayload[3]=flag.val;

            for(size_t i=0; i<pow(2,flag.reglen); i++)
            {
                retpayload[4+i]=((dat>>(8*i))&0xFF);
            }
            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            //读取失败
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_register_address_t);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }

            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);

        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_WRITEMULTIREGISTER:
    {
        if(payloadlen < 4 || payload==NULL)
        {
            break;
        }
        size_t success_count=0;


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        uint8_t *retpayload=free_buff;
        size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t);

        if(free_buff_size<retpayloadlen)
        {
            break;
        }
        free_buff_size-=retpayloadlen;
        free_buff+=retpayloadlen;

        {
            //写入寄存器
            for(size_t i=0; i<payloadlen;)
            {
                bool IsWriteSuccess=false;
                uint64_t dat=0;
                SMGS_payload_register_flag_t flag;
                memset(&flag,0,sizeof(flag));
                SMGS_payload_register_address_t addr=0;

                addr+=payload[i+1];
                addr<<=8;
                addr+=payload[i+0];

                flag.val=payload[i+2];

                if(payloadlen-i >= (pow(2,flag.reglen)+3))
                {
                    for(size_t j=0; j<pow(2,flag.reglen); j++)
                    {
                        dat|=(((uint64_t)payload[i+3+j]) << (j*8));
                    }
                }
                else
                {
                    break;
                }

                i+=(3+pow(2,flag.reglen));

                if( i > payloadlen || payloadlen-i < 4)
                {
                    break;
                }

                if(IS_SMGS_GATEWAY_INTERNAL_REGISTER_ADDRESS(addr))
                {
                    IsWriteSuccess=SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_WriteRegister(ctx,addr,&dat,&flag);
                }
                else
                {
                    if(ctx->WriteRegister!=NULL)
                    {
                        IsWriteSuccess=ctx->WriteRegister(ctx,plies,addr,&dat,&flag);
                    }
                }

                if(IsWriteSuccess)
                {
                    uint8_t *databuff=&retpayload[retpayloadlen];
                    size_t  databufflen=sizeof(SMGS_payload_register_address_t)+sizeof(SMGS_payload_register_flag_t)+pow(2,flag.reglen);

                    if(free_buff_size<databufflen)
                    {
                        break;
                    }
                    free_buff_size-=databufflen;
                    free_buff+=databufflen;

                    databuff[0]=((addr+i)&0xFF);
                    databuff[1]=(((addr+i)>>8)&0xFF);
                    databuff[2]=flag.val;

                    for(size_t j=0; j<pow(2,flag.reglen); j++)
                    {
                        databuff[3+j]=((dat>>(8*j))&0XFF);
                    }

                    retpayloadlen+=databufflen;

                    success_count++;

                }
            }
        }
        SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
        if(success_count>0)
        {
            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            retpayload[0]=SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID;
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_READSENSOR:
    {
        if(payloadlen < 3 || payload==NULL)
        {
            break;
        }
        SMGS_payload_sensor_address_t addr=0;
        SMGS_payload_sensor_flag_t flag;
        memset(&flag,0,sizeof(flag));

        addr+=payload[1];
        addr<<=8;
        addr+=payload[0];

        flag.val=payload[2];

        bool IsReadSuccess=false;
        uint64_t dat=0;


        {
            if(ctx->ReadSensor!=NULL)
            {
                IsReadSuccess=ctx->ReadSensor(ctx,plies,addr,&dat,&flag);
            }
        }


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        if(IsReadSuccess)
        {
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_sensor_address_t)+sizeof(SMGS_payload_sensor_flag_t)+pow(2,flag.sensorlen);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }
            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);
            retpayload[3]=flag.val;

            for(size_t i=0; i<pow(2,flag.sensorlen); i++)
            {
                retpayload[4+i]=((dat>>(8*i))&0xFF);
            }

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            //读取失败
            uint8_t *retpayload=free_buff;
            size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t)+sizeof(SMGS_payload_sensor_address_t);

            if(free_buff_size<retpayloadlen)
            {
                break;
            }

            free_buff+=retpayloadlen;
            free_buff_size-=retpayloadlen;

            retpayload[0]=SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID;
            retpayload[1]=(addr&0xFF);
            retpayload[2]=((addr>>8)&0xFF);

            SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);

        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_READMULTISENSOR:
    {
        if(payloadlen < 3 || payloadlen%3 != 0 || payload==NULL)
        {
            break;
        }

        size_t success_count=0;


        uint8_t *free_buff=buff;
        size_t  free_buff_size=buff_size;

        uint8_t *retpayload=free_buff;
        size_t  retpayloadlen=sizeof(SMGS_payload_retcode_t);

        if(free_buff_size<retpayloadlen)
        {
            break;
        }
        free_buff_size-=retpayloadlen;
        free_buff+=retpayloadlen;

        {
            //读取传感器
            for(size_t i=0; i< (payloadlen/3) ; i++)
            {
                bool IsReadSuccess=false;
                uint64_t dat=0;
                SMGS_payload_sensor_address_t addr= 0;
                SMGS_payload_sensor_flag_t flag;
                memset(&flag,0,sizeof(flag));

                addr+=payload[3*i+1];
                addr<<=8;
                addr+=payload[3*i+0];

                flag.val=payload[3*i+2];

                {
                    if(ctx->ReadSensor!=NULL)
                    {
                        IsReadSuccess=ctx->ReadSensor(ctx,plies,addr,&dat,&flag);
                    }
                }

                if(IsReadSuccess)
                {
                    uint8_t *databuff=&retpayload[retpayloadlen];
                    size_t  databufflen=sizeof(SMGS_payload_sensor_address_t)+sizeof(SMGS_payload_sensor_flag_t)+pow(2,flag.sensorlen);

                    if(free_buff_size<databufflen)
                    {
                        break;
                    }
                    free_buff_size-=databufflen;
                    free_buff+=databufflen;

                    databuff[0]=((addr)&0xFF);
                    databuff[1]=(((addr)>>8)&0xFF);
                    databuff[2]=flag.val;

                    for(size_t j=0; j<pow(2,flag.sensorlen); j++)
                    {
                        databuff[3+j]=((dat>>(8*j))&0XFF);
                    }

                    retpayloadlen+=databufflen;

                    success_count++;

                }
            }
        }
        SMGS_buff_alloc(Buff,free_buff-buff);//锁定已使用的空间
        if(success_count>0)
        {
            retpayload[0]=SMGS_PAYLOAD_RETCODE_SUCCESS;
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
        else
        {
            retpayload[0]=SMGS_PAYLOAD_RETCODE_RESOURCE_INVALID;
            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpayloadlen,qos,retain,Buff);
        }
    }
    break;
    case SMGS_TOPIC_PLY_CMD_END:
    default:
        break;
    }

    return ret;
}

/*
网关处理BinReq消息。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],size_t plies_count,uint8_t *payload,size_t payloadlen,uint8_t qos,int retain,SMGS_buff_t *Buff)
{
    int Moddule=SMGS_Get_Topic_Ply_Module(plies[SMGS_TOPIC_PLY_MODULE]);
    bool ret=false;
    switch(Moddule)
    {
    case SMGS_TOPIC_PLY_MODULE_GATEWAY:
    {
        ret=SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay(ctx,plies,plies_count,payload,payloadlen,qos,retain,Buff);
    }
    break;
    default://默认为设备模块
    {
        bool IsDevicePosNumber=true;
        uint8_t Pos=0;
        {
            //检查
            for(size_t i=0; i<strlen(plies[SMGS_TOPIC_PLY_MODULE]); i++)
            {
                if(plies[SMGS_TOPIC_PLY_MODULE][i] < '0' || plies[SMGS_TOPIC_PLY_MODULE][i] > '9')
                {
                    IsDevicePosNumber=false;
                    break;
                }
            }

            int pos=atoi(plies[SMGS_TOPIC_PLY_MODULE]);

            if(pos <=0 || pos > 255)
            {
                IsDevicePosNumber=false;
            }

            Pos=pos;
        }

        SMGS_device_context_t *devctx=NULL;

        if(IsDevicePosNumber)
        {
            devctx=ctx->Device_Find_By_Pos(ctx,Pos);
        }
        else
        {
            devctx=ctx->Device_Find_By_SerialNumber(ctx,plies[SMGS_TOPIC_PLY_MODULE]);
        }


        if(devctx!=NULL)//只处理有效的设备模块
        {
            ret=SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device(ctx,devctx,plies,plies_count,payload,payloadlen,qos,retain,Buff);
        }
    }
    break;
    }


    return ret;
}

bool SMGS_GateWay_Receive_MQTT_MSG(SMGS_gateway_context_t *ctx,const char *topic,size_t topiclen,uint8_t *payload,size_t payloadlen,uint8_t qos,int retain,SMGS_buff_t *Buff)
{
    if(!SMGS_Is_GateWay_Context_OK(ctx))
    {
        return false;
    }

    if(topic==NULL || topiclen==0 || payload==NULL || payloadlen==0 || Buff==NULL)
    {
        return false;//参数有误
    }

    if(topiclen>CONFIG_SMGS_MAX_TOPIC_LENGTH)
    {
        return false;//大于最大主题大小
    }

    if(payloadlen> CONFIG_SMGS_MAX_PAYLOAD_LENGTH)
    {
        return false;//大于最大负载大小
    }

    bool ret=false;

    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END];
    memset(plies,0,sizeof(plies));

    if(!SMGS_Topic_Plies_Decode(plies,SMGS_TOPIC_PLY_END,Buff,topic,topiclen))
    {
        return false;
    }

    if(strcmp(plies[SMGS_TOPIC_PLY_DESTADDR],ctx->GateWaySerialNumber)!=0)
    {
        return false;//目的地址不正确
    }

    int comtype=SMGS_Get_Topic_Ply_ComType(plies[SMGS_TOPIC_PLY_COMTYPE]);
    //检查通信类型
    switch(comtype)
    {
    case SMGS_TOPIC_PLY_COMTYPE_BINREQ://只响应BINREQ
    {
        ret=SMGS_GateWay_Process_Comtype_BinReq(ctx,plies,SMGS_TOPIC_PLY_END,payload,payloadlen,qos,retain,Buff);
    }
    break;
    default:
        break;
    }



    return ret;
}

bool SMGS_GateWay_Will_Encode(SMGS_gateway_context_t *ctx,SMGS_gateway_will_t *will,SMGS_buff_t *Buff)
{
    if(will==NULL)
    {
        return false;
    }

    if(Buff==NULL)
    {
        return false;
    }

    if(!SMGS_Is_GateWay_Context_OK(ctx))
    {
        return false;
    }

    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END];
    memset(plies,0,sizeof(plies));

    plies[SMGS_TOPIC_PLY_DESTADDR]=CONFIG_SMGS_SERVER_DEFAULT_NAME; //目的地址
    plies[SMGS_TOPIC_PLY_SRCADDR]=ctx->GateWaySerialNumber;
    plies[SMGS_TOPIC_PLY_COMTYPE]=SMGS_Get_Topic_Ply_ComType_String(SMGS_TOPIC_PLY_COMTYPE_BINEVT);
    plies[SMGS_TOPIC_PLY_MODULE]=SMGS_Get_Topic_Ply_Module_String(SMGS_TOPIC_PLY_MODULE_GATEWAY);
    plies[SMGS_TOPIC_PLY_CMD]=SMGS_Get_Topic_Ply_CMD_String(SMGS_TOPIC_PLY_CMD_COMMAND);
    plies[SMGS_TOPIC_PLY_CMD_PARA_1]="offline";

    const char *topic=SMGS_Topic_Plies_EnCode(plies,SMGS_TOPIC_PLY_END,Buff);
    if(topic==NULL)
    {
        return false;
    }

    will->topic=topic;

    uint8_t *buff=SMGS_buff_alloc(Buff,sizeof(SMGS_payload_cmdid_t));
    if(buff==NULL)
    {
        return false;//buff不够
    }

    uint8_t *payload=&buff[0];
    payload[0]=(SMGS_GATEWAY_CMDID_OFFLINE&0xFF);
    payload[1]=((SMGS_GATEWAY_CMDID_OFFLINE>>8)&0xFF);

    will->payload=payload;
    will->payloadlen=sizeof(SMGS_payload_cmdid_t);

    will->qos=CONFIG_SMGS_GATEWAY_WILL_QOS;
    will->ratain=CONFIG_SMGS_GATEWAY_WILL_RETAIN;

    return true;

}
