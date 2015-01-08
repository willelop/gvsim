#ifndef SETTINGSMAP_H
#define SETTINGSMAP_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QSettings>
/*!
 * \brief The settingsMap class allows to modify map settings such as the theme
 */
class settingsMap : public QDialog
{
    Q_OBJECT
public:
    explicit settingsMap(QWidget *parent = 0);
private:
    QComboBox *selectMap;
signals:
    void mapThemeChanged(int map); ///<  \brief emits the index of the map when changed
public slots:
    void restoreSettings();
private slots:
    void mapThemeSave(int map);
};

#endif // SETTINGSMAP_H
