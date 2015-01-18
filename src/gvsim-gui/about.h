#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
/*!
 * \brief The about class widget to show the information about the application
 */
class about : public QDialog
{
    Q_OBJECT
public:
    explicit about(QWidget *parent = 0);
private:
    QLabel *text;
    QLabel *author;
signals:
    
public slots:
    
};

#endif // ABOUT_H
