#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QString>

#include "define.h"

namespace Global {
class   GlobalData;
struct  web_api;
}

struct web_api {
    QString                 login;
    QString                 gettodolist;
};

class GlobalData
{
public:
    // 获取此对象实例
    static GlobalData* getInstance();
    // func
    web_api& get_webapi();
private:
    GlobalData();

    struct web_api
    webapi = {
        .login              = "/api/login",
        .gettodolist        = "/api/queryTodoListByUserId",
    };

};

#endif // GLOBALDATA_H
