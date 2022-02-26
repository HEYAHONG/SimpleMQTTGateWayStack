#include "InternalDatabase.h"
#include "wx/log.h"

sqlite3 *memdb=NULL;
void InternalDatabase_Init()
{
    int rc=0;
    if((rc=sqlite3_open(":memory:",&memdb))!=SQLITE_OK)
    {
        wxLogMessage(_T("初始化内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
    }

    {
        //创建表
        if((rc= sqlite3_exec(memdb,"CREATE TABLE ProgramInfo(KEY TEXT,VALUE TEXT);",NULL,NULL,NULL))!=SQLITE_OK)
        {
            wxLogMessage(_T("初始化内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
        }
    }
}


bool InternalDatebase_ProgramInfo_Set(wxString key,wxString value)
{
    if(memdb==NULL)
    {
        return false;
    }

    int rc=0;


    {
        //删除之前的数据
        wxString sql=((wxString)"DELETE FROM ProgramInfo WHERE KEY=\'")+key+"\';";
        sqlite3_exec(memdb,sql.ToAscii(),NULL,NULL,NULL);
    }

    {
        //插入信息
        wxString sql=((wxString)"INSERT INTO  ProgramInfo VALUES(\'")+key+"\',\'"+value+"\');";
        //操作表
        if((rc= sqlite3_exec(memdb,sql.ToAscii(),NULL,NULL,NULL))!=SQLITE_OK)
        {
            wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
            return false;
        }
    }


    return true;
}

static int InternalDatebase_ProgramInfo_Get_cb(void *para,int ncolumn,char ** columnvalue,char *columnname[])
{
    if(para==NULL)
    {
        return 0;
    }

    wxString &ret=(*(wxString *)para);

    int i;
    for(i = 0; i < ncolumn; i++)
    {
        if(wxString(_T("VALUE"))==columnname[i])
        {
            ret=columnvalue[i];
        }
    }
    return 0;
}

wxString InternalDatebase_ProgramInfo_Get(wxString key)
{
    wxString ret;

    if(memdb==NULL)
    {
        return ret;
    }

    int rc=0;

    {
        //查询信息
        wxString sql=((wxString)"SELECT * FROM ProgramInfo WHERE KEY=\'")+key+"\';";
        //操作表
        if((rc= sqlite3_exec(memdb,sql.ToAscii(),InternalDatebase_ProgramInfo_Get_cb,&ret,NULL))!=SQLITE_OK)
        {
            wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
            return ret;
        }
    }

    return ret;

}

bool InternalDatabase_Backup(wxString destpath)
{
    if(memdb==NULL)
    {
        return false;
    }

    sqlite3 *filedb=NULL;

    if(sqlite3_open(destpath.ToAscii(),&filedb)!=SQLITE_OK)
    {
        return false;
    }

    sqlite3_backup * filedb_bak=sqlite3_backup_init(filedb,"main",memdb,"main");
    {
        sqlite3_backup_step(filedb_bak,-1);
    }
    sqlite3_backup_finish(filedb_bak);

    if(filedb!=NULL)
    {
        sqlite3_close(filedb);
    }


    return true;
}


void InternalDatabase_Deinit()
{
    if(memdb!=NULL)
    {
        sqlite3_close(memdb);
    }
}
