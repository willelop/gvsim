#ifndef LOGWORKER_H
#define LOGWORKER_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QTime>
#include <QElapsedTimer>
#include "typedefs.h"
/*!
 * \brief The logWorker class logs to a file text messages. The class receives
 * this messages via two slots (one for normal messages and other for errors)
 *  and emits a signal when successfully logued.
 */
class logWorker : public QObject
{
    Q_OBJECT
public:
    explicit logWorker(QObject *parent = 0);
private:
    QFile *logfile;
    QElapsedTimer *timer;
    QString timeStamp();
    QString uptime();
signals:
    void messagePrinted(QString,LOG::TYPE); ///< \brief emits a signal when a message is logued
public slots:
    void write2log(QString message, LOG::TYPE type);
};

#endif // LOGWORKER_H
