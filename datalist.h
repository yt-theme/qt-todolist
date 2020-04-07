#ifndef DATALIST_H
#define DATALIST_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QDebug>

class Datalist
{
public:
    Datalist(QWidget* _this) : __this__(_this){};
    Datalist();

    // frame
    QWidget*                create_dataListItem();
//    void                    create_dataList();

private:
    QWidget* __this__ = nullptr;
};

#endif // DATALIST_H
