
#ifndef _DUCKPLOTTER_h
#define _DUCKPLOTTER_h

#include <Servo.h>
#include <math.h>

#include "ShieldDriver.h"

#define X 0
#define Y 1

#define SERVO_PIN 32

#define penUP 10
#define penDOWN 95

#define millimitersX 210.0
#define millimitersY 297.0

#define maxStepsX 12436 //maximum number of steps on the X axis
#define maxStepsY 21766 //maximum number of steps on the Y axis

#define numMinSteps 5 //minimum number of steps done


class DuckPlotter
{
  public:
    //Constructor
    DuckPlotter();

    Servo penMotor;
    ShieldDriver driver = ShieldDriver();


    //Reset motor positions
    void reset();

    float millimetersToSteps(float millimeters, int axis);
    void moveMotor(float mill, int axis);
    
    void moveLinear(float fromX, float fromY, float toX, float toY);
    
    void moveArc(float fromX, float fromY, float toX, float toY, float radius, float centerX, float centerY, bool clockwise);
    void moveArc(float fromX, float fromY, float toX, float toY, float offsetX, float offsetY, bool clockwise);
    void moveArc(float fromX, float fromY, float toX, float toY, float radius, bool clockwise);
    
    void movePen(bool down);

  private:
    const float scaleXfactor = maxStepsX / millimitersX;
    const float scaleYfactor = maxStepsY / millimitersY;
    
    const float incrX = numMinSteps / scaleXfactor;
    const float incrY = numMinSteps / scaleYfactor;
    
    float discards[2] = {0, 0};

    bool near(float point, float target);
    bool canMove(float position, int axi);
};

#endif
