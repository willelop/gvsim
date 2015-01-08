#include "exportdata.h"

exportData::exportData(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Export Data to CSV"));
    filedialog01 = new QFileDialog();
    progress = new QProgressBar();
    scroll = new QScrollArea();
    scrollInner = new QWidget(scroll);
    scrollLayout = new QGridLayout(scrollInner);
    scroll->setWidget(scrollInner);
    scrollInner->setLayout(scrollLayout);
    scrollInner->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    fileLabel = new QLabel("");
    selectButton = new QPushButton(tr("Select File"));
    resetButton = new QPushButton(tr("Reset Fields"));
    this->setWindowFlags(Qt::Dialog);
    fileLabel->setStyleSheet("border: 1px solid #BBB;border-radius: 3px");
#ifdef USING_QT5
    filedialog01->setNameFilter("GVSim files (*.gvsim);;");
#else
    filedialog01->setFilter("GVSim files (*.gvsim);;");
#endif

    list.clear();
    list << "None";
    list << "Simulation Time";
    list << "Euler0" << "Euler1" << "Euler2";
    list << "Latitude" << "Longitude" << "Altitude";
    list << "Ground Altitude";
    list << "Velocity U" << "Velocity V" << "Velocity W";
    list << "Velocity N" << "Velocity E" << "Velocity D";
    list << "Ang Vel p" << "Ang Vel q" << "Ang Vel r";
    list << "Ang Accel p_dot" << "Ang Accel q_dot" << "Ang Accel r_dot";
    list << "Alpha" << "Beta";
    list << "Alpha_Dot" << "Beta_Dot";
    list << "Force Xb" << "Force Yb" << "Force Zb";
    list << "Engine Force Xb" << "Engine Force Yb" << "Engine Force Zb";
    list << "Gear Force Xb" << "Gear Force Yb" << "Gear Force Zb";
    list << "Moment L" << "Moment M" << "Moment N";
    list << "Engine Moment L" << "Engine Moment M" << "Engine Moment N";
    list << "Gear Moment L" << "Gear Moment M" << "Gear Moment N";
    list << "Mass";
    list << "AirSpeed";
    list << "Shaft Power";
    list << "Thrust";
    list << "Engine RPM" << "Propeller RPM";
    list << "Beta 0.75" << "Fuel Consumption";
    list << "Mass Center X" << "Mass Center Y" << "Mass Center Z";
    list << "Inertia Matrix 11"  << "Inertia Matrix 12" << "Inertia Matrix 13";
    list << "Inertia Matrix 21"  << "Inertia Matrix 22" << "Inertia Matrix 23";
    list << "Inertia Matrix 31"  << "Inertia Matrix 32" << "Inertia Matrix 33";
    list << "CL" << "CLalphadot" << "CLde" << "CLq";
    list << "CD" << "CDalphadot" << "CDde" << "CDq";
    list << "Cybeta" << "Cybetadot" << "Cydr" << "Cyda" << "Cyp" << "Cyr";
    list << "Cm0" << "Cmalpha" << "Cmalphadot" << "Cmde" << "Cmq";
    list << "Clbeta" << "Clbetadot" << "Cldr" << "Clda" << "Clp" << "Clr";
    list << "Cnbeta" << "Cnbetadot" << "Cndr" << "Cnda" << "Cnp" << "Cnr";
    list << "Com. Elevator" << "Com. Ailerons" <<  "Com. Rudder" << "Com. Throttle" << "Com. Propeller";
    list << "Trim Elevator" << "Trim Ailerons" <<  "Trim Rudder";
    list << "Gear Down" << "Gear Brakes" << "Flaps";
    list << "TakeOff Mode" << "Missile 1" << "Missile 2";
    list << "Atm. Temp" << "Atm. Pres" << "Atm. Dens" << "Sound Speed";
    list << "Wind Speed N" << "Wind Speed E" << "Wind Speed D";
    list << "Air Force Xsb" << "Air Force Ysb" << "Air Force Zsb";
    list << "Air Moment Xsb" << "Air Moment Ysb" << "Air Moment Zsb";

    mainLayout = new QGridLayout();
    mainLayout->addWidget(fileLabel,0,0,1,2);
    mainLayout->addWidget(selectButton,0,2);
    mainLayout->addWidget(progress,1,0,1,3);
    for(int i = 0; i< MAX_FIELDS; i++)
    {
        comboList[i] = new QComboBox();
        comboList[i]->addItems(list);
        labelList[i] = new QLabel(tr("Field ")+QString::number(i));
        scrollLayout->addWidget(labelList[i],i,0);
        scrollLayout->addWidget(comboList[i],i,1);
        comboList[i]->setAccessibleDescription(QString::number(i));
    }
    scrollLayout->setRowStretch(MAX_FIELDS,100);
    mainLayout->addWidget(scroll,2,0,1,3);
    scroll->setWidgetResizable(true);

    sendButton = new QPushButton(tr("Export"));
    saveButton = new QPushButton(tr("Save"));
    mainLayout->addWidget(saveButton, 3,0,1,1,Qt::AlignLeft);
    mainLayout->addWidget(resetButton,3,1,1,1,Qt::AlignLeft);
    mainLayout->addWidget(sendButton, 3,2,1,1,Qt::AlignRight);
    mainLayout->setColumnMinimumWidth(0,200);
    mainLayout->setColumnMinimumWidth(1,100);
    this->setLayout(mainLayout);
    connect(sendButton,SIGNAL(clicked()),this,SLOT(exportClicked()));
    connect(selectButton,SIGNAL(clicked()),filedialog01,SLOT(show()));
    connect(filedialog01,SIGNAL(fileSelected(QString)),fileLabel,SLOT(setText(QString)));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveStructure()));
    connect(sendButton,SIGNAL(clicked()),this,SLOT(saveStructure()));
    connect(resetButton,SIGNAL(clicked()),this,SLOT(resetValues()));

    this->setMinimumWidth(500);
   // this->setFixedSize(this->sizeHint());
    this->restoreStructure();
}

