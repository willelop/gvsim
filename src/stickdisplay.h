#ifndef STICKDISPLAY_H
#define STICKDISPLAY_H

#include <QGLWidget>
/*!
 * \brief The stickDisplay class displays graphically the stick positing with the current configuration
 */
class stickDisplay : public QGLWidget
{
    Q_OBJECT
public:
    explicit stickDisplay(QWidget *parent = 0);
private:
    float axes[2];
public slots:
    void update_axe(int axe, float value);
protected:
    void paintEvent(QPaintEvent *);
    
};

#endif // STICKDISPLAY_H
