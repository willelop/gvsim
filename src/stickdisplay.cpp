#include "stickdisplay.h"

stickDisplay::stickDisplay(QWidget *parent) :
    QGLWidget(parent)
{
    //this->update();
}
void stickDisplay::paintEvent(QPaintEvent *)
{
    this->makeCurrent();
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0,0, this->width(),this->height());
    painter.setBackground(QBrush(Qt::black));
    painter.translate(this->width()*0.5, this->height()*0.5);
    painter.translate(this->width()*0.5*axes[0], this->height()*0.5*axes[1]);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(0,0),10,10);
    painter.end();
}
/*!
 * \brief stickDisplay::update_axe update the value of a single axe
 * \param axe axe
 * \param value value of axe normalized (-1.0f, 1.0f)
 */
void stickDisplay::update_axe(int axe, float value)
{
    axes[axe] = value;
    this->update();
}