void exportData::exportClicked()
{
    if(fileLabel->text().isEmpty() || fileLabel->text().isNull())
    {
        QMessageBox msg;
        msg.setText(tr("Invalid File"));
        msg.exec();
        emit logMessage("Empty file string to export",LOG::ERRORMSG);
        return;
    }

    QStringList out;
    out.clear();
    for(int i = 0; i < MAX_FIELDS; i++)
    {
        if(comboList[i]->currentIndex() > 0) out.append(comboList[i]->currentText());
    }
    QString outfile = fileLabel->text();
    outfile.replace(".gvsim",".csv");
    emit startExport(fileLabel->text(),outfile,out);
    this->sendButton->setEnabled(false);
}
void exportData::statusUpdated(int prog, int total)
{
    this->progress->setMaximum(total);
    this->progress->setValue(prog);
}
void exportData::setFinished(bool status)
{
    this->sendButton->setEnabled(true);
    if(status)   emit logMessage("Export finished successfully",LOG::NORMAL);
    else         emit logMessage("Export failed",LOG::ERRORMSG);
}
void exportData::saveStructure()
{
    QSettings settings;
    for(int i = 0; i < MAX_FIELDS; i++)
    {
        settings.setValue("ExportStructure/"+QString::number(i),comboList[i]->currentText());
    }
}
void exportData::restoreStructure()
{
    QSettings settings;
    for(int i = 0; i < MAX_FIELDS; i++)
    {
        if(settings.value("ExportStructure/"+QString::number(i)).isValid()){
            int index = comboList[i]->findText(settings.value("ExportStructure/"+QString::number(i)).toString());
            comboList[i]->setCurrentIndex(index);
        }else{
            comboList[i]->setCurrentIndex(0);
        }
    }
}
void exportData::resetValues()
{
    for(int i = 0; i< MAX_FIELDS;i++)
    {
        if(i < comboList[i]->count()){
            comboList[i]->setCurrentIndex(i+1);
        }else{
            comboList[i]->setCurrentIndex(0);
        }
    }
}
