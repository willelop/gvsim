#include "myeventfilter.h"

myEventFilter::myEventFilter(QObject *parent) :
    QObject(parent)
{
}
bool myEventFilter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
      {
        QKeyEvent *keyevent = (QKeyEvent*)event;
        emit keyPress(keyevent);
        return QCoreApplication::instance()->eventFilter(object, event);
    }else{
        return QCoreApplication::instance()->eventFilter(object, event);
    }
}
