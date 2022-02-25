#ifndef INTERNALDATABASE_H_INCLUDED
#define INTERNALDATABASE_H_INCLUDED

#include "sqlite3.h"
#include "stdbool.h"
#include "wx/string.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//内部数据库初始化
void InternalDatabase_Init();

//备份内部数据库
bool InternalDatabase_Backup(wxString destpath);

//程序信息设定
bool InternalDatebase_ProgramInfo_Set(wxString key,wxString value);

//内部数据库反初始化
void InternalDatabase_Deinit();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INTERNALDATABASE_H_INCLUDED
