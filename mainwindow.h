#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QSize>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QString>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QProgressBar>
#include <QProgressDialog>
#include <QEventLoop>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QDebug>

#include <functional>

#include "sqlite_db.h"
#include "globaldata.h"
#include "datalist.h"
//#include "virtual.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void                    create_systemTray();

    // network data
    void                    get_user_token(QString ip, QString name, QString password);         // 获取用户token操作
    void                    get_todoList_data(QString ip, QString token, MODE mode, std::function<void(void)> callback);            // 获取待办事项列表 mode: TODAY ALL_HISTORY

    void                    render_todolist();                                                  // 渲染待办列表



private:
    Ui::MainWindow          *ui;

    // 窗口关闭事件
    void                    closeEvent(QCloseEvent *);

    // ******************************************************************************************************
    //                 data and var obj
    // ******************************************************************************************************
    // userdata
    QString                 user_token;                                                         // 用户token
    QJsonArray              todoList_data;                                                      // 待办事项列表
    // sqlite
    Sqlite_db               *sqlite_db;                                                         // 数据库
    // tray
    QSystemTrayIcon         *systemTrayIcon;


    // ******************************************************************************************************
    //                 func
    // ******************************************************************************************************
    // sqlite
    void                    sqlite_init();                                                      // 数据库初始化
    // ******************************************************************************************************

private slots:
    void                    on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void                    on_login_pressed();
    void on_sync_pressed();
};
#endif // MAINWINDOW_H
