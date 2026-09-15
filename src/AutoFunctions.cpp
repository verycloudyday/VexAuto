#include <math.h>
#include "vex.h"
#include "movement.hpp"
#include "helper_functions.hpp"

void LiftAuto (int deg, int speed, bool braking)
{
  liftL.setMaxTorque(100,percent);
  liftR.setMaxTorque(100,percent);
  liftL.spinToPosition(deg, degrees, speed, velocityUnits::pct);
  liftR.spinToPosition(deg, degrees, speed, velocityUnits::pct);
  if(braking){
    liftL.setStopping(brake);
    liftR.setStopping(brake);
    wait(120,msec);
  }
  else{
    liftL.setStopping(brake);
    liftR.setStopping(brake);
  }
}

void ChainbarAuto (int deg, int speed, bool braking)
{
  chainbar.setMaxTorque(100,percent);
  chainbar.spinToPosition(deg, degrees, speed, velocityUnits::pct);
  if(braking){
    chainbar.setStopping(brake);
    wait(120,msec);
  }
  else{
    chainbar.setStopping(brake);
  }
}

void ClawAuto (bool state)
{
  Claw.set(state);
  wait(120,msec);
}

