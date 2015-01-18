#ifndef SETTINGSJOYSTICK_H
#define SETTINGSJOYSTICK_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <SDL/SDL.h>
#ifdef WIN32
    #undef main
#endif
#include <QTimer>
#include <QTime>
#include <QCheckBox>
#include <QKeyEvent>
#include "stickdisplay.h"
#include "typedefs.h"
#include <QDebug>
#include <QSettings>
#include "keyselector.h"

/*!
 * \brief The settingsJoystick class is the widget to modify the input configuration, both joystick and keyboard
 */
class settingsJoystick : public QDialog
{
    Q_OBJECT
public:
    explicit settingsJoystick(QWidget *parent = 0);
    ~settingsJoystick();
private:
    bool islocked;
    QSettings *settings;
    QTimer *postTimer;
    QComboBox *joystickSelectBox;
    QComboBox *aileronSelect;
    QComboBox *elevatorSelect;
    QComboBox *rudderSelect;
    QComboBox *throttleSelect;
    QCheckBox *aileronICheck;
    QCheckBox *rudderICheck;
    QCheckBox *elevatorICheck;
    QCheckBox *throttleICheck;
    QHBoxLayout *controlSelectorBox;
    QHBoxLayout *keysMainLayout;
    QVBoxLayout *keysLeftLayout;
    QVBoxLayout *keysRightLayout;
    QGroupBox *joystickgroup;
    SDL_Joystick *joystick;
    QStringList joystickNames;
    QString activeName;
    QCheckBox *enableJoyBox;
    stickDisplay *stick1;
    stickDisplay *stick2;
    //KEYS
    keySelector *elevatorup;
    keySelector *elevatordown;
    keySelector *aileronleft;
    keySelector *aileronright;
    keySelector *rudderleft;
    keySelector *rudderright;
    keySelector *throttleup;
    keySelector *throttledown;
    keySelector *breaks;
    keySelector *geartoogle;

    int channels[4];
    bool haveJoystick;
    bool joyenabled;
    bool loaded;
    int numAxes;
    int joystickIndex;
    int joystickNum;
    int numButtons;
    int numHats;
    int numTrackballs;

    int eventTimeout;
    int autoRepeatDelay;
    bool autoRepeat;
    bool inverted[4];

    QMap<int, Sint16> axes;
    QMap<int, Uint8> buttons;
    QMap<int, Uint8> hats;
    QMap<int, QTime> axisRepeatTimers;
    QMap<int, QTime> buttonRepeatTimers;
    QMap<int, QTime> hatRepeatTimers;

    bool isOpen() { return joystick != NULL; }

    bool open(int stick);
    void close();
    void updateSelectors();
    void saveJoystick(QString,bool);
    void saveAxeValue(QString _joy,inputs::CONTROLAXES, int,bool);
    void requestJoystickSettings(QString _joy);
    void restoreKeysValues();
    void lock(bool);

signals:
    void message2log(QString, LOG::TYPE);
    void requestOpenJoy(int);
    void joystickEnabledChange(bool);
    void requestJoyInfo(QString);
    void requestAxes();
    void requestKeys();
    void requestJoystick();
    void setInternalKeyValue(inputs::CONTROLKEYS,int);
    void newAxeValue(inputs::CONTROLAXES,int,bool);
    void newKeyValue(inputs::CONTROLKEYS,int);
public slots:
    void printValue(inputs::CONTROLAXES axis, Sint16 value);
    void restoreJoystick(int,bool);
    void restoreAxis(inputs::CONTROLAXES,int,bool);
    void restoreAll();
    void restored();
    void restoreSettings();
private slots:
    void changeJoystick(int);
    void changeAilerons(int);
    void changeElevator(int);
    void changeRudder(int);
    void changeThrottle(int);
    void invertElevator(bool);
    void invertAilerons(bool);
    void invertThrottle(bool);
    void invertRudder(bool);
    void saveKeyValue(inputs::CONTROLKEYS, int);
    void selectJoy(int);
    void toggledJoystick(bool);
    void receivedChangedSticks(Sint16, Sint16, Sint16, Sint16);
};

#endif // SETTINGSJOYSTICK_H
