#ifndef MYEVENTFILTER_H
#define MYEVENTFILTER_H

#include <QObject>
#include <QDebug>
#include <QEvent>
#include <QWidget>
#include <QKeyEvent>
#include <QApplication>
/*!
 * \brief The myEventFilter class filters the keyboard presses to allow them to be used as controls
 */

class myEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit myEventFilter(QObject *parent = 0);

signals:
    void keyPress(QKeyEvent*);  ///< \brief emits a new keypress event
public slots:
protected:
    bool eventFilter(QObject *object, QEvent *event);

};

#endif // MYEVENTFILTER_H
