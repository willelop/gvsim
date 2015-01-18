#ifndef VIEW3DWINDOW_H
#define VIEW3DWINDOW_H

#include <QWidget>
#include <QGLWidget>
#include <QVBoxLayout>
#include <QResizeEvent>
#include "view3dwidget.h"
/*!
 * \brief The view3dWindow class is a wrapper to bypass some error in windows with GLWidgets
 */
class view3dWindow : public QWidget
{
    Q_OBJECT
public:
    explicit view3dWindow(QWidget *parent = 0);
    view3dWidget *view3d;
};

#endif // VIEW3DWINDOW_H
