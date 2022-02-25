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

    wxString sql=((wxString)"INSERT INTO  ProgramInfo VALUES(\'")+key+"\',\'"+value+"\');";
    //操作表
    if((rc= sqlite3_exec(memdb,sql.ToAscii(),NULL,NULL,NULL))!=SQLITE_OK)
    {
        wxLogMessage(_T("操作内部数据库出错(%d:%s)!"),rc,sqlite3_errstr(rc));
        return false;
    }


    return true;
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
