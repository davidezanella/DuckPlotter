#include "ShieldStepper.h"

/**
   Constructor
*/
ShieldStepper::ShieldStepper(int _stepPin, int _dirPin, int _enablePin, int _stopPin, int _invertDir)
{
  stepPin = _stepPin;
  dirPin = _dirPin;
  enablePin = _enablePin;
  stopPin = _stopPin;
  invertDir = _invertDir;

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  digitalWrite(enablePin, LOW);

  pinMode(stopPin, INPUT_PULLUP);
}

/**
   Set the direction of the stepper motor
*/
void ShieldStepper::setDirection(short _dir)
{
  if (invertDir == 0) //invert positions
  {
    _dir = !_dir;
  }

  digitalWrite(dirPin, _dir);
}

/**
   Make one step
*/
void ShieldStepper::step()
{
  //make one step setting high and low the state
  digitalWrite(stepPin, HIGH);
  //delay(1);
  delayMicroseconds(step_time);
  digitalWrite(stepPin, LOW);
}

/**
   Make a number of steps.
   If the number is negative the direction will be inverted
*/
void ShieldStepper::makeSteps(int _nSteps)
{
  int dir = (_nSteps > 0) ? 1 : 0;
  setDirection(dir);

  for (int i = 0; i < abs(_nSteps); i++)
  {
    //make one step per time
    step();
  }
}

//------------------------------------------
//to test
void ShieldStepper::setTime(unsigned long _step_time)
{
  step_time = _step_time;
}

//------------------------------------------

/**
   Reset the position of the motor
*/
void ShieldStepper::reset()
{
  setDirection(0);
  while (!limitReached())
  {
    step();
  }
}

/**
   Check if the motor has reached its limit
*/
bool ShieldStepper::limitReached()
{
  return digitalRead(stopPin);
}
