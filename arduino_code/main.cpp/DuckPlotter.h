
#ifndef _DUCKPLOTTER_h
#define _DUCKPLOTTER_h

#include <Servo.h>
#include <math.h>
#include <Arduino.h>

#include "ShieldDriver.h"

#define X 0
#define Y 1

#define SERVO_PIN 32

#define penUP 30
#define penDOWN 90

#define millimitersX 210.0
#define millimitersY 297.0

#define maxStepsX 12436 //maximum number of steps on the X axis
#define maxStepsY (maxStepsX / millimitersX * millimitersY) //21766 //maximum number of steps on the Y axis

#define numMinSteps 1 //minimum number of steps done

#define scaleXfactor (maxStepsX / millimitersX)
#define scaleYfactor (maxStepsY / millimitersY)
    
#define incrX (numMinSteps / scaleXfactor)
#define incrY (numMinSteps / scaleYfactor)

#define incrT 0.008 //millimenets of the circumference dove by each step of an arc movement


struct Position {
   double x;
   double y;
};

class DuckPlotter
{
  public:
    void init();

    //Reset motor positions
    void reset();

    double millimetersToSteps(double millimeters, int axis);
    void moveMotor(double mill, int axis);
    
    Position moveLinear(Position from, Position to);
    
    Position moveArc(Position from, Position to, double radius, Position center, bool clockwise);
    Position moveArc(Position from, Position to, Position offset, bool clockwise);
    Position moveArc(Position from, Position to, double radius, bool clockwise);
    
    void movePen(bool down);

  private:    
    Servo penMotor;    
    ShieldDriver driver;
    
    double discards[2];

    bool nearLinear(double point, double target);
    bool nearArc(double t, double finalT, double incr);
    bool canMove(double position, int axi);
    double calculateRadOfPoint(Position pos, Position center);
    double fixRadians(double rad);
};

#endif
