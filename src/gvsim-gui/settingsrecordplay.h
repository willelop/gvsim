#ifndef SETTINGSRECORDPLAY_H
#define SETTINGSRECORDPLAY_H

#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>
#include <QBuffer>
#include <QFile>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QTimer>
#include <QFile>
#include <QLabel>
#include <QDataStream>
#include <QMessageBox>
#include <QRadioButton>
#include "simengine/planestatusdata.h"
#include "typedefs.h"
#include <QCheckBox>
#include <QSlider>
#define PLAY_SPEEDS 5
/*!
 * \brief The settingsRecordPlay class is a widget to manage the record/play worker
 */
class settingsRecordPlay : public QDialog
{
    Q_OBJECT
public:
    explicit settingsRecordPlay(QWidget *parent = 0);
    QSlider *playSlider;
private:
    QPushButton *play;
    QPushButton *stop;
    QPushButton *record;
    QPushButton *pause;
    QSpinBox *donwsample;
    QRadioButton *playSpeed[PLAY_SPEEDS];
    QGroupBox *playBox;
    QGroupBox *recordBox;
    QVBoxLayout *mainLayout;
    QFileDialog *playFileSelector;
    QFileDialog *recordFileSelector;
    QLineEdit *recordFileName;
    QLineEdit *playFileName;
    QPushButton *recordOpenFile;
    QPushButton *playOpenFile;
    int statusSize;

    QVBoxLayout *recordLayout;
    QHBoxLayout *recordFileLayout;

    QVBoxLayout *playLayout;
    QHBoxLayout *playFileLayout;
protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);


signals:
    void widgetShow();
    void widgetClose();
    void startPlaying();
    void startRecording();
    void pausePlaying();
    void stopPlayRecord();
    void updateSubsampling(int);
    void sendNewStatus(planeStatusData); ///< \brief emits a new plane status
    void statusChanged(int); ///< \brief emits a new state change
    void progressChanged(int,int,int); ///< \brief emits a new progress when playing
    void playFileChanged(QString); ///< \brief emits the play file when changed
    void recordFileChanged(QString); ///< \brief emits the record to file when changed
    void message2log(QString,LOG::TYPE); ///< \brief emits a new message to log
    void changedPlaySpeed(int); ///< \brief emits a new change in simulation speed
public slots:
    void recordPlayStatus(int); ///< \brief updates the record/play state
    void recordPlaySlide(int,int,int); ///< \brief updates the slider
private slots:
    void changeRecordFile(QString);
    void changePlayFile(QString);
    void speedClicked();
    void checkFileAndRecord();
    void checkFileAndPlay();

    //void playLoop();

};

#endif // SETTINGSRECORDPLAY_H
