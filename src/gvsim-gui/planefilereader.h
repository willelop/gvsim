#ifndef PLANEFILEREADER_H
#define PLANEFILEREADER_H

#include <QWidget>
#include <QPushButton>
#include <QCoreApplication>
#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include <QTextEdit>
#include <QGridLayout>
#include <QTextStream>
#include <QDebug>
#include <QLineEdit>
#include <QXmlStreamReader>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QDomAttr>
#include "planemodeldata.h"
#include "curve3dhandler.h"
#include "curvehandler.h"
#include "typedefs.h"
#include "utils/xmlhighlighter.h"
/*!
 * \brief The planeFileReader class reads the plane data file and provides a valid internal plane structure
 */
class planeFileReader : public QDialog
{
    Q_OBJECT
public:
    explicit planeFileReader(QWidget *parent = 0);
private:
    XmlHighlighter *syntax01;
    QTextEdit *edit01;
    QLabel *filelabel01;
    QPushButton *openFileButton;
    QPushButton *loadButton;
    QFileDialog *fileDialog01;
    planeModelData planeModel;
    curveHandler readCurve(QString in,bool *ok);
    float getConstantValue(QDomElement constantElement, QString name, bool *ok);
    bool readConstant(QDomElement constantElement, float *output);
signals:
    void logMessage(QString,LOG::TYPE);
    void newPlaneModel(planeModelData);
private slots:
    void readXML(QString,bool *ok);
    bool readSingleCurve(QDomElement singleCurveElement,curveHandler *out);
    bool readMultiCurve(QDomElement multiCurveElement,curve3DHandler *out);
    bool readMap(QDomElement mapElement,curve3DHandler *out);
    bool readTensor(QDomElement mapElement,massStruct *mass);
    bool readPosition(QDomElement mapElement,massStruct *mass);
    bool readVector(QDomElement mapElement,float vector[3]);
    bool readLandingGear(QDomElement mapElement,wheelStruct *wheel);

    bool readFuelPosition(QDomElement mapElement, fuelStruct *fuel);
    bool readMassStruct(QDomElement mapElement,massStruct *mass_struct);
    bool readFuelStruct(QDomElement mapElement, fuelStruct *fuel_struct);

public slots:
    void openFile(QString);
    void loadEditContent();

};

#endif // PLANEFILEREADER_H
