#include "logdisplay.h"

logDisplay::logDisplay(QWidget *parent) :
    QTextEdit(parent)
{
    this->setReadOnly(true);
}
/*!
 * \brief logDisplay::showMessage prints a message in the screen
 * \param message message to display
 * \param type type of the message, used to colour it.
 */
void logDisplay::showMessage(QString message,LOG::TYPE type)
{
    if(type == LOG::ERRORMSG)
    {
        this->setTextColor(Qt::red);
    }else if(type == LOG::INFO){
        this->setTextColor(Qt::darkGreen);
    }else{
        this->setTextColor(Qt::black);
    }
    this->append(message);
}
