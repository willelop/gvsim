#include "centerwidget.h"

centerWidget::centerWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainlayout = new QVBoxLayout();
    datacontainer01 = new QTabWidget();
    numericaldata01 = new numericalData();
    graphicaldata01 = new graphicalData();
    charts01 = new chartsData();
    datacontainer01->addTab(numericaldata01,tr("Numerical Data"));
    datacontainer01->addTab(graphicaldata01,tr("Graphical Data"));
    datacontainer01->addTab(charts01,tr("Live Graphs"));
    mainlayout->addWidget(datacontainer01,2);
    this->setLayout(mainlayout);
}
/*!
 * \brief centerWidget::updateStatus updates the downstream widgets
 * \param _status plane status
 */
void centerWidget::updateStatus(planeStatusData _status)
{
    this->numericaldata01->receivePackage(_status);
    this->graphicaldata01->receivePackage(_status);
    this->charts01->updateValues(_status);
}
/*!
 * \brief centerWidget::setSpeed updates the simulation speed downstream
 * \param speed simulation speed
 */
void centerWidget::setSpeed(int speed)
{
    this->charts01->setSpeed(speed);
}
/*!
 * \brief centerWidget::updateShownTabs updates if the tabs are displayed
 * \param num numerical tab
 * \param graph graphical tab
 * \param live live graphs tab
 * \param map map tab
 */
void centerWidget::updateShownTabs(bool num, bool graph, bool live, bool map)
{
    datacontainer01->clear();
    if(num)     datacontainer01->addTab(numericaldata01,tr("Numerical Data"));
    if(graph)   datacontainer01->addTab(graphicaldata01,tr("Graphical Data"));
    if(live)    datacontainer01->addTab(charts01,tr("Live Graphs"));


}
void centerWidget::setSimulationFreq(int freq)
{
    this->charts01->setFreq(freq);
}
