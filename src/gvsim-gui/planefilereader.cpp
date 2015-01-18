#include "planefilereader.h"

planeFileReader::planeFileReader(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Plane Model Editor"));
    this->setWindowIcon(QIcon(QPixmap(":/icons/flight.png")));
    this->setMinimumSize(640,400);
    filelabel01 = new QLabel();
    loadButton = new QPushButton(tr("Load"));
    openFileButton = new QPushButton(tr("Open File"));
    fileDialog01 = new QFileDialog();
    edit01 = new QTextEdit();
    syntax01 = new XmlHighlighter(edit01);
    QPushButton *closeButton = new QPushButton(tr("Close"));
    filelabel01->setStyleSheet("background: #EEE; border: 1px solid #777;border-radius:3px;");
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *toolsLayout = new QHBoxLayout();
    QHBoxLayout *fileLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch(100);
    bottomLayout->addWidget(closeButton);
    toolsLayout->addStretch(100);
    toolsLayout->addWidget(loadButton);
    fileLayout->addWidget(filelabel01,10);
    fileLayout->addWidget(openFileButton,1);
    mainLayout->addLayout(toolsLayout);
    mainLayout->addLayout(fileLayout);
    mainLayout->addWidget(edit01);
    mainLayout->addLayout(bottomLayout);

    this->setLayout(mainLayout);
    connect(openFileButton,SIGNAL(clicked()),fileDialog01,SLOT(show()));
    connect(fileDialog01,SIGNAL(fileSelected(QString)),this,SLOT(openFile(QString)));
    connect(fileDialog01,SIGNAL(fileSelected(QString)),filelabel01,SLOT(setText(QString)));
    connect(loadButton,SIGNAL(clicked()),this,SLOT(loadEditContent()));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));
 #ifdef WIN32
    //this->openFile(QCoreApplication::applicationDirPath()+"/predator.xml");
    this->openFile(":/files/predator.xml");
#else
    //this->openFile(QDir::homePath()+"/.gvsim/predator.xml");
    this->openFile(":/files/predator.xml");
#endif
    qDebug() << "Preloading Plane File" << QCoreApplication::applicationDirPath()+"/predator.xml";
}
/*!
 * \brief planeFileReader::openFile start reading the provided file
 * \param _file file to read
 */
void planeFileReader::openFile(QString _file)
{
    QFile file(_file);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream read(&file);
        edit01->append(read.readAll());
        filelabel01->setText(_file);
    }
}
/*!
 * \brief planeFileReader::readXML read the XML content
 * \param _content XML content
 * \param ok boolean containing the read out status
 */
