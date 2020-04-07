#include "virtual.h"
#include <QProgressDialog>

Progress_dialog::Progress_dialog ()
{

}

Progress_dialog::~Progress_dialog ()
{

}

void Progress_dialog::run ()
{
    this->i += 1;
    qDebug() << "线程 =>" << i;
}
