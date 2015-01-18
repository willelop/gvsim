#include "chartliftdrag.h"

chartLiftDrag::chartLiftDrag(QWidget *parent) :
    QGLWidget(parent)
{
    QPixmap alphachannel[3];
    plane_side = QPixmap(":/images/predator_side.png");
    alphachannel[0] = QPixmap(plane_side.size());
    alphachannel[0].fill(QColor(128,128,128));

    plane_back = QPixmap(":/images/predator_back.png");
    alphachannel[1] = QPixmap(plane_back.size());
    alphachannel[1].fill(QColor(128,128,128));
    plane_top = QPixmap(":/images/predator_top.png");
    alphachannel[2] = QPixmap(plane_top.size());
    alphachannel[2].fill(QColor(128,128,128));
#ifndef USING_QT5
    plane_side.setAlphaChannel(alphachannel[0]);
    plane_back.setAlphaChannel(alphachannel[1]);
    plane_top.setAlphaChannel(alphachannel[2]);
#endif
    h_w_ratio = (float)plane_side.height() / (float)plane_side.width();
    h_w_ratio_back = (float)plane_back.height() / (float)plane_back.width();
    h_w_ratio_top = (float)plane_top.height() / (float)plane_top.width();
    this->widgetheight = 180;
    this->widgetwidth = 4*this->widgetheight;
    this->modulewidth = this->widgetwidth/3;
    this->setMinimumSize(widgetwidth,widgetheight);
    this->alpha = 0;
    this->beta = 0;
    this->euler[0] = 0;
    this->euler[1] = 0;
    this->euler[2] = 0;
    this->sb_forces[0] = 0;
    this->sb_forces[1] = 0;
    this->sb_forces[2] = 0;

}
/*!
 * \brief chartLiftDrag::paintEvent main paint funcion
 */
void chartLiftDrag::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    float width = this->modulewidth*0.6;
    float height = width*h_w_ratio;
    QRect rectleft(0,0,modulewidth,widgetheight);
    QRect rectcenter(modulewidth,0,modulewidth,widgetheight);
    QRect rectright(2*modulewidth,0,modulewidth,widgetheight);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawRect(QRect(0,0,this->width(),this->height()));
    // ALPHA
    painter.setClipRect(rectleft);
    painter.setClipping(true);
    painter.translate(this->widgetheight*0.5,this->widgetheight*0.5);
    this->paintAxe(&painter,this->widgetheight*0.4,0,"x");
    this->paintAxe(&painter,this->widgetheight*0.4,-90,"z");
    painter.rotate(-alpha);
    painter.drawPixmap(-0.5*width,-0.5*height,width,height,plane_side);
    painter.rotate(alpha);
    this->paintVector(&painter,-limiter(-this->widgetheight*sb_forces[0]*DRAG_MULTI/MAX_FORCE),0,QString::number((int)-sb_forces[0]),Qt::darkRed);
    this->paintVector(&painter,limiter(-this->widgetheight*sb_forces[2]/MAX_FORCE),90,QString::number((int)-sb_forces[2]),Qt::darkGreen);
    painter.setClipping(false);

    //ROLL
    painter.resetTransform();
    painter.setClipRect(rectcenter);
    painter.setClipping(true);
    painter.translate(this->modulewidth,0);
    painter.translate(this->modulewidth*0.5,this->widgetheight*0.5);
    painter.rotate(this->euler[2]);
    this->paintAxe(&painter,this->widgetheight*0.4,0,"y");
    this->paintAxe(&painter,this->widgetheight*0.4,-90,"z");
    width = this->modulewidth*0.8;
    height = width*h_w_ratio_back;
    painter.drawPixmap(-0.5*width,-0.5*height,width,height,plane_back);
    this->paintVector(&painter,limiter(this->widgetheight*sb_forces[1]*DRAG_MULTI/MAX_FORCE),0,QString::number((int)sb_forces[1]),Qt::darkBlue);
    this->paintVector(&painter,limiter(-this->widgetheight*sb_forces[2]/MAX_FORCE),90,QString::number((int)-sb_forces[2]),Qt::darkGreen);
    painter.setClipping(false);

    // SIDESLIP
    painter.resetTransform();
    painter.setClipRect(rectright);
    painter.setClipping(true);
    painter.translate(2*this->modulewidth,0);
    painter.translate(this->modulewidth*0.5,this->widgetheight*0.5);
    painter.rotate(-this->beta);
    this->paintAxe(&painter,this->widgetheight*0.4,0,"y");
    this->paintAxe(&painter,this->widgetheight*0.4,90,"x");
    width = this->modulewidth*0.8;
    height = width*h_w_ratio_top;
    painter.drawPixmap(-0.5*width,-0.5*height,width,height,plane_top);
    this->paintVector(&painter,limiter(this->widgetheight*sb_forces[1]*DRAG_MULTI/MAX_FORCE),0,QString::number((int)sb_forces[1]),Qt::darkBlue);
    this->paintVector(&painter,-limiter(-this->widgetheight*sb_forces[0]*DRAG_MULTI/MAX_FORCE),90,QString::number((int)-sb_forces[0]),Qt::darkRed);
    painter.setClipping(false);

    // TITLE
    QRect titlerect(this->width()*0.1,0,this->width()*0.8,this->height()*0.15);
    QFont titlefont("Courier",titlerect.height()*0.8);
    painter.resetTransform();
    painter.setPen(Qt::darkBlue);
    painter.setFont(titlefont);
    painter.drawText(titlerect,Qt::AlignCenter,"Stability Axes");
    painter.end();
}
/*!
 * \brief chartLiftDrag::updateValues updates the values to show
 * \param _status plane Status Data
 */
