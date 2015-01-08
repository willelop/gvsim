#ifndef TURNBANK_H
#define TURNBANK_H

#include <QGLWidget>
#include <QDebug>
#define MAX_BALL 500.0f
#define REF_RATE 0.052359833
/*!
 * \brief The turnbank class displays graphically the turn and bank state of the plane
 */
class turnbank : public QGLWidget
{
    Q_OBJECT
public:
    explicit turnbank(QWidget *parent = 0);
private:
    float ball;
    float stick;
protected:
    void paintEvent(QPaintEvent *);
public slots:
    void updateValues(float ang, float force);

};

#endif // TURNBANK_H
