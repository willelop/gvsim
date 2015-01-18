#include "graphicaldata.h"

graphicalData::graphicalData(QWidget *parent) :
    QWidget(parent)
{
    liftDrag = new chartLiftDrag();
   // bolabaston = new turnbank();
    scrollInner = new QWidget(this);
    scroll01 = new QScrollArea();

    compass = new chartCompass();
    compass->setFixedSize(200,200);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *inLayout = new QGridLayout();
    inLayout->addWidget(compass,1,0);
    inLayout->addWidget(liftDrag,0,0,1,2);
    //inLayout->addWidget(bolabaston,1,1);

    inLayout->setRowStretch(20,20);
    inLayout->setColumnStretch(20,20);
    scrollInner->setLayout(inLayout);
    scroll01->setWidget(scrollInner);
    scrollInner->setStyleSheet("background: #FFFFFF");
    scroll01->setWidgetResizable(true);
    mainLayout->addWidget(scroll01);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);
}
/*!
 * \brief graphicalData::receivePackage updates the contained widgets
 * \param _plane plane statuss
 */
void graphicalData::receivePackage(planeStatusData _plane)
{
    this->compass->updateCourse(_plane.state.eulerangles[0]);
    this->liftDrag->updateValues(_plane);
   // this->bolabaston->updateValues(_plane.state.ang_velo_b[2],_plane.state.forces_air_sb[1]);
}
