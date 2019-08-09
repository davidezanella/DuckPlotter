
#include "DuckPlotter.h"

/**
  Constructor
*/
DuckPlotter::DuckPlotter()
{
  penMotor.attach(SERVO_PIN);
  movePen(false);
}

/**
   Check if a number is near enough to a target one
*/
bool DuckPlotter::near(float point, float target)
{
  const float sens = 0.3;
  return (target - sens <= point && point <= target + sens);
}

/**
   Reset the position of all the stepper motors
*/
void DuckPlotter::reset()
{
  driver.reset();
}

/**
   Convert a millimenter value to a stepper one in base of the axix
*/
float DuckPlotter::millimetersToSteps(float millimeters, int axis)
{
  float scale = scaleXfactor; //X scale factor
  if (axis == Y)
    scale = scaleYfactor; //Y scale factor

  return millimeters * scale;
}

/**
   Move the stepper motor of the axis by some millimeter value
*/
void DuckPlotter::moveMotor(float mill, int axis)
{
  float stepsFloat = millimetersToSteps(mill, axis);
  int steps = floor(stepsFloat);
  float discard = stepsFloat - ((float) steps);

  discards[axis] += discard;
  if (discards[axis] >= 1)
  {
    steps += 1;
    discards[axis] -= 1;
  }

  if (axis == X)
  {
    driver.motorX.makeSteps(steps);
  }
  else if (axis == Y)
  {
    driver.motorY.makeSteps(steps);
  }
}

/**
   Make a linear movement
*/
void DuckPlotter::moveLinear(float fromX, float fromY, float toX, float toY)
{
  //find the equation of the line (y = mx + q)
  float m = (toY - fromY) / (toX - fromX);
  float q = fromY - (m * fromX);

  float x = fromX;
  float y = fromY;

  while (!near(x, toX) || !near(y, toY))
  {
    float nextX = x;
    float nextY = y;

    if (x != toX) // increment x
    {
      int sign = (x < toX) ? 1 : -1;
      nextX += incrX * sign;
      nextY = m * nextX + q;
    }
    else // increment y
    {
      int sign = (y < toY) ? 1 : -1;
      nextY += incrY * sign;
    }

    //move motors
    moveMotor(nextX - x, X);
    moveMotor(nextY - y, Y);

    x = nextX;
    y = nextY;
  }
}

/**
   Make an arc movement
*/
void DuckPlotter::moveArc(float fromX, float fromY, float toX, float toY, float radius, float centerX, float centerY, bool clockwise)
{
  // x = radius * cos(t) + centerX AND y = radius * sin(t) + centerY
  // t = atan((y - centerY) / (x - centerX))

  float x = fromX;
  float y = fromY;
  float incr = 0.01;
  incr = (clockwise ? -incr : incr);

  float dx = (x - centerX);
  float dy = (y - centerY);

  float t = atan(dy / dx);

  //chose the right quadrant
  if (dx < 0)
  {
    t += PI;
  }

  while (!near(x, toX) || !near(y, toY))
  {
    float nextX = radius * cos(t) + centerX;
    float nextY = radius * sin(t) + centerY;

    //move motors
    moveMotor(nextX - x, X);
    moveMotor(nextY - y, Y);

    t += incr;
    x = nextX;
    y = nextY;
  }
}

/**
   Make an arc movement
*/
void DuckPlotter::moveArc(float fromX, float fromY, float toX, float toY, float offsetX, float offsetY, bool clockwise)
{
  //find the radius
  float radius = sqrt(pow(offsetX, 2) + pow(offsetY, 2));
  float centerX = fromX + offsetX;
  float centerY = fromY + offsetY;

  moveArc(fromX, fromY, toX, toY, radius, centerX, centerY, clockwise);
}

/**
   Make an arc movement
*/
void DuckPlotter::moveArc(float fromX, float fromY, float toX, float toY, float radius, bool clockwise)
{
  //find the centerX and centerY
  float radsq = radius * radius;
  float q = sqrt(pow(fromX - toX, 2) + pow(fromY - toY, 2));
  float x3 = (fromX + toX) / 2;
  float centerX = x3 + sqrt(radsq - pow(q / 2, 2) * ((fromY - toY) / q));

  float y3 = (fromY + toY) / 2;
  float centerY = y3 + sqrt(radsq - pow(q / 2, 2) * ((fromX - toX) / q));

  moveArc(fromX, fromY, toX, toY, radius, centerX, centerY, clockwise);
}

/**
   Put up or down the pen
*/
void DuckPlotter::movePen(bool down)
{
  if (down)
    penMotor.write(penDOWN);
  else
    penMotor.write(penUP);
}
