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

/** \brief 备份数据到指定路径
 *
 * \param destpath 目标路径
 * \return 是否成功
 *
 */
bool InternalDatabase_Backup(wxString destpath);

//程序信息设定

/** \brief 设定程序信息
 *
 * \param key 键
 * \param value 值
 * \return 是否成功
 *
 */
bool InternalDatebase_ProgramInfo_Set(wxString key,wxString value);

//程序信息获取
/** \brief 获取程序信息
 *
 * \param key 键
 * \return 值
 *
 */
wxString InternalDatebase_ProgramInfo_Get(wxString key);

//内部数据库反初始化
/** \brief 数据库反初始化
 *
 * \return
 *
 */
void InternalDatabase_Deinit();



//检查表名是否有效
/** \brief 表是否有效
 *
 * \param Table_Name 表名
 * \return 是否成功
 *
 */
bool InternalDatabase_Is_Table_Valied(wxString Table_Name);

/*
创建表.
由于sqlite3为动态类型(可不声明类型),参数不包含类型
*/
/** \brief 创建表
 *
 * \param Table_Name 表名
 * \param Header 表头
 * \return 是否成功
 *
 */
bool InternalDatabase_Create_Table(wxString Table_Name,wxArrayString Header);


/*
查询表的所有数据。返回map，key为列名,value为该列的所有数据。输入的条件数据为map,key为列名,value为对应的数据
*/

/** \brief 获取符合条件的数据
 *
 * \param Table_Name 表名
 * \param Condition 条件
 * \return 数据
 *
 */
std::map<wxString,wxArrayString> InternalDatabase_Table_Get_AllData(wxString Table_Name,std::map<wxString,wxString> Condition = std::map<wxString,wxString>());


/*
向表插入数据。输入的数据为map,key为列名,value为对应的数据
*/
/** \brief 向表插入数据
 *
 * \param Table_Name 表名
 * \param Data 数据
 * \return 是否成功
 *
 */
bool InternalDatabase_Table_Insert_Data(wxString Table_Name,std::map<wxString,wxString> Data);

/*
删除数据。输入的条件数据为map,key为列名,value为对应的数据
*/
/** \brief 删除表数据
 *
 * \param Table_Name 表名
 * \param Condition 条件
 * \return 是否成功
 *
 */
bool InternalDatabase_Table_Delete_Data(wxString Table_Name,std::map<wxString,wxString> Condition);

/*
更新数据。
*/
/** \brief 更新表数据
 *
 * \param Table_Name 表名
 * \param Data 数据
 * \param Condition 条件
 * \return 是否成功
 *
 */
bool InternalDatabase_Table_Update_Data(wxString Table_Name,std::map<wxString,wxString> Data,std::map<wxString,wxString> Condition=std::map<wxString,wxString>());

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INTERNALDATABASE_H_INCLUDED
