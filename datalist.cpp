#include "datalist.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

Datalist::Datalist()
{

}

// 列表项 item
QWidget* Datalist::create_dataListItem () {

    QWidget     *widget       = new QWidget(__this__);
    widget->setFixedHeight(80);
    // 每个列表项 横向布局
    QHBoxLayout *mainLaylout  = new QHBoxLayout(__this__);
    // 每个列表项 左侧布局
    QVBoxLayout *leftLaylout  = new QVBoxLayout(__this__);
    // 每个列表项 右侧布局
    QVBoxLayout *rightLaylout = new QVBoxLayout(__this__);

    // left
    QLabel *leftLabel = new QLabel("left", __this__);
    QLabel *leftLabel2 = new QLabel("left down", __this__);
    leftLabel->setFixedHeight(30);
    leftLabel2->setFixedHeight(30);
    leftLaylout->addWidget(leftLabel);
    leftLaylout->addWidget(leftLabel2);

    // rightx
    QLabel *rightLabel = new QLabel("right", __this__);
    QLabel *rightLabel2 = new QLabel("right down", __this__);
    rightLabel->setFixedHeight(30);
    rightLabel2->setFixedHeight(30);
    rightLaylout->addWidget(rightLabel);
    rightLaylout->addWidget(rightLabel2);

    mainLaylout->addLayout(leftLaylout);
    mainLaylout->addLayout(rightLaylout);
    widget->setLayout(mainLaylout);

    return widget;
};

//// 列表生成
//void Datalist::create_dataList () {
//    QListWidgetItem     *widghtItem = new QListWidgetItem();
//    QSize               size = widghtItem->sizeHint();
//    QWidget             *item = create_dataListItem();
//    widghtItem->setSizeHint(QSize(size.width(), 80));
//    ui->listWidget->addItem(widghtItem);
//    ui->listWidget->setItemWidget(widghtItem, item);

//    QListWidgetItem     *widghtItem2 = new QListWidgetItem();
//    QSize               size2 = widghtItem2->sizeHint();
//    QWidget             *item2 = create_dataListItem();
//    widghtItem2->setSizeHint(QSize(size2.width(), 80));
//    ui->listWidget->addItem(widghtItem2);
//    ui->listWidget->setItemWidget(widghtItem2, item2);
//}
