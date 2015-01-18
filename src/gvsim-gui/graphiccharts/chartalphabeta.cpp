#include "chartalphabeta.h"

chartalphabeta::chartalphabeta(QWidget *parent) :
    QGLWidget(parent)
{
    this->alpha = 0;
    this->beta = 0;
}
/*!
 * \brief chartalphabeta::paintEvent updates the graphics
 */
void chartalphabeta::paintEvent(QPaintEvent *)
{
    int distv;
    distv = std::min(this->height()*0.5f,this->width()*0.25f);
    distv = this->height()*0.5f;
    int disth = this->width()*0.25f;
    float shortfactor = 0.8;
    QColor gris(230,230,230);
    QColor background(gris);
    QColor backgroundlines(Qt::black);
    QPen foregroundlines(Qt::darkBlue);
    QPen text(Qt::black);
    QFont title("Courier", 15,QFont::Bold);
    foregroundlines.setWidth(3);

    QPainter painter(this);

    painter.setPen(background);
    painter.setBrush(background);
    painter.drawRect(0,0,4*disth,2*distv);
    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::black);
    painter.setClipping(true);
    QRect zone1(0,0,2*disth,2*distv);
    QRect zone2(2*disth,0,2*disth,2*distv);
    QPoint center1(0,distv);
    QPoint center2(3*disth,2*distv);
    QPoint extrema1(shortfactor*2*disth*cos(-alpha),(distv-shortfactor*disth*sin(alpha)));
    QPoint extrema2(3*disth-shortfactor*2*distv*sin(beta),(1- shortfactor)*2*distv + (1.0f-cos(beta))*2*distv*shortfactor  );
    painter.setPen(backgroundlines);
    painter.setClipRect(zone1);
    painter.drawEllipse(center1,2*disth-2,2*disth-2);
    painter.drawLine(0,distv,2*disth*0.5f*sqrt(3),0);
    painter.drawLine(0,distv,2*disth*0.5f*sqrt(3),2*distv);
    painter.setClipRect(zone2);
    painter.drawEllipse(center2,2*distv-2,2*distv-2);
    painter.drawLine(3*disth,2*distv,2*disth,(1-0.5f*sqrt(3))*2*distv);
    painter.drawLine(3*disth,2*distv,4*disth,(1-0.5f*sqrt(3))*2*distv);
    painter.setClipping(false);
    painter.drawLine(0,distv,4*disth,distv);


    painter.setPen(foregroundlines);
    painter.drawLine(center1,extrema1);
    painter.drawLine(center2,extrema2);

    painter.setPen(text);
    QRect rect1(QPoint(center1.x()+1,center1.y()-8),QSize(40,16));
    QRect rect2(QPoint(center2.x()-20,center2.y()-16-1),QSize(40,16));
    painter.setBrush(Qt::white);
    painter.drawRect(rect1);
    painter.drawRect(rect2);
    painter.drawText(rect1,Qt::AlignCenter,QString("%1").arg(alpha*57.3,4,'F',2));
    painter.drawText(rect2,Qt::AlignCenter,QString("%1").arg(beta*57.3,4,'F',2));
    painter.setPen(foregroundlines);
    painter.setFont(title);
    QRect title1(0.6*disth-40,1.5f*distv-10,80,20);
    QRect title2((3-0.6)*disth-40,1.5f*distv-10,80,20);
    painter.drawText(title1,tr("Alpha"));
    painter.drawText(title2,tr("Beta"));
    painter.end();
}
/*!
 * \brief The chartalphabeta::updateValues updates the printed values
 * \param _alpha alpha angle in radians
 * \param _beta beta angle in radians
 */
void chartalphabeta::updateValues(float _alpha, float _beta)
{
    this->alpha = _alpha;
    this->beta = _beta;
    this->update();
}
