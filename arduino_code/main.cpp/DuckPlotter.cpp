
#include "DuckPlotter.h"

#define DEFAULT_STEP_TIME 1000 //microseconds

/**
  Init function. Cannot be a contructor beacuse of the Servo class
*/
void DuckPlotter::init()
{
  driver = ShieldDriver();
  discards[X] = 0;
  discards[Y] = 0;

  setStepTime(DEFAULT_STEP_TIME,DEFAULT_STEP_TIME);

  penMotor.attach(SERVO_PIN);
  movePen(false);
}

/**
  Set delay in microseconds between each step of both the motorX and motorY
*/
//TO TEST
void DuckPlotter::setStepTime(unsigned long step_timeX, unsigned long step_timeY)
{
  driver.motorX.setTime(step_timeX);
  driver.motorY.setTime(step_timeY);
}
/**
   Check if a point is near enough to a target one for a linear movement
*/

bool DuckPlotter::nearLinear(double point, double target)
{
  return (target - incrX < point && point < target + incrX);
}

double DuckPlotter::fixRadians(double rad)
{
  if (rad < 0)
    return rad + (PI * 2);

  if (rad > 2 * PI)
    return rad - (PI * 2);

  return rad;
}

/**
   Check if a point is near enough to a target one for an arc movement
*/
bool DuckPlotter::nearArc(double t, double finalT, double incr)
{
  incr = abs(incr);

  t = fixRadians(t);
  finalT = fixRadians(finalT);

  return (finalT - incr < t && t < finalT + incr);
}

/**
  Calculate the radians of a point in a circle
*/
double DuckPlotter::calculateRadOfPoint(Position pos, Position center)
{
  double dx = (pos.x - center.x);
  double dy = (pos.y - center.y);

  double t = atan(dy / dx);

  //choose the right quadrant
  if (dx < 0)
  {
    t += PI;
  }

  return t;
}

/**
   Reset the position of all the stepper motors
*/
void DuckPlotter::reset()
{
  driver.reset();
}

/**
   Convert a millimenter value to a stepper one in base of the axis
*/
double DuckPlotter::millimetersToSteps(double millimeters, int axis)
{
  double scale = scaleXfactor; //X scale factor
  if (axis == Y)
    scale = scaleYfactor; //Y scale factor

  return millimeters * scale;
}

/**
   Move the stepper motor of the axis by some millimeter value
*/
void DuckPlotter::moveMotor(double mill, int axis)
{
  double stepsdouble = millimetersToSteps(mill, axis);
  int steps = floor(stepsdouble);
  double discard = stepsdouble - ((double) steps);

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
Position DuckPlotter::moveLinear(Position from, Position to)
{
  //find the equation of the line (y = mx + q)
  double m = (to.y - from.y) / (to.x - from.x);
  double q = from.y - (m * from.x);

  Position pos;
  pos.x = from.x;
  pos.y = from.y;

  while (!nearLinear(pos.x, to.x) || !nearLinear(pos.y, to.y))
  {
    Position next;
    next.x = pos.x;
    next.y = pos.y;

    if (!nearLinear(pos.x, to.x)) // increment x
    {
      int sign = (pos.x < to.x) ? 1 : -1;
      next.x += incrX * sign;
      next.y = m * next.x + q;
    }
    else // increment y
    {
      int sign = (pos.y < to.y) ? 1 : -1;
      next.y += incrY * sign;
    }

    //move motors
    if (canMove(next.x, X))
      moveMotor(next.x - pos.x, X);
    if (canMove(next.y, Y))
      moveMotor(next.y - pos.y, Y);

    pos.x = next.x;
    pos.y = next.y;
  }

  return pos;
}

/**
   Make an arc movement
*/
Position DuckPlotter::moveArc(Position from, Position to, double radius, Position center, bool clockwise)
{
  // x = radius * cos(t) + centerX AND y = radius * sin(t) + centerY
  // t = atan((y - centerY) / (x - centerX))

  Position pos;
  pos.x = from.x;
  pos.y = from.y;
  double incr = incrT / radius; //millimeters of the circonference converted to radians
  incr = (clockwise ? -incr : incr);

  double t = calculateRadOfPoint(pos, center);
  double finalT = calculateRadOfPoint(to, center);

  while (!nearArc(t, finalT, incr))
  {
    Position next;
    next.x = radius * cos(t) + center.x;
    next.y = radius * sin(t) + center.y;

    //move motors
    if (canMove(next.x, X))
      moveMotor(next.x - pos.x, X);
    if (canMove(next.y, Y))
      moveMotor(next.y - pos.y, Y);

    t += incr;
    pos.x = next.x;
    pos.y = next.y;
  }

  return pos;
}

/**
   Make an arc movement
*/
Position DuckPlotter::moveArc(Position from, Position to, Position offset, bool clockwise)
{
  //find the radius
  double radius = sqrt(pow(offset.x, 2) + pow(offset.y, 2));
  Position center;
  center.x = from.x + offset.x;
  center.y = from.y + offset.y;

  return moveArc(from, to, radius, center, clockwise);
}

/**
   Make an arc movement
*/
Position DuckPlotter::moveArc(Position from, Position to, double radius, bool clockwise)
{
  //find the centerX and centerY
  double radsq = radius * radius;
  double q = sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2));

  Position centerA;
  Position centerB;

  double x3 = (from.x + to.x) / 2;
  double x3b = sqrt(radsq - pow(q / 2.0, 2)) * ((from.y - to.y) / q);
  centerA.x = x3 + x3b;
  centerB.x = x3 - x3b;

  double y3 = (from.y + to.y) / 2;
  double y3b = sqrt(radsq - pow(q / 2.0, 2)) * ((to.x - from.x) / q);
  centerA.y = y3 + y3b;
  centerB.y = y3 - y3b;

  double tStartA = calculateRadOfPoint(from, centerA);
  double tEndA = calculateRadOfPoint(to, centerA);

  double tStartB = calculateRadOfPoint(from, centerB);
  double tEndB = calculateRadOfPoint(to, centerB);

  Position center;
  //choose the right center
  if((clockwise && radius > 0) || (!clockwise && radius < 0)) //shortest arc
  {
    center = (tStartA > tEndA) ? centerA : centerB;
  }
  else
  {
    center = (tStartA < tEndA) ? centerA : centerB;
  }

  return moveArc(from, to, radius, center, clockwise);
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

/**
  Check if it is possible to perform a movement without exceeding the limits
*/
bool DuckPlotter::canMove(double position, int axi)
{
  if (axi == X)
    return position >= 0 && position <= millimitersX;
  else if (axi == Y)
    return position >= 0 && position <= millimitersY;

  return false;
}