void chartLiftDrag::updateValues(planeStatusData _status)
{
    // FIX ISUE WITH HIGH ANGLES
    this->alpha = _status.state.alpha_beta[0]*RAD2DEG_;
    this->beta = _status.state.alpha_beta[1]*RAD2DEG_;
    for(int i= 0; i< 3; i++)
    {
        this->euler[i] = _status.state.eulerangles[i]*RAD2DEG_;
        this->sb_forces[i] = _status.state.forces_air_sb[i];
    }
    this->update();

}
QPolygon chartLiftDrag::arrow(float dist)
{
    float multi = 0.0f;
    if(dist > 0) multi = 1.0f;
    else if(dist < 0) multi = -1.0f;
    else return QPolygon();
    QPolygon _arrow;
    _arrow << QPoint(dist,0) << QPoint(dist-multi*10,-5) << QPoint(dist-multi*10,5);
    return _arrow;
}
float chartLiftDrag::limiter(float dist)
{
    float outdist = dist;
    if(dist > this->widgetheight*0.5) outdist = this->widgetheight*0.5;
    if(dist < -this->widgetheight*0.5) outdist = -this->widgetheight*0.5;
    return outdist;
}
void chartLiftDrag::paintAxe(QPainter *_painter, int size, float rotation, QString title)
{
    QPolygon arrow;
    QRect titlerect(8,-8,16,16);
    QFont axetitle("Courier",12,1);
    arrow << QPoint(size,0) << QPoint(size-3,3) << QPoint(size-3,-3);
    _painter->rotate(-rotation);
    _painter->setFont(axetitle);
    _painter->setPen(QPen(Qt::darkGray,2));
    _painter->setBrush(Qt::darkGray);
    _painter->drawLine(0,0,size,0);
    _painter->drawPolygon(arrow);
    _painter->setPen(QPen(Qt::darkGray,1));
    _painter->translate(size,0);
    _painter->rotate(rotation);
    _painter->drawText(titlerect,title);
    _painter->rotate(-rotation);
    _painter->translate(-size,0);
    _painter->rotate(rotation);
}
void chartLiftDrag::paintVector(QPainter *_painter, int size, float rotation, QString title, QColor color)
{
    float multi = 0.0f;
    if(size > 3) multi = 1.0f;
    else if(size < -3) multi = -1.0f;
    QPolygon arrow;
    QRect titlerect(8,0,120,16);
    QFont axetitle("Courier",10,1);
    arrow << QPoint(size,0) << QPoint(size-multi*3,3) << QPoint(size-multi*3,-3);
    if(multi == 0.0f) arrow.clear();

    _painter->rotate(-rotation);
    _painter->setFont(axetitle);
    _painter->setPen(QPen(color,2));
    _painter->setBrush(color);
    _painter->drawLine(0,0,size,0);
    _painter->drawPolygon(arrow);
    _painter->setPen(QPen(color,1));
    _painter->translate(size,0);
    _painter->rotate(rotation);
    _painter->drawText(titlerect,title);
    _painter->rotate(-rotation);
    _painter->translate(-size,0);
    _painter->rotate(rotation);
}
