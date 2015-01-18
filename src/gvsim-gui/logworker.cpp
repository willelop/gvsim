#include "logworker.h"

logWorker::logWorker(QObject *parent) :
    QObject(parent)
{
    logfile = new QFile("gvsim.log");
    timer = new QElapsedTimer();
    timer->start();
    logfile->open(QIODevice::Append | QIODevice::Text);
    QTextStream outStream(logfile);
    outStream << uptime();
    outStream << "GVSim Log Started";
    outStream << "\n";
    logfile->close();
}
/*!
 * \brief logWorker::timeStamp get the current timestamp text string
 * \return the text string with current timestamp
 */
QString logWorker::timeStamp()
{
    QString time;
    time.append("[");
    time.append(QTime::currentTime().toString().toLatin1().data());
    time.append("] ");
    return time;
}
/*!
 * \brief logWorker::uptime request the program uptime
 * \return text string with uptime in seconds with 3 decimal numbers
 */
QString logWorker::uptime()
{
    QString time;
    time.sprintf("[%08.3f]",(float)timer->elapsed()*0.001f);
    return time;
}
/*!
 * \brief logWorker::write2log writes a QString to the log file and shows it in the apropriate widget.
 * \param message text to log
 * \param type log type
 */
void logWorker::write2log(QString message, LOG::TYPE type)
{
    QString preChain;
    if(type == LOG::ERRORMSG){
        preChain = " ERROR: ";
    }else if(type == LOG::INFO){
        preChain = " INFO: ";
    }else{
        preChain = " ";
    }
    logfile->open(QIODevice::Append | QIODevice::Text);
    QTextStream outStream(logfile);
    outStream << uptime();
    outStream << preChain;
    outStream << message;
    outStream << "\n";
    logfile->close();
    emit messagePrinted(uptime()+preChain+message, type);
}