void planeFileReader::readXML(QString _content,bool *ok)
{
    *ok = false;
    bool okval;
    QDomDocument document;
    QString error;
    int row,column;
    document.setContent(_content,&error,&row,&column);
    QDomElement docElem = document.documentElement();
    QDomElement constants = docElem.firstChildElement("constants");
    QDomElement landinggear = docElem.firstChildElement("landingGear");
    QDomElement masses = docElem.firstChildElement("masses");
    QDomElement derivatives = docElem.firstChildElement("derivatives");
    QDomElement propulsor = docElem.firstChildElement("propulsor");
    QDomElement control = docElem.firstChildElement("control");

    qDebug() << "Reading Data Started";
    //DATA

    //LANDING GEAR
    planeModel.cBreak = getConstantValue(landinggear,"cBreak",&okval);
    if(!okval){
        logMessage("Problem Reading cBreak",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.cBreakout = getConstantValue(landinggear,"cBreakout",&okval);
    if(!okval){
        logMessage("Problem Reading cBreakout",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.cSide = getConstantValue(landinggear,"cSide",&okval);
    if(!okval){
        logMessage("Problem Reading cSide",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.cRolling = getConstantValue(landinggear,"cRolling",&okval);
    if(!okval){
        logMessage("Problem Reading cRolling",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.speedLimit = getConstantValue(landinggear,"speedLimit",&okval);
    if(!okval){
        logMessage("Problem Reading speedLimit",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.noseGearDef = getConstantValue(landinggear,"noseGearDef",&okval);
    if(!okval){
        logMessage("Problem Reading noseGearDef",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readLandingGear(landinggear.firstChildElement("noseWheel"),&planeModel.landingWheels[0]))
    {
        logMessage("Problem Reading Nose Wheel",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readLandingGear(landinggear.firstChildElement("mainWheel1"),&planeModel.landingWheels[1]))
    {
        logMessage("Problem Reading Main Wheel 1",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readLandingGear(landinggear.firstChildElement("mainWheel2"),&planeModel.landingWheels[2]))
    {
        logMessage("Problem Reading Main Wheel 2",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    //CLAlpha
    if(!this->readMap(derivatives.firstChildElement("cLalpha"),&planeModel.cLalpha))
    {
        logMessage("Wrong cLalpha Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    //cLalphaGround
    if(!this->readMap(derivatives.firstChildElement("cLalphaGround"),&planeModel.cLalphaGround))
    {
        logMessage("Wrong cLalphaGround Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    //deltaCmGround
    if(!this->readMap(derivatives.firstChildElement("deltaCmGround"),&planeModel.deltaCmGround))
    {
        logMessage("Wrong deltaCmGround Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    //CDalpha
    if(!this->readMap(derivatives.firstChildElement("cDalpha"),&planeModel.cDalpha))
    {
        logMessage("Wrong cDalpha Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readSingleCurve(derivatives.firstChildElement("deltaCmFlaps"),&planeModel.deltaCmFlaps))
    {
        logMessage("Wrong deltaCmFlaps Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    //MOTOR CURVES

    if(!this->readMap(propulsor.firstChildElement("AltIsaCorrection"),&planeModel.AltIsaCorrection))
    {
        logMessage("Wrong AltIsaCorrection Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readMap(propulsor.firstChildElement("ctTable"),&planeModel.cT))
    {
        logMessage("Wrong ctTable Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readMap(propulsor.firstChildElement("cpTable"),&planeModel.cP))
    {
        logMessage("Wrong  cpTable Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readSingleCurve(propulsor.firstChildElement("PerformanceRPM"),&planeModel.rpmMaxP))
    {
        logMessage("Wrong Performance vs RPM Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    //MOTOR CONSTANS
    if(!this->readVector(propulsor.firstChildElement("Tvector"),planeModel.engVector))
    {
        logMessage("Problem Reading TVector",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readVector(propulsor.firstChildElement("Tposition"),planeModel.engPosition))
    {
        logMessage("Problem Reading Tposition",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.ce = getConstantValue(propulsor,"ce",&okval);
    if(!okval){
        logMessage("Problem Reading ce",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.takeOffPerformance = getConstantValue(propulsor,"takeOffPerformance",&okval);
    if(!okval){
        logMessage("Problem Reading takeOffPerformance",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.min_pitch = getConstantValue(propulsor,"minpitch",&okval);
    if(!okval){
        logMessage("Problem Reading minpitch",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.max_pitch = getConstantValue(propulsor,"maxpitch",&okval);
    if(!okval){
        logMessage("Problem Reading maxpitch",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.propellerIxx = getConstantValue(propulsor,"propellerIxx",&okval);
    if(!okval){
        logMessage("Problem Reading propellerIxx",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.red_factor = getConstantValue(propulsor,"reductionFactor",&okval);
    if(!okval){
        logMessage("Problem Reading reductionFactor",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.propDiam = getConstantValue(propulsor,"diameter",&okval);
    if(!okval){
        logMessage("Problem Reading prop diameter",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.controlKp = getConstantValue(propulsor,"controlkp",&okval);
    if(!okval){
        logMessage("Problem Reading prop controlkp",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.controlKd = getConstantValue(propulsor,"controlkd",&okval);
    if(!okval){
        logMessage("Problem Reading prop controlkd",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.powerExtraction = getConstantValue(propulsor,"powerExtraction",&okval);
    if(!okval){
        logMessage("Problem Reading powerExtraction",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readConstant(propulsor.firstChildElement("minrpm"),&planeModel.min_rpm))
    {
        logMessage("Wrong Min RPM Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readConstant(propulsor.firstChildElement("maxrpm"),&planeModel.max_rpm))
    {
        logMessage("Wrong Max RPM Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    //FUEL
    if(!this->readFuelStruct(masses.firstChildElement("fuel_tank"),&planeModel.fuel_tank))
    {
        logMessage("Wrong Fuel Tank Mass Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    //MASSES
    if(!this->readMassStruct(masses.firstChildElement("gearIn"),&planeModel.gearIn))
    {
        logMessage("Wrong  GearIn Mass Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readMassStruct(masses.firstChildElement("gearOut"),&planeModel.gearOut))
    {
        logMessage("Wrong  gearOut Mass Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readMassStruct(masses.firstChildElement("missile1"),&planeModel.missile[0]))
    {
        logMessage("Wrong  Missile1 Mass Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readMassStruct(masses.firstChildElement("missile2"),&planeModel.missile[1]))
    {
        logMessage("Wrong Missile2 Mass Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readMassStruct(masses.firstChildElement("dry_plane"),&planeModel.dryplane))
    {
        logMessage("Wrong Dryplane Mass Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!this->readMassStruct(masses.firstChildElement("miscellaneous"),&planeModel.miscelaneous))
    {
        logMessage("Wrong Dryplane Mass Data",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    //DEFLECTIONS
    planeModel.alphamax = getConstantValue(constants,"alphamax",&okval);
    if(!okval){
        logMessage("Problem Reading alphamax",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.betamax = getConstantValue(constants,"betamax",&okval);
    if(!okval){
        logMessage("Problem Reading betamax",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    planeModel.ail_def = getConstantValue(constants,"ailerondef",&okval);
    if(!okval){
        logMessage("Problem Reading ailerondef",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.rud_def = getConstantValue(constants,"rudderdef",&okval);
    if(!okval){
        logMessage("Problem Reading rudderdef",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.ele_def = getConstantValue(constants,"elevatordef",&okval);
    if(!okval){
        logMessage("Problem Reading elevatordef",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    //CONSTANTS
    planeModel.Sw = getConstantValue(constants,"Sw",&okval);
    if(!okval){
        logMessage("Problem Reading Sw",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.b = getConstantValue(constants,"b",&okval);
    if(!okval){
        logMessage("Problem Reading b",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.cmg = getConstantValue(constants,"cmg",&okval);
    if(!okval){
        logMessage("Problem Reading cmg",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.cma = getConstantValue(constants,"cma",&okval);
    if(!okval){
        logMessage("Problem Reading cma",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    //DERIVATIVES
    planeModel.CLde = getConstantValue(derivatives,"cLde",&okval);
    if(!okval){
        logMessage("Problem Reading cLde",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    if(!readSingleCurve(derivatives.firstChildElement("cLalphadot"),&planeModel.CLalphadot)){
        logMessage("Problem Reading cLalphadot",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readSingleCurve(derivatives.firstChildElement("cLq"),&planeModel.CLq)){
        logMessage("Problem Reading cLq",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.CDde = getConstantValue(derivatives,"cDde",&okval);
    if(!okval){
        logMessage("Problem Reading cDde",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.CDalphadot = getConstantValue(derivatives,"cDalphadot",&okval);
    if(!okval){
        logMessage("Problem Reading cDalphadot",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.CDq = getConstantValue(derivatives,"cDq",&okval);
    if(!okval){
        logMessage("Problem Reading cDq",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.Cyda = getConstantValue(derivatives,"cyda",&okval);
    if(!okval){
        logMessage("Problem Reading cyda",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.Cybeta = getConstantValue(derivatives,"cybeta",&okval);
    if(!okval){
        logMessage("Problem Reading cybeta",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readMap(derivatives.firstChildElement("cyr"),&planeModel.Cyr)){
        logMessage("Problem Reading cyr",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readMap(derivatives.firstChildElement("cyp"),&planeModel.Cyp)){
        logMessage("Problem Reading cyp",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.Cydr = getConstantValue(derivatives,"cydr",&okval);
    if(!okval){
        logMessage("Problem Reading cydr",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readSingleCurve(derivatives.firstChildElement("cybetadot"),&planeModel.Cybetadot)){
        logMessage("Problem Reading cybetadot",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.Cm0 = getConstantValue(derivatives,"cm0",&okval);
    if(!okval){
        logMessage("Problem Reading cm0",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.Cmalpha = getConstantValue(derivatives,"cmalpha",&okval);
    if(!okval){
        logMessage("Problem Reading cmalpha",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readSingleCurve(derivatives.firstChildElement("cmde"),&planeModel.Cmde)){
        logMessage("Problem Reading cmde",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readSingleCurve(derivatives.firstChildElement("cmq"),&planeModel.Cmq)){
        logMessage("Problem Reading cmq",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readSingleCurve(derivatives.firstChildElement("cmalphadot"),&planeModel.Cmalphadot)){
        logMessage("Problem Reading cmalphadot",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readMap(derivatives.firstChildElement("clbeta"),&planeModel.Clbeta)){
        logMessage("Problem Reading clbeta",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readSingleCurve(derivatives.firstChildElement("clbetadot"),&planeModel.Clbetadot)){
        logMessage("Problem Reading clbetadot",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readMap(derivatives.firstChildElement("cldr"),&planeModel.Cldr)){
        logMessage("Problem Reading cldr",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.Clda = getConstantValue(derivatives,"clda",&okval);
    if(!okval){
        logMessage("Problem Reading clda",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.Clp = getConstantValue(derivatives,"clp",&okval);
    if(!okval){
        logMessage("Problem Reading clp",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readMap(derivatives.firstChildElement("clr"),&planeModel.Clr)){
        logMessage("Problem Reading clr",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readMap(derivatives.firstChildElement("cnbeta"),&planeModel.Cnbeta)){
        logMessage("Problem Reading cnbeta",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readSingleCurve(derivatives.firstChildElement("cnbetadot"),&planeModel.Cnbetadot)){
        logMessage("Problem Reading cnbetadot",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readMap(derivatives.firstChildElement("cndr"),&planeModel.Cndr)){
        logMessage("Problem Reading cndr",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.Cnda = getConstantValue(derivatives,"cnda",&okval);
    if(!okval){
        logMessage("Problem Reading cnda",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readMap(derivatives.firstChildElement("cnp"),&planeModel.Cnp)){
        logMessage("Problem Reading cnp",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(!readMap(derivatives.firstChildElement("cnr"),&planeModel.Cnr)){
        logMessage("Problem Reading cnr",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.cdGear = getConstantValue(derivatives,"cDgear",&okval);
    if(!okval){
        logMessage("Problem Reading cDgear",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.cdMissile= getConstantValue(derivatives,"cDmissile",&okval);
    if(!okval){
        logMessage("Problem Reading cDmissile",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    planeModel.groundEffectLimit = getConstantValue(constants,"groundEffectLimit",&okval);
    if(!okval){
        logMessage("Problem Reading groundEffectLimit",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.groundEffectBase = getConstantValue(constants,"groundEffectBase",&okval);
    if(!okval){
        logMessage("Problem Reading groundEffectBase",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    if(planeModel.groundEffectLimit - planeModel.groundEffectBase < 0.0){
        logMessage("groundEffectBase must be lower than groundEffectLimit",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    //CONTROL
    planeModel.ki_speed = getConstantValue(control.firstChildElement("speed"),"ki",&okval);
    if(!okval){
        logMessage("Problem Reading ki_speed",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.kp_speed = getConstantValue(control.firstChildElement("speed"),"kp",&okval);
    if(!okval){
        logMessage("Problem Reading kp_speed",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.kd_speed = getConstantValue(control.firstChildElement("speed"),"kd",&okval);
    if(!okval){
        logMessage("Problem Reading kd_speed",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    planeModel.ki_pitch = getConstantValue(control.firstChildElement("pitch"),"ki",&okval);
    if(!okval){
        logMessage("Problem Reading ki_pitch",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.kp_pitch = getConstantValue(control.firstChildElement("pitch"),"kp",&okval);
    if(!okval){
        logMessage("Problem Reading kp_pitch",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.kd_pitch = getConstantValue(control.firstChildElement("pitch"),"kd",&okval);
    if(!okval){
        logMessage("Problem Reading kd_pitch",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.minmax_pitch = getConstantValue(control.firstChildElement("pitch"),"minmax",&okval);
    if(!okval){
        logMessage("Problem Reading kd_pitch",LOG::ERRORMSG);
        *ok = false;
        return;
    }

    planeModel.ki_alt = getConstantValue(control.firstChildElement("altitude"),"ki",&okval);
    if(!okval){
        logMessage("Problem Reading ki_alt",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.kp_alt = getConstantValue(control.firstChildElement("altitude"),"kp",&okval);
    if(!okval){
        logMessage("Problem Reading kp_alt",LOG::ERRORMSG);
        *ok = false;
        return;
    }
    planeModel.kd_alt = getConstantValue(control.firstChildElement("altitude"),"kd",&okval);
    if(!okval){
        logMessage("Problem Reading kd_alt",LOG::ERRORMSG);
        *ok = false;
        return;
    }



    *ok = true;
    logMessage("XML File readed Successfully",LOG::INFO);

    //THIS PREVENTS WHILE DEBUGGING
    planeModel.cbar = planeModel.cmg;
}
/*!
 * \brief planeFileReader::loadEditContent processes the content loaded in the textbox
 */
void planeFileReader::loadEditContent()
{
    bool ok;
    this->readXML(edit01->toPlainText(),&ok);
    qDebug()<< "Read XML" << ok;
    if(ok) emit newPlaneModel(this->planeModel);
}
curveHandler planeFileReader::readCurve(QString in, bool *ok)
{
    curveHandler curve;
    QString clean = in.trimmed();
    QStringList lines = clean.split(QRegExp("(\\n)(\\t)?"));
    if(lines.size() <= 0)
    {
        *ok = false;
        return curve;
    }
    for(int i = 0; i< lines.size();i++)
    {
        QStringList data = lines.at(i).trimmed().split(QRegExp("((\\t)+)|((\\s)+)"));
        bool ok1, ok2;
        float xdata = data.at(0).trimmed().toFloat(&ok1);
        float ydata = data.at(1).trimmed().toFloat(&ok2);
        if(!ok1 || !ok2){
            *ok = false;
            return curve;
        }
        curve.addPoint(QPointF(xdata,ydata));
    }
    *ok = true;
    return curve;
}
float planeFileReader::getConstantValue(QDomElement constantElement, QString name, bool *ok)
{
    bool ok1 = !constantElement.firstChildElement(name).isNull();
    bool ok2;
    float value = constantElement.firstChildElement(name).text().trimmed().toFloat(&ok2);
    *ok = ok1 && ok2;
    return value;
}
bool planeFileReader::readSingleCurve(QDomElement singleCurveElement, curveHandler *out)
{
    bool valid;
    if(singleCurveElement.childNodes().size() != 1 || !singleCurveElement.firstChild().isText() || singleCurveElement.isNull()){
        return false;
    }else{
        *out = readCurve(singleCurveElement.text(),&valid);
        return valid;
    }
}
bool planeFileReader::readMultiCurve(QDomElement multiCurveElement, curve3DHandler *out)
{
    if(!multiCurveElement.hasChildNodes() || multiCurveElement.isNull() ){
        return false;
    }else{
        QDomNodeList curve = multiCurveElement.childNodes();
        for(int j = 0; j < curve.size(); j++)
        {
            bool ok1, ok2;
            float value = curve.at(j).attributes().namedItem("value").toAttr().value().toFloat(&ok1);
            QString stringcurve = curve.at(j).firstChild().toText().data();
            curveHandler curve = this->readCurve(stringcurve,&ok2);
            if(ok1 && ok2){
                out->addCurve(value,curve);
                qDebug() << out->getValue(0.0,0.0,&ok1);
            }else{
                return false;
            }
        }
    }
    return true;
}
bool planeFileReader::readMap(QDomElement mapElement, curve3DHandler *out)
{
    if(mapElement.isNull() ){
        return false;
    }else{
        out->clear();
        bool ok1, ok2;
        int fieldsNum = 0;
        QString clean = mapElement.text().trimmed();
        QStringList lines = clean.split(QRegExp("(\\n)(\\t)?"));
        QStringList header = lines.at(0).trimmed().split(QRegExp("((\\t)+)|((\\s)+)"));
        curveHandler curve;

        fieldsNum = header.size();
        for(int j = 1; j < lines.size(); j++)
        {
            curve.clear();
            QStringList fields = lines.at(j).trimmed().split(QRegExp("((\\t)+)|((\\s)+)"));
            if(fields.size() != fieldsNum+1) return false;
            float value = fields.at(0).trimmed().toFloat(&ok1);

            if(!ok1) return false;
            for(int i = 1; i <fields.size();i++)
            {
                float x = header.at(i-1).trimmed().toFloat(&ok1);
                float y = fields.at(i).trimmed().toFloat(&ok2);
                if(!(ok1 && ok2)) return false;
                curve.addPoint(QPointF(x,y));
            }
            out->addCurve(value,curve);
        }
        return true;
    }
}
bool planeFileReader::readMassStruct(QDomElement mapElement, massStruct *mass_struct)
{
    if(!this->readConstant(mapElement.firstChildElement("mass"),&mass_struct->mass))
    {
        return false;
    }
    if(!this->readPosition(mapElement.firstChildElement("position"),mass_struct))
    {
        return false;
    }
    if(!this->readTensor(mapElement.firstChildElement("inertia"),mass_struct))
    {
        return false;
    }
    return true;
}
bool planeFileReader::readFuelStruct(QDomElement mapElement, fuelStruct *fuel_struct)
{
    if(!this->readConstant(mapElement.firstChildElement("max_mass"),&fuel_struct->max_mass))
    {
        return false;
    }
    if(!this->readFuelPosition(mapElement.firstChildElement("basecenter"),fuel_struct))
    {
        return false;
    }
    if(!this->readConstant(mapElement.firstChildElement("width"),&fuel_struct->width))
    {
        return false;
    }
    if(!this->readConstant(mapElement.firstChildElement("height"),&fuel_struct->height))
    {
        return false;
    }
    if(!this->readConstant(mapElement.firstChildElement("lenght"),&fuel_struct->length))
    {
        return false;
    }
    return true;
}
bool planeFileReader::readLandingGear(QDomElement mapElement, wheelStruct *wheel)
{
    if(!this->readConstant(mapElement.firstChildElement("kspring"),&wheel->kspring))
    {
        return false;
    }
    if(!this->readConstant(mapElement.firstChildElement("kdamper"),&wheel->kdamper))
    {
        return false;
    }
    if(!this->readVector(mapElement.firstChildElement("position"),wheel->position))
    {
        return false;
    }
    return true;
}

bool planeFileReader::readTensor(QDomElement mapElement, massStruct *mass)
{
    if(mapElement.isNull() ){
        return false;
    }else{
        bool ok2;
        QString clean = mapElement.text().trimmed();
        QStringList lines = clean.split(QRegExp("(\\n)(\\t)?"));
        if(lines.size() != 3) return false;
        for(int j = 0; j < lines.size(); j++)
        {
            QStringList fields = lines.at(j).trimmed().split(QRegExp("((\\t)+)|((\\s)+)"));
            if(fields.size() != 3) return false;
            for(int i = 0; i <fields.size();i++)
            {
                float value = fields.at(i).trimmed().toFloat(&ok2);
                if(!ok2) return false;
                mass->inertia[j][i] = value;
            }
        }
        return true;
    }
}
bool planeFileReader::readPosition(QDomElement mapElement, massStruct *mass)
{
    if(mapElement.isNull() ){
        return false;
    }else{
        bool ok2;
        QString clean = mapElement.text().trimmed();
        QStringList lines = clean.split(QRegExp("(\\n)(\\t)?"));
        if(lines.size() != 1) return false;
        QStringList fields = lines.at(0).trimmed().split(QRegExp("((\\t)+)|((\\s)+)"));

        if(fields.size() != 3) return false;
        for(int i = 0; i <fields.size();i++)
        {
            float value = fields.at(i).trimmed().toFloat(&ok2);
            if(!ok2) return false;
            mass->position[i] = value;
        }
        return true;
    }
}
bool planeFileReader::readVector(QDomElement mapElement, float vector[3])
{
    if(mapElement.isNull() ){
        return false;
    }else{
        bool ok2;
        QString clean = mapElement.text().trimmed();
        QStringList lines = clean.split(QRegExp("(\\n)(\\t)?"));
        if(lines.size() != 1) return false;
        QStringList fields = lines.at(0).trimmed().split(QRegExp("((\\t)+)|((\\s)+)"));

        if(fields.size() != 3) return false;
        for(int i = 0; i <fields.size();i++)
        {
            float value = fields.at(i).trimmed().toFloat(&ok2);
            if(!ok2) return false;
            vector[i] = value;
        }
        return true;
    }
}

bool planeFileReader::readFuelPosition(QDomElement mapElement, fuelStruct *fuel)
{
    if(mapElement.isNull() ){
        return false;
    }else{
        bool ok2;
        QString clean = mapElement.text().trimmed();
        QStringList lines = clean.split(QRegExp("(\\n)(\\t)?"));
        if(lines.size() != 1) return false;
        QStringList fields = lines.at(0).trimmed().split(QRegExp("((\\t)+)|((\\s)+)"));

        if(fields.size() != 3) return false;
        for(int i = 0; i <fields.size();i++)
        {
            float value = fields.at(i).trimmed().toFloat(&ok2);
            if(!ok2) return false;
            fuel->basecenter[i] = value;
        }
        return true;
    }
}
bool planeFileReader::readConstant(QDomElement constantElement, float *output)
{
    bool ok1 = !constantElement.isNull();
    bool ok2;
    *output = constantElement.text().trimmed().toFloat(&ok2);
    return ok1 && ok2;

}
