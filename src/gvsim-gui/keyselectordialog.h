#ifndef KEYSELECTORDIALOG_H
#define KEYSELECTORDIALOG_H

#include <QDialog>
#include <QKeyEvent>
/*!
 * \brief The keySelectorDialog class is a dialog waiting for a keypress
 */
class keySelectorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit keySelectorDialog(QString, QWidget *parent = 0);
private:
    QString name;
signals:
    void selectedKey(int key);///< \brief emits the pressed key information
public slots:

protected:
        virtual void	keyPressEvent ( QKeyEvent * e );
    
};

#endif // KEYSELECTORDIALOG_H
