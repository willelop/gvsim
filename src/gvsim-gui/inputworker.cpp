#include "inputworker.h"

inputWorker::inputWorker(QObject *parent) :
    QObject(parent)
{
    haveJoystick = false;
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == 0 ) {
        SDL_EnableUNICODE( 1 );
        int i;
        joystickNum = SDL_NumJoysticks();
        for (i = 0; i < SDL_NumJoysticks(); i++) joystickNames.append(SDL_JoystickName(i));
        connect(&joystickTimer, SIGNAL(timeout()), this, SLOT(processEvents()));
        haveJoystick = true;
        joyenabled = false;
    } else {
        joystickNum = 0;
        haveJoystick = false;
        fprintf(stderr, "ERROR: couldn't initialize SDL joystick support");
    }

    eventTimeout = 100;
    autoRepeatDelay = 500;
    joystick = NULL;
    if(haveJoystick) {
        open(0);
    }
}
inputWorker::~inputWorker()
{
  if ( isOpen() )
    close();

  SDL_Quit();
}
bool inputWorker::open(int stick)
{
    if ( isOpen() )
      close();
    joystick = SDL_JoystickOpen(stick);
    if ( joystick ) {
      numAxes = SDL_JoystickNumAxes(joystick);
      numButtons = SDL_JoystickNumButtons(joystick);
      numHats = SDL_JoystickNumHats(joystick);
      numTrackballs = SDL_JoystickNumBalls(joystick);
      joystickTimer.start(eventTimeout);
      for(int i = 0; i< numAxes;i++)
      {
          deadzones[i] = 2000;
          sensitivities[i] = 10;
          inverted[i] = false;
      }
      return true;
    } else {
      fprintf(stderr, "ERROR: couldn't open SDL joystick #%d", stick);
      return false;
    }
}
void inputWorker::close()
{
  joystickTimer.stop();
  if ( joystick )
    SDL_JoystickClose(joystick);
  joystick = NULL;
  numAxes = numButtons = numHats = numTrackballs = 0;
}

void inputWorker::processEvents()
{
    SDL_Event event;
    int value;
    Sint16 delta;
    bool changed = false;
    int index;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_JOYAXISMOTION:
            changed = true;
            index = event.jaxis.axis;
            value = event.jaxis.value;
            delta = abs(axes[index] - value);
            if(abs(value) >= deadzones[index])
            {
                if(value > 0) value -= deadzones[index];
                if(value < 0) value += deadzones[index];
                value *= 32767/(32767-deadzones[index]);
                if(value > 32767)       axes[index] = 32767;
                else if(value < -32767) axes[index] = -32767;
                else                    axes[index] = value;
            }else{
                axes[index] = 0;
            }
            break;
        case SDL_JOYBUTTONDOWN:
            index = event.jbutton.state;
            emit buttonValueChanged(index,true);
            break;
        case SDL_JOYBUTTONUP:
            index = event.jbutton.state;
            emit buttonValueChanged(index, false);
            break;
        }
    }
    if(changed)
    {
        emit changedSticks(multipliers[axis[inputs::ELEVATORAXE]]*axes[axis[inputs::ELEVATORAXE]],multipliers[axis[inputs::AILERONAXE]]*axes[axis[inputs::AILERONAXE]],multipliers[axis[inputs::THROTTLEAXE]]*axes[axis[inputs::THROTTLEAXE]],multipliers[axis[inputs::RUDDERAXE]]*axes[axis[inputs::RUDDERAXE]]);
    }
}
/*!
 * \brief inputWorker::changeJoystick changes the joystick num
 * \param _joystick joystick index
 */
void inputWorker::changeJoystick(int _joystick)
{
    if(isOpen())    close();
    open(_joystick);
    this->joystickIndex = _joystick;
    emit joystickOpened(_joystick);
}

/*!
 * \brief inputWorker::manageKey handles a keypress
 * \param key pressed key
 */
void inputWorker::manageKey(int key)
{
    /*!
     * Handle the keyPress and emit the necesary signal
     */
    if(key == keys[inputs::AILERONLEFT]) emit changedKeys(inputs::AILERONLEFT);
    else if(key == keys[inputs::AILERONRIGHT]) emit changedKeys(inputs::AILERONRIGHT);
    else if(key == keys[inputs::RUDDERRIGHT]) emit changedKeys(inputs::RUDDERRIGHT);
    else if(key == keys[inputs::RUDDERLEFT]) emit changedKeys(inputs::RUDDERLEFT);
    else if(key == keys[inputs::ELEVATORUP]) emit changedKeys(inputs::ELEVATORUP);
    else if(key == keys[inputs::ELEVATORDOWN]) emit changedKeys(inputs::ELEVATORDOWN);
    else if(key == keys[inputs::THROTTLEUP]) emit changedKeys(inputs::THROTTLEUP);
    else if(key == keys[inputs::THROTTLEDOWN]) emit changedKeys(inputs::THROTTLEDOWN);
    else if(key == keys[inputs::BREAKS]) emit changedKeys(inputs::BREAKS);
    else if(key == keys[inputs::GEARTOGGLE]) emit changedKeys(inputs::GEARTOGGLE);


}
/*!
 * \brief inputWorker::restoreJoystick restore a saved joystick
 * \param joy joystick
 * \param enabled enabled
 */
void inputWorker::restoreJoystick(int joy, bool enabled){
    /*!
     * Slot receiving restoration information about the status of a joystick
     */
    joyenabled = enabled;
    close();
    open(joy);
}
/*!
 * \brief inputWorker::restoreAxis restore a saved axis
 * \param axe axis
 * \param axeval axis value
 * \param _inverted is inverted
 */
void inputWorker::restoreAxis(inputs::CONTROLAXES axe, int axeval,bool _inverted)
{
    /*!
     * Slot restoring information about axis axignation
     */
    axis[axe] = axeval;
    inverted[axeval] = _inverted;
    for(int i = 0; i< numAxes; i++)
    {
        if(inverted[i]) multipliers[i] = -1;
        else multipliers[i] = 1;
    }
}
/*!
 * \brief inputWorker::restoreKey restores a saved key
 * \param _key saved control
 * \param _value saved key to previous control
 */
void inputWorker::restoreKey(inputs::CONTROLKEYS _key, int _value)
{
    /*!
     * Slot restoring key information
     */
    keys[_key] = _value;
}
/*!
 * \brief inputWorker::keyChangedInt changes a key assignation
 * \param _key control key
 * \param _value key values
 */
void inputWorker::keyChangedInt(inputs::CONTROLKEYS _key, int _value)
{
    /*!
     * Emit setKeyValue signal if the widget is completely loaded
     */
    if(loaded) emit setKeyValue(_key,_value);
}
void inputWorker::toggledJoystick(bool _value)
{
    /*!
     * Toggle Joystic enabled value
     */
    this->joyenabled = _value;
    if(loaded) emit setJoystick(this->joystickIndex,this->joyenabled);
}
void inputWorker::restored(){
    /*!
     * Setup restoration completed
     */
    this->loaded = true;
}
void inputWorker::receiveKeyEvent(QKeyEvent * event)
{
    this->manageKey(event->key());
}
