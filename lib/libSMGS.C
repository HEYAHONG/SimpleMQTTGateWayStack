/** \file libSMGS.c
 *  \brief     简易MQTT网关协议栈C源代码
 *  \author    何亚红
 *  \version   20220208
 *  \date      2022
 *  \copyright MIT License.
 */


#include "libSMGS.h"


bool SMGS_Topic_Plies_Decode(SMGS_topic_string_ptr_t plies[],size_t max_plies_count,uint8_t *buff,size_t buff_size,const char * topic,size_t topic_length)
{
    //参数检查
    if(plies==NULL || max_plies_count==0 || buff ==NULL || buff_size < (topic_length+1) || topic==NULL || topic_length == 0)
    {
        return false;
    }

    memset(buff,0,buff_size);

    memset(plies,0,max_plies_count);

    strcpy((char *)buff,topic);

    size_t current_index=0;

    plies[current_index]=(const char *)buff;

    current_index++;

    for(size_t i=0; i<buff_size; i++)
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



const char * SMGS_Topic_Plies_EnCode(SMGS_topic_string_ptr_t plies[],size_t plies_count,uint8_t *buff,size_t buff_size)
{
    const char * ret=NULL;

    if(plies==NULL || plies[0]==NULL || plies_count == 0 || buff == NULL || buff_size == 0)
    {
        return ret;
    }

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

        if(need_buff_size>buff_size)
        {
            return ret;
        }
    }

    //填写字符串
    {
        memset(buff,0,buff_size);

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


bool SMGS_GateWay_Send_GateWay_Event(SMGS_gateway_context_t *ctx,const char *cmd_para_1,const char * cmd_para_2,const char * cmd_para_3,SMGS_payload_cmdid_t cmdid,void *cmddata,size_t cmddata_length,uint8_t *buff,size_t buff_size,uint8_t qos,int retian)
{
    if(buff==NULL || buff_size == 0)
    {
        return false;
    }

    if(!SMGS_Is_GateWay_Context_OK(ctx))
    {
        return false;
    }

    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END]= {0};

    plies[SMGS_TOPIC_PLY_DESTADDR]=CONFIG_SMGS_SERVER_DEFAULT_NAME; //目的地址
    plies[SMGS_TOPIC_PLY_SRCADDR]=ctx->GateWaySerialNumber;
    plies[SMGS_TOPIC_PLY_COMTYPE]=SMGS_Get_Topic_Ply_ComType_String(SMGS_TOPIC_PLY_COMTYPE_BINEVT);
    plies[SMGS_TOPIC_PLY_MODULE]=SMGS_Get_Topic_Ply_Module_String(SMGS_TOPIC_PLY_MODULE_GATEWAY);
    plies[SMGS_TOPIC_PLY_CMD]=SMGS_Get_Topic_Ply_CMD_String(SMGS_TOPIC_PLY_CMD_COMMAND);
    plies[SMGS_TOPIC_PLY_CMD_PARA_1]=cmd_para_1;
    plies[SMGS_TOPIC_PLY_CMD_PARA_2]=cmd_para_2;
    plies[SMGS_TOPIC_PLY_CMD_PARA_3]=cmd_para_3;

    const char *topic=SMGS_Topic_Plies_EnCode(plies,SMGS_TOPIC_PLY_END,buff,buff_size);
    if(topic==NULL)
    {
        return false;
    }

    //检查剩余buff大小
    if((sizeof(cmdid)+cmddata_length) > (buff_size-(strlen(topic)+1)))
    {
        return false;//buff不够
    }

    uint8_t * payload=&buff[strlen(topic)+1];
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


bool SMGS_GateWay_Online(SMGS_gateway_context_t *ctx,uint8_t *buff,size_t buff_size,uint8_t qos,int retian)
{
    //发送上线消息
    if(!SMGS_GateWay_Send_GateWay_Event(ctx,"online",NULL,NULL,SMGS_GATEWAY_CMDID_ONLINE,NULL,0,buff,buff_size,qos,retian))
    {
        return false;
    }

    //上报设备表
    if(ctx->Device_Next!=NULL)
    {
        SMGS_device_context_t *devctx=NULL;
        while((devctx=ctx->Device_Next(ctx,devctx))!=NULL)
        {
            if(SMGS_Is_Device_Context_OK(devctx))
            {
                size_t cmddatalen=strlen(devctx->DeviceSerialNumber)+2;
                if(cmddatalen>buff_size)
                {
                    continue;
                }
                memset(buff,0,cmddatalen);

                //填写位置号
                buff[0]=devctx->DevicePosNumber;

                //复制字符串
                strcpy((char *)&buff[1],devctx->DeviceSerialNumber);

                if(!SMGS_GateWay_Send_GateWay_Event(ctx,NULL,NULL,NULL,SMGS_GATEWAY_CMDID_REPORT_DEVICETABLE_ONLINE,buff,cmddatalen,&buff[cmddatalen],buff_size-cmddatalen,qos,retian))
                {
                    return false;
                }


            }
        }
    }


    return true;
}

bool SMGS_GateWay_Send_Device_Event(SMGS_gateway_context_t *ctx,SMGS_device_context_t *devctx,const char *cmd_para_1,const char * cmd_para_2,const char * cmd_para_3,SMGS_payload_cmdid_t cmdid,void *cmddata,size_t cmddata_length,uint8_t *buff,size_t buff_size,uint8_t qos,int retian)
{
    if(buff==NULL || buff_size == 0)
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

    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END]= {0};

    plies[SMGS_TOPIC_PLY_DESTADDR]=CONFIG_SMGS_SERVER_DEFAULT_NAME; //目的地址
    plies[SMGS_TOPIC_PLY_SRCADDR]=ctx->GateWaySerialNumber;
    plies[SMGS_TOPIC_PLY_COMTYPE]=SMGS_Get_Topic_Ply_ComType_String(SMGS_TOPIC_PLY_COMTYPE_BINEVT);
    plies[SMGS_TOPIC_PLY_MODULE]=devctx->DeviceSerialNumber;
    plies[SMGS_TOPIC_PLY_CMD]=SMGS_Get_Topic_Ply_CMD_String(SMGS_TOPIC_PLY_CMD_COMMAND);
    plies[SMGS_TOPIC_PLY_CMD_PARA_1]=cmd_para_1;
    plies[SMGS_TOPIC_PLY_CMD_PARA_2]=cmd_para_2;
    plies[SMGS_TOPIC_PLY_CMD_PARA_3]=cmd_para_3;

    const char *topic=SMGS_Topic_Plies_EnCode(plies,SMGS_TOPIC_PLY_END,buff,buff_size);
    if(topic==NULL)
    {
        return false;
    }

    //检查剩余buff大小
    if((sizeof(cmdid)+cmddata_length) > (buff_size-(strlen(topic)+1)))
    {
        return false;//buff不够
    }

    uint8_t * payload=&buff[strlen(topic)+1];
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
static bool SMGS_GateWay_Reply_Comtype_BinReq(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t src_plies[],size_t src_plies_count,uint8_t *retpayload,size_t retpayloadlen,uint8_t qos,int retain,uint8_t *buff,size_t buff_size)
{
    src_plies[SMGS_TOPIC_PLY_DESTADDR]=src_plies[SMGS_TOPIC_PLY_SRCADDR];//目的地址为源地址
    src_plies[SMGS_TOPIC_PLY_SRCADDR]=ctx->GateWaySerialNumber;//源地址为网关序列号
    src_plies[SMGS_TOPIC_PLY_COMTYPE]=SMGS_Get_Topic_Ply_ComType_String(SMGS_TOPIC_PLY_COMTYPE_BINRESP);//通信类型为二进制回复

    const char * topic=SMGS_Topic_Plies_EnCode(src_plies,src_plies_count,buff,buff_size);

    if(topic!=NULL && ctx->MessagePublish!=NULL)
    {
        return ctx->MessagePublish(ctx,topic,retpayload,retpayloadlen,qos,retain);
    }

    return false;
}


/*
网关处理设备模块的BinReq。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device(SMGS_gateway_context_t *ctx,SMGS_device_context_t *devctx,SMGS_topic_string_ptr_t plies[],size_t plies_count,uint8_t *payload,size_t payloadlen,uint8_t qos,int retain,uint8_t *buff,size_t buff_size)
{
    bool ret=false;
    switch(SMGS_Get_Topic_Ply_CMD(plies[SMGS_TOPIC_PLY_CMD]))
    {

    case SMGS_TOPIC_PLY_CMD_END:
    default:
        break;
    }

    return ret;
}

/*
网关处理网关模块的BinReq的内部命令。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_Command(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],size_t plies_count,SMGS_payload_cmdid_t cmdid,uint8_t *cmddata,size_t cmddatalen,uint8_t qos,int retain,uint8_t *buff,size_t buff_size)
{
    bool ret=false;



    return ret;
}

/*
网关处理网关模块的BinReq。参数必须有效
*/
static bool SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],size_t plies_count,uint8_t *payload,size_t payloadlen,uint8_t qos,int retain,uint8_t *buff,size_t buff_size)
{
    bool ret=false;

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
            ret=SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay_CMD_Internal_Command(ctx,plies,plies_count,cmdid,&payload[2],payloadlen-2,qos,retain,buff,buff_size);
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
                if(ctx->Command(ctx,&cmdid,&payload[2],payloadlen-2,retbuff,&retbufflen,&retcode))
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

                            ret=SMGS_GateWay_Reply_Comtype_BinReq(ctx,plies,plies_count,retpayload,retpaylodlen,qos,retain,free_buff,free_buff_size);

                        }

                    }
                }
            }
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
static bool SMGS_GateWay_Process_Comtype_BinReq(SMGS_gateway_context_t *ctx,SMGS_topic_string_ptr_t plies[],size_t plies_count,uint8_t *payload,size_t payloadlen,uint8_t qos,int retain,uint8_t *buff,size_t buff_size)
{
    int Moddule=SMGS_Get_Topic_Ply_Module(plies[SMGS_TOPIC_PLY_MODULE]);
    bool ret=false;
    switch(Moddule)
    {
    case SMGS_TOPIC_PLY_MODULE_GATEWAY:
    {
        ret=SMGS_GateWay_Process_Comtype_BinReq_Modbule_GateWay(ctx,plies,plies_count,payload,payloadlen,qos,retain,buff,buff_size);
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
            ret=SMGS_GateWay_Process_Comtype_BinReq_Modbule_Device(ctx,devctx,plies,plies_count,payload,payloadlen,qos,retain,buff,buff_size);
        }
    }
    break;
    }


    return ret;
}

