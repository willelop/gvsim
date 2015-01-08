#include "chartsdata.h"

chartsData::chartsData(QWidget *parent) :
    QWidget(parent)
{
    this->frequency = 100;
    this->speed = 1;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *toolLayout = new QHBoxLayout();
    QLabel *timeLabel = new QLabel("Graphics Time (s)");
    timeBox = new QComboBox();
    timeBox->addItem("25");
    timeBox->addItem("50");
    timeBox->addItem("100");
    timeBox->addItem("200");
    timeBox->setCurrentIndex(0);
    toolLayout->addWidget(timeLabel);
    toolLayout->addWidget(timeBox);
    toolLayout->addStretch(100);
    QGridLayout *inLayout = new QGridLayout();
    QWidget *scrollInner = new QWidget(this);
    QScrollArea *scroll01 = new QScrollArea();


    charts[0] = new liveGraph("Alpha",0.35,-0.35,25,0.01);
    inLayout->addWidget(charts[0],0,0);
    charts[1] = new liveGraph("Beta",0.35,-0.35,25,0.01);
    inLayout->addWidget(charts[1],0,1);
    charts[2] = new liveGraph("Pitch",0.3,-0.3,25,0.01);
    inLayout->addWidget(charts[2],1,0);
    charts[3] = new liveGraph("Roll",0.6,-0.6,25,0.01);
    inLayout->addWidget(charts[3],1,1);
    charts[4] = new liveGraph("Vel Down",6,-6,25,0.01);
    inLayout->addWidget(charts[4],2,0);
    charts[5] = new liveGraph("Beta 075",90,0,25,0.01);
    inLayout->addWidget(charts[5],3,0);
    charts[6] = new liveGraph("RPM",6000,0,25,0.01);
    inLayout->addWidget(charts[6],3,1);
    charts[7] = new liveGraph("Elevator",-1,1,25,0.01);
    inLayout->addWidget(charts[7],2,1);


    inLayout->setRowStretch(10,100);
    scrollInner->setLayout(inLayout);
    scroll01->setWidget(scrollInner);
    scrollInner->setStyleSheet("background-color: #222;");
    scroll01->setWidgetResizable(true);
    mainLayout->addLayout(toolLayout);
    mainLayout->addWidget(scroll01);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);
    connect(timeBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(changedBaseTime(QString)));
    this->changedBaseTime("25");
}
/*!
 * \brief chartsData::updateValues adds a new set of values to the graphics
 * \param status new plane statsus data set
 */
void chartsData::updateValues(planeStatusData status)
{
    this->charts[0]->newvalue(status.state.alpha_beta[0]);
    this->charts[1]->newvalue(status.state.alpha_beta[1]);
    this->charts[2]->newvalue(status.state.eulerangles[1]);
    this->charts[3]->newvalue(status.state.eulerangles[2]);
    this->charts[4]->newvalue(status.state.velocity_g[2]);
    this->charts[5]->newvalue(status.engine.beta*57.3);
    this->charts[6]->newvalue(status.engine.engine_rpm);
    this->charts[7]->newvalue(status.commanded.elevator);
}
/*!
 * \brief chartsData::setSpeed modifies the graphcis to match the simulation speed
 * \param speed simulation speed
 */
void chartsData::setSpeed(int _speed)
{
    this->speed = _speed;
    for(int i = 0; i < CHARTS_NUM; i++)
    {
        this->charts[i]->setTimeSample(speed*VIEWING_STEP);
    }
}
void chartsData::setFreq(int _freq)
{
    this->frequency = _freq;
    for(int i = 0; i < CHARTS_NUM; i++)
    {
        this->charts[i]->setTimeSample(speed*VIEWING_STEP);
    }
}

/*!
 * \brief chartsData::changedBaseTime changes the backscale time
 * \param entry the backscale time
 */
void chartsData::changedBaseTime(QString entry)
{
    bool ok;
    float timebase = entry.toFloat(&ok);
    if(ok){
        qDebug() << timebase;
        for(int i = 0; i < CHARTS_NUM; i++)
        {
            this->charts[i]->setBaseTime(timebase);
        }

    }
}



