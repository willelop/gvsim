#ifndef FUELTOOLS_H
#define FUELTOOLS_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
/*!
 * \brief The fuelTools class is a widget to modify in real time the fuel status of the plane
 */
class fuelTools : public QWidget
{
    Q_OBJECT
public:
    explicit fuelTools(QWidget *parent = 0);
private:
    QPushButton *set;
    QSlider *slide;
    QLabel *number;
signals:
    void newFuelCuantity(float _fuel_ratio); ///<  \brief emits a new fuel ratio (0.0f - 1.0f)
private slots:
    void updateNumber();
    void setPressed();

};

#endif // FUELTOOLS_H
