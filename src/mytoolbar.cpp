#include "mytoolbar.h"

mytoolbar::mytoolbar(QWidget *parent) :
    QToolBar(parent)
{

    simStart = new QAction(QIcon(QPixmap(":/icons/startsim.png")),tr("Start"),this);
    simPause = new QAction(QIcon(QPixmap(":/icons/pause.png")),tr("Pause"),this);
    simResume = new QAction(QIcon(QPixmap(":/icons/resume.png")),tr("Resume"),this);
    simStop = new QAction(QIcon(QPixmap(":/icons/proc_stop.png")),tr("Stop"),this);
    toogleControP = new QAction(QIcon(QPixmap(":/icons/cockpit.png")),tr("Control Panel"),this);
    toogleRecordP = new QAction(QIcon(QPixmap(":/icons/record.png")),tr("Record Panel"),this);
    toogleControP->setCheckable(true);
    toogleRecordP->setCheckable(true);
    this->setFocusPolicy(Qt::NoFocus);
    this->addAction(simStart);
    this->addAction(simStop);
    this->addAction(simPause);
    this->addAction(simResume);
    this->addSeparator();
    QLabel *spacer = new QLabel();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->addWidget(spacer);
    this->addSeparator();
    this->addAction(toogleControP);
    this->addAction(toogleRecordP);
    simPause->setVisible(false);
    simStop->setVisible(false);
    simResume->setVisible(false);
}
/*!
 * \brief mytoolbar::getSimStatus updates the button to match a simulation status
 * \param status status, 0 - paused, 1 - simulating, -1 stopped
 */
void mytoolbar::getSimStatus(int status)
{
    if(status == 0){
        simPause->setVisible(false);
        simStart->setVisible(false);
        simStop->setVisible(true);
        simResume->setVisible(true);
    }else if(status == -1){
        simPause->setVisible(false);
        simStart->setVisible(true);
        simStop->setVisible(false);
        simResume->setVisible(false);
    }else{
        simPause->setVisible(true);
        simStart->setVisible(false);
        simStop->setVisible(true);
        simResume->setVisible(false);
    }
}
void mytoolbar::cpanelClosed()
{
    this->toogleControP->setChecked(false);
}
void mytoolbar::cpanelOpened()
{
    this->toogleControP->setChecked(true);
}
void mytoolbar::rpanelClosed()
{
    this->toogleRecordP->setChecked(false);
}
void mytoolbar::rpanelOpened()
{
    this->toogleRecordP->setChecked(true);
}
