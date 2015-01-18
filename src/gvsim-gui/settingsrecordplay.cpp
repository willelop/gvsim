#include "settingsrecordplay.h"

settingsRecordPlay::settingsRecordPlay(QWidget *parent) :
    QDialog(parent)
{

    playSlider = new QSlider(Qt::Horizontal);
    play = new QPushButton(QIcon(QPixmap(":/icons/play.png")),tr("Play"),this);
    pause = new QPushButton(QIcon(QPixmap(":/icons/pause.png")),tr("Pause"),this);
    stop = new QPushButton(QIcon(QPixmap(":/icons/stop.png")),tr("Stop"),this);
    record = new QPushButton(QIcon(QPixmap(":/icons/record.png")),tr("Record"),this);
    pause->setEnabled(false);
    stop->setEnabled(false);
    playSlider->setFocusPolicy(Qt::NoFocus);
    donwsample = new QSpinBox();
    donwsample->setMinimum(1);
    donwsample->setMaximum(1000);
    donwsample->setValue(1);

    QGroupBox *buttonbox = new QGroupBox(tr("Action"));
    QHBoxLayout *buttons = new QHBoxLayout();
    QVBoxLayout *buttonlayout = new QVBoxLayout();
    buttons->addWidget(play);
    buttons->addWidget(pause);
    buttons->addWidget(stop);
    buttons->addWidget(record);
    buttonlayout->addLayout(buttons);
    buttonlayout->addWidget(playSlider);
    buttonbox->setLayout(buttonlayout);

    this->setWindowTitle(tr("Record and play options"));
    mainLayout = new QVBoxLayout();
    planeStatusData guille;
    statusSize = sizeof(guille);
    //RECORD
    recordBox = new QGroupBox(tr("Record Data"));
    recordFileName = new QLineEdit();
    recordOpenFile = new QPushButton(tr("Select"));
    recordFileSelector = new QFileDialog();
#ifdef USING_QT5
    recordFileSelector->setNameFilter("GVSim files (*.gvsim);;");
#else
    recordFileSelector->setFilter("GVSim files (*.gvsim);;");
#endif
    recordFileSelector->setAcceptMode(QFileDialog::AcceptSave);
    recordFileSelector->setDefaultSuffix("gvsim");
    recordLayout = new QVBoxLayout();
    recordFileLayout = new QHBoxLayout();
    recordFileLayout->addWidget(recordFileName);
    recordFileLayout->addWidget(recordOpenFile);
    recordLayout->addLayout(recordFileLayout);
    QHBoxLayout *sampleLayout = new QHBoxLayout();
    sampleLayout->addWidget(this->donwsample);
    sampleLayout->addWidget(new QLabel(tr("Subsampling Numer")));
    sampleLayout->addStretch(100);
    recordLayout->addLayout(sampleLayout);
    recordBox->setLayout(recordLayout);

    //PLAY
    playBox = new QGroupBox(tr("Play Data"));
    playFileName = new QLineEdit();
    playOpenFile = new QPushButton(tr("Select"));
    playFileSelector = new QFileDialog();
#ifdef USING_QT5
    playFileSelector->setNameFilter("GVSim files (*.gvsim);;");
#else
    playFileSelector->setFilter("GVSim files (*.gvsim);;");
#endif
    playLayout = new QVBoxLayout();
    playFileLayout = new QHBoxLayout();
    QHBoxLayout *speedLayout = new QHBoxLayout();
    QStringList speeds;
    speeds << "1x" << "2x" << "4x" << "8x" << "16x";
    for(int i = 0; i < PLAY_SPEEDS; i++)
    {
        playSpeed[i] = new QRadioButton(speeds.at(i));
        speedLayout->addWidget(playSpeed[i]);
        connect(playSpeed[i],SIGNAL(clicked()),this,SLOT(speedClicked()));
    }
    playSpeed[0]->setChecked(true);
    playLayout->addLayout(speedLayout);
    playFileLayout->addWidget(playFileName);
    playFileLayout->addWidget(playOpenFile);
    playLayout->addLayout(playFileLayout);
    playBox->setLayout(playLayout);

    mainLayout->addWidget(buttonbox);
    mainLayout->addWidget(recordBox);
    mainLayout->addWidget(playBox);
    mainLayout->addStretch(3);

    connect(recordOpenFile,SIGNAL(clicked()),recordFileSelector,SLOT(show()));
    connect(playOpenFile,SIGNAL(clicked()),playFileSelector,SLOT(show()));
    connect(recordFileSelector,SIGNAL(fileSelected(QString)),this,SLOT(changeRecordFile(QString)));
    connect(playFileSelector,SIGNAL(fileSelected(QString)),this,SLOT(changePlayFile(QString)));
    connect(stop,SIGNAL(clicked()),this,SIGNAL(stopPlayRecord()));
    connect(pause,SIGNAL(clicked()),this,SIGNAL(pausePlaying()));
    connect(play,SIGNAL(clicked()),this,SLOT(checkFileAndPlay()));
    connect(record,SIGNAL(clicked()),this,SLOT(checkFileAndRecord()));

    //connect(playTimer,SIGNAL(timeout()),this,SLOT());
    emit message2log("Size of plane data is " +QString::number(sizeof(planeStatusData))+ " bytes",LOG::NORMAL);

    this->setLayout(mainLayout);
    QSize size = this->sizeHint();
    this->setFixedSize(400,size.height());
    connect(donwsample,SIGNAL(valueChanged(int)),this,SIGNAL(updateSubsampling(int)));

}
void settingsRecordPlay::changeRecordFile(QString _file)
{
    this->recordFileName->setText(_file);
    emit recordFileChanged(_file);
}
void settingsRecordPlay::changePlayFile(QString _file)
{
    this->playFileName->setText(_file);
    emit playFileChanged(_file);
}

