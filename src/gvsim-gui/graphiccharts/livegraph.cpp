#include "livegraph.h"

liveGraph::liveGraph(QString _name, float _max, float _min, float _timesize,float _sample_time, QWidget *parent) :
    QGLWidget(parent)
{

    this->counter = 0;
    this->counter2 = 0;
    this->name = _name;
    this->max = _max;
    this->min = _min;
    this->timesize = _timesize;
    this->sample_time = _sample_time;
    this->setFixedHeight(140);
    margintext = 20;
    margin = 10;
    this->skip = 0;
    this->skip2 = 0;
    this->setTimeSample(_sample_time);
}
void liveGraph::paintEvent(QPaintEvent *)
{
    float inheight = this->height()-2*margin;
    float inwidht = this->width()-2*margin -margintext;
    QPainter painter(this);
    QFont bold("Arial",9,3,true);
    QFont normal("Arial",10);
    painter.setPen(QColor("#222"));
    painter.setBrush(QColor("#222"));
    painter.drawRect(0,0,this->width(),this->height());
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::green);
    painter.translate(0,this->height());
    painter.rotate(270);
    QRect textcont1(QPoint(0,0),QPoint(this->height(),margintext));
    painter.setFont(bold);
    painter.drawText(textcont1,name,QTextOption(Qt::AlignCenter));
    painter.setFont(normal);
    painter.setBackground(Qt::black);
    painter.resetTransform();
    painter.translate(margin+margintext,margin);
    painter.setPen(Qt::gray);
    float dec = inwidht*0.1;

    for(int i = 0;i < 11; i++)
    {
        float dist = dec*i;
        painter.drawLine(QPoint(dist,0),QPoint(dist,inheight));
        if(i%2 == 0) {
            QRect textcont(QPoint(dist-35,2),QPoint(dist-5,15));
            painter.drawText(textcont,QString::number((float)((-timesize) + (float)(timesize*0.1*i)))+" s",QTextOption(Qt::AlignRight));
        }
    }
    for(int i = 0;i < 7; i++)
    {
        float quint = inheight*0.16666666;
        float incval = (max-min)*0.16666666;
        painter.drawLine(QPoint(0,quint*i),QPoint(inwidht,quint*i));
        QRect textcont;
        QFont min("Arial",8);
        painter.setFont(min);
        if(i == 0)
        {
            textcont= QRect (QPoint(5,quint*i+1),QPoint(35,quint*i+11));
        }else{
            textcont= QRect (QPoint(5,quint*i-12),QPoint(35,quint*i-2));

        }
        QString num  = QString("%1").arg(max - i * incval,1,'F',2);
        painter.drawText(textcont,num,QTextOption(Qt::AlignRight));
    }
    painter.resetTransform();
    painter.translate(margin+margintext,margin);
    painter.translate(0,inheight*0.5);
    painter.setPen(Qt::green);
    painter.scale(inwidht,0.5*inheight);
    painter.drawPolyline(points);


}
void liveGraph::newvalue(float _val)
{
    counter++;
    bool get = false;
    if(skip <= 0)
    {
        get = true;
    }else{
        if(counter%skip == 0) {
            get = true;
        }
    }
    if(get){
        float cent = 1.0f/N_POINTS;
        if(points.size() == N_POINTS) points.pop_front();
        points.translate(-cent,0);
        points.append(QPointF(1.0f,-(_val-0.5*(max+min))/(0.5*(max-min))));
        this->update();
    }
}
void liveGraph::setBaseTime(float secs)
{

    this->timesize = secs;
    this->clear();
    this->setTimeSample(this->sample_time);

}
void liveGraph::setTimeSample(float secs_ps)
{
    this->sample_time = secs_ps;
    float skiptime = timesize/N_POINTS;
    skip = skiptime / this->sample_time + 1;
    this->clear();
}
void liveGraph::clear()
{
    this->points.clear();
}