bool SMGS_GateWay_Receive_MQTT_MSG(SMGS_gateway_context_t *ctx,const char *topic,size_t topiclen,uint8_t *payload,size_t payloadlen,uint8_t qos,int retain,uint8_t *buff,size_t buff_size)
{
    if(!SMGS_Is_GateWay_Context_OK(ctx))
    {
        return false;
    }

    if(topic==NULL || topiclen==0 || payload==NULL || payloadlen==0 || buff==NULL || buff_size==0)
    {
        return false;//参数有误
    }

    if(topiclen>CONFIG_SMGS_MAX_TOPIC_LENGTH)
    {
        return false;//大于最大主题大小
    }

    size_t    free_buff_size=buff_size;//空闲的buff大小
    uint8_t *free_buff_start=buff;//空闲buff指针
    bool ret=false;

    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END]= {0};

    if(!SMGS_Topic_Plies_Decode(plies,SMGS_TOPIC_PLY_END,free_buff_start,free_buff_size,topic,topiclen))
    {
        return false;
    }

    if(free_buff_size<(topiclen+1))
    {
        return false;//剩余缓冲不够
    }

    //修改剩余buff大小
    free_buff_size  -=(topiclen+1);
    free_buff_start +=(topiclen+1);

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
        ret=SMGS_GateWay_Process_Comtype_BinReq(ctx,plies,SMGS_TOPIC_PLY_END,payload,payloadlen,qos,retain,free_buff_start,free_buff_size);
    }
    break;
    default:
        break;
    }



    return ret;
}

