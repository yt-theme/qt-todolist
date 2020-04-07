#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    create_systemTray();

    ui->setupUi(this);

//    net_acce_manage = new QNetworkAccessManager(this);

    sqlite_init();
}

MainWindow::~MainWindow()
{
    delete sqlite_db;
    delete ui;
}

// 系统托盘生成
void MainWindow::create_systemTray()
{
    systemTrayIcon = new QSystemTrayIcon(this);
    systemTrayIcon->setToolTip("todoList");

    QIcon trayico(":/new/prefix1/arch_yt_002.png");
    systemTrayIcon->setIcon(trayico);

    QMenu *trayMenu = new QMenu(this);

    // show main window
    QAction *showMWAction = new QAction("show", this);
    connect(showMWAction, &QAction::triggered, [=](){ this->show(); } );
    trayMenu->addAction(showMWAction);
    // exit
    QAction *exitAction = new QAction("exit", this);
    connect(exitAction, &QAction::triggered, [=](){ exit(0); } );
    trayMenu->addAction(exitAction);

    systemTrayIcon->setContextMenu(trayMenu);
    systemTrayIcon->show();

    connect(systemTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
}

// 托盘事件
void MainWindow::on_activatedSysTrayIcon (QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::Trigger:
            if (this->isVisible()) {
                this->hide();
            } else {
                this->show();
            }
        break;
        case QSystemTrayIcon::DoubleClick:

        break;
        default: break;
    }
}



// 获取用户token
void MainWindow::get_user_token (QString ip, QString username, QString password) {
    // 等待
//    QProgressDialog *progressDialog = new QProgressDialog(this);
//    progressDialog->setModal(true);
//    progressDialog->setRange(0, 100);
//    progressDialog->setValue(50);

    QNetworkAccessManager   *net_acce_manage = new QNetworkAccessManager(this);
    QString                 post_data_str = "{\"username\":\"" + username + "\", \"password\": \"" + password + "\"}";


    QNetworkRequest         net_req;
    net_req.setUrl(QUrl(ip));
    net_req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    net_req.setHeader(QNetworkRequest::ContentLengthHeader, post_data_str.length());
    net_acce_manage->post(net_req, post_data_str.toUtf8());

//    progressDialog->setValue(50);

    // 处理返回数据
    connect(net_acce_manage, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

//        progressDialog->setValue(100);

        if (code == 200) {
            QByteArray          data = reply->readAll();
            QJsonDocument       data_json = QJsonDocument::fromJson(data);
            QJsonObject         data_obj  = data_json.object();
            // 登录状态
            int stat = data_obj["stat"].toInt();
            if (stat == 1) {
                // 登录token
                QString token = data_obj["token"].toString();

                // token 存入数据库 (type=0)
                qDebug() << "存入数据库 =>";
                QString login_info_qr = QString("UPDATE %1 SET value='%2' WHERE type=0").arg(DB_LOGININFO_TABLENAME).arg(token);
                sqlite_db->exec(login_info_qr);

                qDebug() << "请求状态=>" << code << data_json << stat << token;

                QString login_info_query = QString("select * from %1").arg(DB_LOGININFO_TABLENAME);
                QSqlQuery query_ret = sqlite_db->query(login_info_query);
                while(query_ret.next()) {
                    int     type  = query_ret.value(0).toInt();
                    QString value = query_ret.value(1).toString();
                    qDebug() << "TYPE VALUE =>" << type << value;
                }


            }

        } else {

        }

        reply->deleteLater();
        reply->close();
    });
}

// 获取todoList数据 TODAY ALL_HISTORY
void MainWindow::get_todoList_data (QString ip, QString token, MODE mode, std::function<void(void)> callback) {
    // 用于等待请求
//    QEventLoop              loop;
//    QTimer                  timer;

    QNetworkAccessManager   *net_acce_manage = new QNetworkAccessManager(this);
    QNetworkRequest         net_req;

    QString                 post_data_str;
    switch (mode) {
        case TODAY:
            post_data_str = "{}";
        break;
        case ALL_HISTORY:
            post_data_str = "{}";
        break;
    }

//    timer.setSingleShot(true);

    net_req.setUrl(QUrl(ip));
    net_req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    net_req.setHeader(QNetworkRequest::ContentLengthHeader, post_data_str.length());
    net_req.setRawHeader("authorization", token.toUtf8());
    net_acce_manage->post(net_req, post_data_str.toUtf8());
    // 处理返回数据
    connect(net_acce_manage, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (code == 200) {
            QByteArray          data        = reply->readAll();
            QJsonDocument       data_json   = QJsonDocument::fromJson(data);
            QJsonObject         data_obj    = data_json.object();
            QJsonArray          data_array  = data_obj["data"].toArray();
            int                 stat        = data_obj["stat"].toInt();
            qDebug() << "获取todoList =>" << stat << data_array;
            // 存入数据库
            qDebug() << "todoList存入数据库 =>";
            // 事务
            QStringList todoList_qr;
            for (auto ite: data_array) {
                // 每项变成 json 对象
                QJsonObject tmp_obj = ite.toObject();

//                for (auto ite_obj: tmp_obj) {
//                    qDebug() << "todoList存入数据库 =>" << ite_obj.toString();

//                }

                todoList_qr.append( QString(
                            "REPLACE INTO %1(_id, user_id, user_name, content, start_time, end_time, overed_time, flag) VALUES ('%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9')")
                            .arg(DB_TODOLISTDATA_TABLENAME)
                            .arg(tmp_obj.take("_id"         ).toString())
                            .arg(tmp_obj.take("user_id"     ).toString())
                            .arg(tmp_obj.take("user_name"   ).toString())
                            .arg(tmp_obj.take("content"     ).toString())
                            .arg(tmp_obj.take("start_time"  ).toString())
                            .arg(tmp_obj.take("end_time"    ).toString())
                            .arg(tmp_obj.take("overed_time" ).toString())
                            .arg(tmp_obj.take("flag"        ).toString())
                        );
            }

            bool isOk = sqlite_db->transaction(todoList_qr);
            if (isOk) {
                callback();
            } else {

            }


        }
    });
}

