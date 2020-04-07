#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <functional>
#include <QThread>
#include <QProgressDialog>
#include <QDebug>

class Progress_dialog: QThread
{
    Q_OBJECT
public:
    Progress_dialog();
    ~Progress_dialog();

    void run();
    int i;
};

#endif // CALLBACKS_H