bool SMGS_GateWay_Will_Encode(SMGS_gateway_context_t *ctx,SMGS_gateway_will_t *will,uint8_t *buff,size_t buff_size)
{
    if(will==NULL)
    {
        return false;
    }

    if(buff==NULL || buff_size == 0)
    {
        return false;
    }

    if(!SMGS_Is_GateWay_Context_OK(ctx))
    {
        return false;
    }

    SMGS_topic_string_ptr_t plies[SMGS_TOPIC_PLY_END]= {0};

    plies[SMGS_TOPIC_PLY_DESTADDR]=CONFIG_SMGS_SERVER_DEFAULT_NAME; //目的地址
    plies[SMGS_TOPIC_PLY_SRCADDR]=ctx->GateWaySerialNumber;
    plies[SMGS_TOPIC_PLY_COMTYPE]=SMGS_Get_Topic_Ply_ComType_String(SMGS_TOPIC_PLY_COMTYPE_BINEVT);
    plies[SMGS_TOPIC_PLY_MODULE]=SMGS_Get_Topic_Ply_Module_String(SMGS_TOPIC_PLY_MODULE_GATEWAY);
    plies[SMGS_TOPIC_PLY_CMD]=SMGS_Get_Topic_Ply_CMD_String(SMGS_TOPIC_PLY_CMD_COMMAND);
    plies[SMGS_TOPIC_PLY_CMD_PARA_1]="offline";

    const char *topic=SMGS_Topic_Plies_EnCode(plies,SMGS_TOPIC_PLY_END,buff,buff_size);
    if(topic==NULL)
    {
        return false;
    }

    will->topic=topic;


    if(sizeof(SMGS_payload_cmdid_t)> (buff_size-(strlen(topic)+1)))
    {
        return false;//buff不够
    }

    uint8_t *payload=&buff[strlen(topic)+1];
    payload[0]=(SMGS_GATEWAY_CMDID_OFFLINE&0xFF);
    payload[1]=((SMGS_GATEWAY_CMDID_OFFLINE>>8)&0xFF);

    will->payload=payload;
    will->payloadlen=sizeof(SMGS_payload_cmdid_t);

    will->qos=CONFIG_SMGS_GATEWAY_WILL_QOS;
    will->ratain=CONFIG_SMGS_GATEWAY_WILL_RETAIN;

    return true;

}
