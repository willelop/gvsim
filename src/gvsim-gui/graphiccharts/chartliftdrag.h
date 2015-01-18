#ifndef CHARTLIFTDRAG_H
#define CHARTLIFTDRAG_H

#define MAX_FORCE 50000.0f
#define DRAG_MULTI 5.0f
#include <QDebug>
#include <QGLWidget>
#include <QPixmap>
#include "simengine/planestatusdata.h"

/*!
 * \brief The chartLiftDrag class represents graphically the aerodynamic forces in the stability axes
 */
class chartLiftDrag : public QGLWidget
{
    Q_OBJECT
public:
    explicit chartLiftDrag(QWidget *parent = 0);
private:
    QPixmap plane_side;
    QPixmap plane_back;
    QPixmap plane_top;
    float h_w_ratio;
    float h_w_ratio_back;
    float h_w_ratio_top;
    int widgetheight;
    int widgetwidth;
    int modulewidth;
    float euler[3];
    float sb_forces[3];
    float alpha;
    float beta;
    QPolygon arrow(float dist);
    float limiter(float dist);
    void paintAxe(QPainter *_painter,int size,float rotation,QString title);
    void paintVector(QPainter *_painter,int size,float rotation,QString title,QColor color);

protected:
    void paintEvent(QPaintEvent *);
signals:
    
public slots:
    void updateValues(planeStatusData _status);
    
};

#endif // CHARTLIFTDRAG_H
