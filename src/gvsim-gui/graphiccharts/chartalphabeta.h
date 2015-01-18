#ifndef CHARTALPHABETA_H
#define CHARTALPHABETA_H

#include <QGLWidget>
#include <QWidget>
#include <math.h>
#include <stdlib.h>

/*!
 * \brief The chartalphabeta class is a widget to represent graphically the alpha and beta angles
 */
class chartalphabeta : public QGLWidget
{
    Q_OBJECT
public:
    explicit chartalphabeta(QWidget *parent = 0);
private:
    float alpha;
    float beta;
protected:
    void paintEvent(QPaintEvent *);
signals:
    
public slots:
    void updateValues(float _alpha, float _beta);
    
};

#endif // CHARTALPHABETA_H
