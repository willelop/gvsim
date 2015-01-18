#ifndef EXPORTDATA_H
#define EXPORTDATA_H

#include <QWidget>
#include <QLabel>
#include <QFileDialog>
#include <QPushButton>
#include <QScrollArea>
#include <QComboBox>
#include <QStringList>
#include <QGridLayout>
#include <QCheckBox>
#include <QSettings>
#include <QProgressBar>
#include <QMessageBox>
#include <QDebug>
#include "typedefs.h"

#define MAX_FIELDS 128
/*!
 * \brief The exportData class is a widget to convert the binary data recorded by the simulator into a CSV text file, readable in common software such as matlab
 */
class exportData : public QDialog
{
    Q_OBJECT
public:
    explicit exportData(QWidget *parent = 0);
private:
    QWidget *scrollInner;
    QScrollArea *scroll;
    QGridLayout *scrollLayout;
    QGridLayout *mainLayout;
    QStringList list;
    QComboBox *comboList[MAX_FIELDS];
    QLabel *labelList[MAX_FIELDS];
    QProgressBar *progress;
    QLabel *fileLabel;
    QFileDialog *filedialog01;
    QPushButton *selectButton;
    QPushButton *sendButton;
    QPushButton *saveButton;
    QPushButton *resetButton;
signals:
    void startExport(QString,QString,QStringList);
    void logMessage(QString,LOG::TYPE);
private slots:
    void resetValues();
    void exportClicked();
    void statusUpdated(int prog,int total);
    void setFinished(bool status);
    void saveStructure();
    void restoreStructure();
};

#endif // EXPORTDATA_H
