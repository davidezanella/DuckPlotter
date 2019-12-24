#include "ShieldDriver.h"

ShieldDriver::ShieldDriver()
{

}

/**
   Reset all the positions of the motors
   in the duckplotter project only motorX and motorY have been used
*/
void ShieldDriver::reset()
{
  motorX.reset();
  motorY.reset();
  //motorZ.reset();
  //motorQ.reset();
  //motorE.reset();
}
