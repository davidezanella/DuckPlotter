
#include "DuckPlotter.h"

/**
  Init function. Cannot be a contructor beacuse of the Servo class
*/
void DuckPlotter::init()
{
  driver = ShieldDriver();
  discards[X] = 0;
  discards[Y] = 0;

  penMotor.attach(SERVO_PIN);
  movePen(false);
}

/**
   Check if a number is near enough to a target one
*/
bool DuckPlotter::near(double point, double target, bool linear)
{
  const double sens = incrX * (linear ? 1 : 28);
  return (target - sens < point && point < target + sens);
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

  while (!near(pos.x, to.x, true) || !near(pos.y, to.y, true))
  {
    Position next;
    next.x = pos.x;
    next.y = pos.y;

    if (!near(pos.x, to.x, true)) // increment x
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
  double incr = 0.05 / (2 * PI * radius); //millimeters of the circonference
  incr = (clockwise ? -incr : incr);

  double dx = (pos.x - center.x);
  double dy = (pos.y - center.y);

  double t = atan(dy / dx);

  //chose the right quadrant
  if (dx < 0)
  {
    t += PI;
  }

  while (!near(pos.x, to.x, false) || !near(pos.y, to.y, false))
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
  double q = sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
  double x3 = (from.x + to.x) / 2;
  Position center;
  center.x = x3 + sqrt(radsq - pow(q / 2, 2) * ((from.y - to.y) / q));

  double y3 = (from.y + to.y) / 2;
  center.y = y3 + sqrt(radsq - pow(q / 2, 2) * ((from.x - to.x) / q));

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
