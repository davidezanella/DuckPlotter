#include "ShieldDriver.h"

ShieldDriver::ShieldDriver()
{

}

/**
   Reset all the positions of the motors
*/
void ShieldDriver::reset()
{
  motorX.reset();
  motorY.reset();
  motorZ.reset();
  motorQ.reset();
  motorE.reset();
}
