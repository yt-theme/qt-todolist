#ifndef DEFINE_H
#define DEFINE_H

#include <QString>

typedef int MODE;                       // 模式类型
#define TODAY                       1   // 今天
#define ALL_HISTORY                 2   // 所有历史

#define DB_PATH                     ".local/share/todoList"     // 数据库目录路径


// ------------------------------- database -------------------------------------------
// 登录信息表
/*
 * -- 只存储单个用户的信息 --
 * type               value
 * 类型                值
 * 0: token
 * 1: 用户名
 * 2: 登录时间戳
*/

#define DB_LOGININFO_TABLENAME      "login_info"
#define DB_LOGININFO_SCHEMA         "CREATE TABLE " DB_LOGININFO_TABLENAME "(" \
                                        "type           INTEGER," \
                                        "value          TEXT" \
                                    ");"



// todoList数据表
/*
 * -- 存储多个用户的数据 --
 * content   start_time   end_time   overed_time   user_id   user_name   flag
 * 内容       开始时间戳     结束时间戳  完成或忽略时间   所属用户  所属的用户名   标志
*/
#define DB_TODOLISTDATA_TABLENAME   "todolist_data"
#define DB_TODOLISTDATA_SCHEMA      "CREATE TABLE " DB_TODOLISTDATA_TABLENAME "(" \
                                        "_id            TEXT PRIMARY KEY NOT NULL," \
                                        "user_id        TEXT," \
                                        "user_name      TEXT," \
                                        "content        TEXT," \
                                        "start_time     TEXT," \
                                        "end_time       TEXT," \
                                        "overed_time    TEXT," \
                                        "flag           TEXT" \
                                    ");"
// ------------------------------- database end ---------------------------------------


// ------------------------------- web api --------------------------------------------
// 登录
//#define API_LOGIN                   "/api/login"
//#define API_GETTODOLIST             "/api/queryTodoListByUserId"
// ------------------------------- web api end ----------------------------------------

#endif // DEFINE_H
