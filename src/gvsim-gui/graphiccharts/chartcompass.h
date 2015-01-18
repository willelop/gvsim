#ifndef CHARTCOMPASS_H
#define CHARTCOMPASS_H

#include <QGLWidget>
#include <QDebug>

/*!
 * \brief The chartCompass class represents graphically the course of the plane
 */
class chartCompass : public QGLWidget
{
    Q_OBJECT
public:
    explicit chartCompass(QWidget *parent = 0);
    
private:
    float course;
    float course_deg;
protected:
    void paintEvent(QPaintEvent *);
signals:
    
public slots:
    void updateCourse(float course);
    
};

#endif // CHARTCOMPASS_H
