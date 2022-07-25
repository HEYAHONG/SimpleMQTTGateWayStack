#include "libSMGSUtil.h"

SMGS_buff_t SMGS_build_buff(uint8_t *buff,size_t size)
{
    SMGS_buff_t Buff;
    Buff.start=buff;
    Buff.total=size;
    Buff.used=0;
    return Buff;
}

uint8_t *SMGS_buff_alloc(SMGS_buff_t *buff,size_t size)
{
    if(buff==NULL || buff->start==NULL || buff->total==0 || (buff->used+size)>buff->total)
    {
        return NULL;
    }

    uint8_t *ret=(buff->start+buff->used);
    buff->used+=size;
    return ret;
}
