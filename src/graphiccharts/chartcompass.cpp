#include "chartcompass.h"

chartCompass::chartCompass(QWidget *parent) :
    QGLWidget(parent)
{
    this->course = 0;
    this->course_deg = 0;
    this->setMinimumSize(150,150);
    this->update();
}
/*!
 * \brief chartCompass::paintEvent main paint function
 */
void chartCompass::paintEvent(QPaintEvent *)
{
    float hheight = this->height()*0.5;
    float hwidth = this->width()*0.5;
    float margin = 0.8;
    QPainter painter(this);
    QFont title("Courier", 15,QFont::Bold);
    QFont cardinals("Courier", 12,QFont::Bold);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawRect(0,0,this->width(),this->height());
    painter.setPen(Qt::black);
    painter.drawEllipse(QPoint(hwidth,hheight),(int)(hwidth*margin),(int)(hheight*margin));
    painter.resetTransform();
    painter.translate(hwidth,hheight);
    painter.rotate(-course_deg);

    for(int i = 0; i < 12; i++)
    {
        float multi;
        if(i%3 == 0) {
            multi = 1.0f;
            painter.setPen(QPen(Qt::black,2));
        }else{
            multi = 0.5f;
            painter.setPen(QPen(Qt::black,1));
        }
        painter.drawLine(0,-hheight*margin,0,-hheight*(margin-(1-margin)*multi));
        if(multi == 1.0f)
        {
            QRect card(-6,-hheight*margin+8,15,15);
            QString chain;
            switch(i)
            {
            case 0:
                chain = "N";
                break;
            case 3:
                chain = "E";
                break;
            case 6:
                chain = "S";
                break;
            case 9:
                chain = "W";
                break;
            }
            painter.setPen(Qt::transparent);
            painter.drawRect(card);
            painter.setPen(Qt::black);
            painter.drawText(card,Qt::AlignCenter,chain);
        }
        painter.rotate(360/12);
    }
    painter.resetTransform();
    painter.setPen(QPen(Qt::darkBlue,2));
    painter.setBrush(Qt::blue);
    painter.translate(hwidth,hheight);
    //painter.rotate(course_deg);
    painter.drawLine(0,0,0,-hheight*margin*0.6);
    QPolygon arrow;
    arrow << QPoint(-5,-hheight*margin*0.6+25) << QPoint (+5,-hheight*margin*0.6+25) << QPoint(0,-hheight*margin*0.6);
    painter.drawPolygon(arrow);

    painter.resetTransform();
    painter.translate(hwidth,hheight);
    QRect rect(-20,-8,40,16);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);
    painter.drawRect(rect);
    painter.drawText(rect,Qt::AlignCenter,QString::number((int)(this->course_deg)));
    QRect rectitle(-hwidth,hheight*0.35-hheight*0.2,2*hwidth,hheight*0.4);
    painter.setFont(title);
    painter.drawText(rectitle,Qt::AlignCenter,QString("Course"));
    painter.end();

}
/*!
 * \brief chartCompass::updateCourse updates the displayed course
 * \param _course course angle in radians
 */
void chartCompass::updateCourse(float _course)
{
    this->course = _course;
    this->course_deg = _course*RAD2DEG_;
    if(this->course_deg < 0) course_deg += 360.0f;
    this->update();
}

