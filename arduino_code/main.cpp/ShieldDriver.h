
#ifndef _SHIELDRIVER_h
#define _SHIELDDRIVER_h

#include "ShieldStepper.h"
#include <Arduino.h>

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define Q_STEP_PIN         36
#define Q_DIR_PIN          34
#define Q_ENABLE_PIN       30

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING


//define all the stop button
#define X_STOP_PIN 3
#define X_INVERT_DIR 0

#define Y_STOP_PIN 14
#define Y_INVERT_DIR 1

#define Z_STOP_PIN 18
#define Z_INVERT_DIR 0

#define E_STOP_PIN 1 //random
#define E_INVERT_DIR 0

#define Q_STOP_PIN 1 //random
#define Q_INVERT_DIR 0


class ShieldDriver
{
  public:

    //Constructor
    ShieldDriver();

    //Reset motor positions
    void reset();

    //declare all the stepper motors
    ShieldStepper motorX = ShieldStepper(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_STOP_PIN, X_INVERT_DIR);
    ShieldStepper motorY = ShieldStepper(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_STOP_PIN, Y_INVERT_DIR);
    //in our project only motorX and motorY have been used, uncomment to enable:
    //ShieldStepper motorZ = ShieldStepper(Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_STOP_PIN, Z_INVERT_DIR);
    //ShieldStepper motorE = ShieldStepper(E_STEP_PIN, E_DIR_PIN, E_ENABLE_PIN, E_STOP_PIN, E_INVERT_DIR);
    //ShieldStepper motorQ = ShieldStepper(Q_STEP_PIN, Q_DIR_PIN, Q_ENABLE_PIN, Q_STOP_PIN, Q_INVERT_DIR);

  private:

};

#endif
