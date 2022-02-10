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

