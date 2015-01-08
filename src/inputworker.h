#ifndef INPUTWORKER_H
#define INPUTWORKER_H

#include <SDL/SDL.h>
#ifdef WIN32
    #undef main
#endif
#include <QObject>
#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>
#include <QBuffer>
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QSlider>
#include <QMessageBox>
#include <QKeyEvent>
#include "simengine/planestatusdata.h"

/*!
 * \brief The inputWorker class handles the joystick of the system refreshing periodically its status
 */
class inputWorker : public QObject
{
    Q_OBJECT
public:
    explicit inputWorker(QObject *parent = 0);
    ~inputWorker();
private:
    QTimer joystickTimer;
    SDL_Joystick *joystick;
    QStringList joystickNames;

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
    QMap<int, int> multipliers;
    QMap<int, bool> inverted;
    QMap<int, int> deadzones;
    QMap<int, int> sensitivities;

    QMap<int, Sint16> axes;
    QMap<int, Uint8> buttons;
    QMap<int, Uint8> hats;
    QMap<int, QTime> axisRepeatTimers;
    QMap<int, QTime> buttonRepeatTimers;
    QMap<int, QTime> hatRepeatTimers;
    int keys[30];
    int axis[10];

    bool isOpen() { return joystick != NULL; }

    bool open(int stick);
    void close();
signals:
    void joystickOpened(int);
    void axisValueChanged(int axis, Sint16 value);                      /**< Emit new value for axis */
    void buttonValueChanged(int button, bool value);                    /**< Emit new status for button */
    void hatValueChanged(int hat, int value);                           /**< Emit new value for hat */
    void trackballValueChanged(int trackball, int deltaX, int deltaY);  /**< Emit new Value for trackball **/
    void changedSticks(Sint16, Sint16, Sint16, Sint16);
    void changedKeys(inputs::CONTROLKEYS);
    void requestAxes();     /**< request all axis values */
    void requestKeys();     /**< request all keys values */
    void requestJoystick(); /**< request all joysticks */
    void setJoystick(int index,bool enabled);                   /**< save joystick status */
    void setKeyValue(inputs::CONTROLKEYS key,int index);        /**< save key value */
    void setAxeValue(inputs::CONTROLAXES axe,int index);        /**< save axe value */
    void setInternalKeyValue(inputs::CONTROLKEYS key,int index);/**< set internal key value */
public slots:
    void changeJoystick(int);
    void manageKey(int key);
    void keyChangedInt(inputs::CONTROLKEYS,int);
    void restoreJoystick(int,bool);
    void restoreAxis(inputs::CONTROLAXES,int,bool);
    void restoreKey(inputs::CONTROLKEYS,int);
    void restored();
    void receiveKeyEvent(QKeyEvent *);
private slots:
    void processEvents();
    void toggledJoystick(bool);
};

#endif // INPUTWORKER_H
