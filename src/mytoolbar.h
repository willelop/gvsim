#ifndef MYTOOLBAR_H
#define MYTOOLBAR_H

#include <QWidget>
#include <QAction>
#include <QToolBar>
#include <QSlider>
#include <QSizePolicy>
#include <QLabel>
#include <QDebug>
/*!
 * \brief The mytoolbar class is the main toolbar of the application, allowing the user to easily start, pause or stop de simulation
 */
class mytoolbar : public QToolBar
{
    Q_OBJECT
public:
    explicit mytoolbar(QWidget *parent = 0);
    QAction *simStart;
    QAction *simPause;
    QAction *simStop;
    QAction *simResume;
    QAction *toogleControP;
    QAction *toogleRecordP;
public slots:
    void getSimStatus(int status);
    void cpanelClosed();
    void cpanelOpened();
    void rpanelOpened();
    void rpanelClosed();
};

#endif // mytoolbar_H
