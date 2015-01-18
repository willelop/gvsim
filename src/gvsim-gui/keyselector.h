#ifndef KEYSELECTOR_H
#define KEYSELECTOR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>
#include "keyselectordialog.h"
#include "typedefs.h"
/*!
 * \brief The keySelector class allows the user to select a key and passes it to another object
 */
class keySelector : public QWidget
{
    Q_OBJECT
public:
    explicit keySelector(QString,inputs::CONTROLKEYS, QWidget *parent = 0);
    const int &key; ///< \brief current key
    const inputs::CONTROLKEYS &control; ///< \brief current axis
private:
    int internalkey;
    inputs::CONTROLKEYS internalcontrol;
    QPushButton *setButton;
    QLabel *name;
    QLabel *current;
    QString keyname;
    QHBoxLayout *container;
    keySelectorDialog *keyDialog;
signals:
    void keyChanged(inputs::CONTROLKEYS, int); ///< \brief Emit current key when the key has been changed */
private slots:
    void keySelected(int); ///< \brief Receives key information from keySelectorDialog */
public slots:
    void updateKey(inputs::CONTROLKEYS _key,int index); ///< \brief Set the key to a value */
    
};

#endif // KEYSELECTOR_H
