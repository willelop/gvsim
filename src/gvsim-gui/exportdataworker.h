#ifndef EXPORTDATAWORKER_H
#define EXPORTDATAWORKER_H

#include <QObject>
#include <QTextStream>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include "planestatusdata.h"
#include "typedefs.h"
/*!
 * \brief The exportDataWorker class is a class that processes the export to CSV process in background.
 */
class exportDataWorker : public QObject
{
    Q_OBJECT
public:
    explicit exportDataWorker(QObject *parent = 0);
private:
    int tot_size;
    int playEntities;
    QString getValue(QString chain, planeStatusData _data );
public slots:
    void startExport(QString,QString,QStringList);
signals:
    void updateStatus(int progress, int total);
    void logMessage(QString,LOG::TYPE);
    void finished(bool status);
};

#endif // EXPORTDATAWORKER_H
