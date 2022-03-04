#ifndef INTERNALDATABASE_H_INCLUDED
#define INTERNALDATABASE_H_INCLUDED

#include "sqlite3.h"
#include "stdbool.h"
#include "wx/string.h"
#include "wx/arrstr.h"
#include "map"
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

//程序信息获取
wxString InternalDatebase_ProgramInfo_Get(wxString key);

//内部数据库反初始化
void InternalDatabase_Deinit();



//检查表名是否有效
bool InternalDatabase_Is_Table_Valied(wxString Table_Name);

/*
创建表.
由于sqlite3为动态类型(可不声明类型),参数不包含类型
*/
bool InternalDatabase_Create_Table(wxString Table_Name,wxArrayString Header);


/*
查询表的所有数据。返回map，key为列名,value为该列的所有数据。
*/
std::map<wxString,wxArrayString> InternalDatabase_Table_Get_AllData(wxString Table_Name);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INTERNALDATABASE_H_INCLUDED
