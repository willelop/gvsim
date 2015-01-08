#include "turnbank.h"

turnbank::turnbank(QWidget *parent) :
    QGLWidget(parent)
{
    this->setFixedSize(200,200);
}

void turnbank::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.translate(this->width()*(0.5*(1+ball)),this->height()*0.65);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(0,0),10,10);
    painter.resetTransform();
    painter.translate(this->width()*0.5,this->height()*0.85);
    painter.rotate(30*this->stick);
    painter.drawLine(0,0,0,-this->height()*0.8);
}
/*!
 * \brief turnbank::updateValues updates the graphics with new values
 * \param ang angular position of the stick
 * \param force lateral force of the ball
 */
void turnbank::updateValues(float ang, float force)
{
    this->ball = (float)force/MAX_BALL;
    if(this->ball > 1)  this->ball = 1;
    if(this->ball < -1) this->ball = -1;
    this->stick = ang / REF_RATE;
    this->update();
}
