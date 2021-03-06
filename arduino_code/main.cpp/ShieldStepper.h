
#ifndef _SHIELDSTEPPER_h
#define _SHIELDSTEPPER_h

#include <Arduino.h>

class ShieldStepper
{
  public:
    ShieldStepper(int _stepPin, int _dirPin, int _enablePin, int _stopPin, int _invertDir);

    void setDirection(short _dir);

    void makeSteps(int _nSteps);

    void setTime(unsigned long _step_time);

    void reset();

  private:
    int stepPin;
    int dirPin;
    int enablePin;
    int stopPin;
    int invertDir;
    unsigned long step_time;

    void step();

    bool limitReached();

};

#endif