// sqlite
void MainWindow::sqlite_init () {
    sqlite_db = new Sqlite_db;
    sqlite_db->connection("todoList");

    // ------------------- table init ---------------------

    bool loginInfo_isOk = sqlite_db->exec(DB_LOGININFO_SCHEMA);

    if (loginInfo_isOk) {
        // loginInfo表 数据初始化
        qDebug() << "loginInfo表 数据初始化 =>" << loginInfo_isOk;
        QStringList loginInfo_qr_list;
        loginInfo_qr_list.append( QString("insert into %1 (type, value) values(0, '')").arg(DB_LOGININFO_TABLENAME) );
        loginInfo_qr_list.append( QString("insert into %1 (type, value) values(1, '')").arg(DB_LOGININFO_TABLENAME) );
        loginInfo_qr_list.append( QString("insert into %1 (type, value) values(2, '')").arg(DB_LOGININFO_TABLENAME) );
        sqlite_db->transaction(loginInfo_qr_list);
    }

    // todoList表
    bool todoList_isOk = sqlite_db->exec(DB_TODOLISTDATA_SCHEMA);
    if (todoList_isOk) {
        qDebug() << "todoList 表初始化 =>" << todoList_isOk;
    }
}


// 窗口关闭拦截
void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    this->hide();
}

// 点击 login
void MainWindow::on_login_pressed()
{
    QString server_login_api    = QString("%1%2").arg(ui->lineEdit_serverip->text()).arg(GlobalData::getInstance()->get_webapi().login);
    QString username            = ui->lineEdit_username->text();
    QString password            = ui->lineEdit_password->text();
    get_user_token(server_login_api, username, password);

//    delete globaldata;
}

// 渲染待办列表
void MainWindow::render_todolist()
{
    qDebug() << "渲染 =>";

    // 清空 listWidget
    ui->listWidget->clear();
    // 获取数据库中todoList数据
    QString login_info_query = QString("select * from %1").arg(DB_TODOLISTDATA_TABLENAME);
    QSqlQuery query_ret = sqlite_db->query(login_info_query);
    while(query_ret.next()) {
        QString v0 = query_ret.value(0).toString();
        QString v1 = query_ret.value(1).toString();
        QString v2 = query_ret.value(2).toString();
        QString v3 = query_ret.value(3).toString();
        QString v4 = query_ret.value(4).toString();
        QString v5 = query_ret.value(5).toString();
        QString v6 = query_ret.value(6).toString();
        QString v7 = query_ret.value(7).toString();

        qDebug() << "TYPE VALUE =>" << v0 << v1 << v2<<v3<<v4<<v5<<v6<<v7;

        // ****************************************************************
        //                  渲染列表项
        // ****************************************************************
        QWidget     *widget       = new QWidget(this);
        widget->setFixedHeight(80);
        // 每个列表项 横向布局
        QHBoxLayout *mainLaylout  = new QHBoxLayout(this);
        // 每个列表项 左侧布局
        QVBoxLayout *leftLaylout  = new QVBoxLayout(this);
        // 每个列表项 右侧布局
        QVBoxLayout *rightLaylout = new QVBoxLayout(this);

        // left
        QLabel *leftLabel = new QLabel(v3, this);
        leftLabel->setFixedHeight(30);
        leftLaylout->addWidget(leftLabel);

        // rightx
        QLabel *rightLabel = new QLabel(v4, this);
        QLabel *rightLabel2 = new QLabel(v6, this);
        rightLabel->setFixedHeight(30);
        rightLabel2->setFixedHeight(30);
        rightLaylout->addWidget(rightLabel);
        rightLaylout->addWidget(rightLabel2);

        mainLaylout->addLayout(leftLaylout);
        mainLaylout->addLayout(rightLaylout);
        widget->setLayout(mainLaylout);

        QListWidgetItem     *widghtItem = new QListWidgetItem(ui->listWidget);
        QSize               size = widghtItem->sizeHint();
        widghtItem->setSizeHint(QSize(size.width(), 80));
        ui->listWidget->addItem(widghtItem);
        ui->listWidget->setItemWidget(widghtItem, widget);
    }


}

void MainWindow::on_sync_pressed()
{
    QString     token;

    // 查询 token
    QSqlQuery query_ret = sqlite_db->query(QString("SELECT value FROM %1 WHERE type = 0;").arg(DB_LOGININFO_TABLENAME));
    while(query_ret.next()) {
        token  = query_ret.value(0).toString();
    }
    // 请求 todoList 数据
    get_todoList_data(QString("%1%2").arg(ui->lineEdit_serverip->text()).arg(GlobalData::getInstance()->get_webapi().gettodolist), token, ALL_HISTORY, [=] () {
        // 渲染
        render_todolist();
    });
}
