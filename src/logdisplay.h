#ifndef LOGDISPLAY_H
#define LOGDISPLAY_H

#include <QTextEdit>
#include "typedefs.h"

/*!
 * \brief The logDisplay class a text widget displaying logged messages using a coloured system
 */
class logDisplay : public QTextEdit
{
    Q_OBJECT
public:
    explicit logDisplay(QWidget *parent = 0);
    
signals:
    
public slots:
    void showMessage(QString message,LOG::TYPE);
    
};

#endif // LOGDISPLAY_H
