#ifndef WINDTOOLS_H
#define WINDTOOLS_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QGroupBox>
/*!
 * \brief The windTools class is a widget allowing to change the athmospheric status of the simulation in real time
 */
class windTools : public QWidget
{
    Q_OBJECT
public:
    explicit windTools(QWidget *parent = 0);
private:
    QVBoxLayout *mainLayout;
    QGroupBox *windGroup;
    QGroupBox *isaGroup;
    QLabel *isaTempLabel;
    QSpinBox *isaTemp;
    QPushButton *sendButton;
private slots:
    void isaChanged(int);
    void sendPressed();
signals:
    void changedIsaValue(int);///< \brief emits new isa temp condition with temperature increment
};

#endif // WINDTOOLS_H