void settingsRecordPlay::recordPlayStatus(int _state)
{
    switch(_state)
    {
    case 0: //none
        this->playBox->setEnabled(true);
        this->playFileName->setEnabled(true);
        this->playOpenFile->setEnabled(true);
        this->recordBox->setEnabled(true);
        this->recordFileName->setEnabled(true);
        this->recordOpenFile->setEnabled(true);
        break;
    case 1: //playing
        this->recordBox->setEnabled(false);
        this->playFileName->setEnabled(false);
        this->playOpenFile->setEnabled(false);
        break;
    case 2: //playing paused
        this->recordBox->setEnabled(false);
        this->playFileName->setEnabled(false);
        this->playOpenFile->setEnabled(false);
        break;
    case 3: //Recording
        this->playBox->setEnabled(true);
        this->playFileName->setEnabled(true);
        this->playOpenFile->setEnabled(true);
        this->recordBox->setEnabled(false);
        this->recordFileName->setEnabled(false);
        this->recordOpenFile->setEnabled(false);
        break;

    }

    switch(_state)
    {
    case 0: //none
        this->pause->setEnabled(false);
        this->stop->setEnabled(false);
        this->play->setEnabled(true);
        this->record->setEnabled(true);
        this->playSlider->setEnabled(false);
        break;
    case 1: //playing
        this->pause->setEnabled(true);
        this->stop->setEnabled(true);
        this->play->setEnabled(false);
        this->record->setEnabled(false);
        this->playSlider->setEnabled(true);
        break;
    case 2: //playing paused
        this->pause->setEnabled(false);
        this->play->setEnabled(true);
        this->stop->setEnabled(true);
        this->record->setEnabled(false);
        this->playSlider->setEnabled(true);
        break;
    case 3: //Recording
        this->pause->setEnabled(false);
        this->play->setEnabled(false);
        this->stop->setEnabled(true);
        this->record->setEnabled(false);
        this->playSlider->setEnabled(false);
        break;

    }
}
void settingsRecordPlay::recordPlaySlide(int _curr,int _min, int _max)
{
    this->playSlider->setMinimum(_min);
    this->playSlider->setMaximum(_max);
    this->playSlider->setValue(_curr);
}
void settingsRecordPlay::speedClicked()
{
    for(int i = 0; i < PLAY_SPEEDS; i++)
    {
        if(playSpeed[i]->isChecked()){
            if(playSpeed[i]->text()=="1x") emit changedPlaySpeed(1);
            else if(playSpeed[i]->text()=="2x") emit changedPlaySpeed(2);
            else if(playSpeed[i]->text()=="4x") emit changedPlaySpeed(4);
            else if(playSpeed[i]->text()=="8x") emit changedPlaySpeed(8);
            else if(playSpeed[i]->text()=="16x") emit changedPlaySpeed(16);
        }
    }
}
void settingsRecordPlay::closeEvent(QCloseEvent *)
{
    emit widgetClose();
}
void settingsRecordPlay::showEvent(QShowEvent *)
{
    emit widgetShow();
}
void settingsRecordPlay::checkFileAndPlay()
{
    if(this->playFileName->text().isEmpty()){
        this->playFileName->setText(playFileSelector->getOpenFileName());
        emit playFileChanged(this->playFileName->text());
    }
    emit startPlaying();
}
void settingsRecordPlay::checkFileAndRecord()
{
    if(this->recordFileName->text().isEmpty()){
        this->recordFileName->setText(recordFileSelector->getOpenFileName());
        emit recordFileChanged(this->recordFileName->text());
    }
    emit startRecording();
}



