#include "settingsmap.h"

settingsMap::settingsMap(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Map View Settings"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    selectMap = new QComboBox();
    selectMap->addItem("OpenStreetMap");
#ifndef WIN32
    selectMap->addItem("Google Earth");
#endif
    mainLayout->addWidget(selectMap);
    mainLayout->addStretch(100);
    this->setLayout(mainLayout);
    connect(selectMap,SIGNAL(currentIndexChanged(int)),this,SIGNAL(mapThemeChanged(int)));
    connect(selectMap,SIGNAL(currentIndexChanged(int)),this,SLOT(mapThemeSave(int)));

}
void settingsMap::restoreSettings()
{
    QSettings settings;
    if(settings.value("Map/Theme").isValid()){
        int val = settings.value("Map/Theme").toInt();
        if(val < selectMap->maxCount()){
            this->selectMap->setCurrentIndex(val);
        }
    }
}
void settingsMap::mapThemeSave(int map)
{
    QSettings settings;
    settings.setValue("Map/Theme",map);
}
