#include "globaldata.h"

GlobalData::GlobalData()
{

}

GlobalData* GlobalData::getInstance()
{
    static GlobalData globaldata;
    return &globaldata;
}

web_api& GlobalData::get_webapi()
{
    return webapi;
}
