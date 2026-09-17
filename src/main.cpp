/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// #include "STDLib.cpp"
#include "vex.h"

#include "screen_gui.hpp"
#include "movement.hpp"
#include "routes/routes.hpp"
#include <iostream>

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

bool SP;
bool EXIT;
void pre_auton(void)
{
  // Claw.set(true);
  EXIT = false;
  Tilt.set(true);
  Clamp.set(true);
  PX = 0;
  JX = 0;
  AutoSelectorVal = 0;
  SP = false;
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Gyro.calibrate();

  // Ensure Robot Launch Position is set before auto proceeds, once plugged into field control,
  // start program and do not temper bot under all circumstances

  // 1. IF ANY ADJUSTMENT IS NEEDED, QUIT PROGRAM, THEN ADJUST, RESTART PROGRAM AFTER ADJUSTMENTS COMPLETED
  // 2. DO NOT START PROGRAM BEFORE PLUGGING IN FIELD CONTROL, THIS MAY DISABLE AUTO
  // 3. ONLY SIGNAL JUDGES TO BEGIN MATCH AFTER THE ZEROING PROMPT ON SCREEN HAS CLEARED

  // Print precautionary message
  Brain.Screen.drawRectangle(0, 0, 500, 500);

  Brain.Screen.setFont(monoXL);
  Brain.Screen.setPenColor("#39FF14");
  Brain.Screen.setCursor(2, 10);
  Brain.Screen.print("FLIR TIMEOUT");

  waitUntil(!Gyro.isCalibrating());

  Zeroing(true, true);
  DisplayAutoSelector();
  DisplayWords();

  // task AutoSelTask=task(ScreenSelMain);
  while (!EXIT)
  {
    if (Brain.Screen.xPosition() < 100)
    {
      if (Brain.Screen.yPosition() < 75 && Brain.Screen.yPosition() > 25)
        AutoSelectorVal = 1;
      else if (Brain.Screen.yPosition() < 150 && Brain.Screen.yPosition() > 100)
        AutoSelectorVal = 3;
      else if (Brain.Screen.yPosition() < 225 && Brain.Screen.yPosition() > 175)
        AutoSelectorVal = 5;
    }
    else if (Brain.Screen.xPosition() > 375)
    {
      if (Brain.Screen.yPosition() < 75 && Brain.Screen.yPosition() > 25)
        AutoSelectorVal = 2;
      else if (Brain.Screen.yPosition() < 150 && Brain.Screen.yPosition() > 100)
        AutoSelectorVal = 4;
      else if (Brain.Screen.yPosition() < 225 && Brain.Screen.yPosition() > 175)
        AutoSelectorVal = 6;
    }

    if (Brain.Screen.xPosition() > 187 && Brain.Screen.xPosition() < 287)
    {
      if (Brain.Screen.yPosition() < 55 && Brain.Screen.yPosition() > 5)
        EXIT = true;
      else if (Brain.Screen.yPosition() > 125 && Brain.Screen.yPosition() < 225)
        AutoSelectorVal = 7;
    }

    if (Brain.Screen.pressing() && !SP)
      UpdateDynamic();

    SP = Brain.Screen.pressing();
    wait(20, msec);
  }

  Brain.Screen.clearScreen();
  if (AutoSelectorVal == 1)
  {
    Brain.Screen.setFillColor(black);
    Brain.Screen.setFont(monoXL);
    Brain.Screen.setPenColor("#39FF14");
    Brain.Screen.setCursor(3, 10);
    Brain.Screen.print("GOAL SIDE");
    Brain.Screen.setCursor(4, 10);
    Brain.Screen.print("6 BALL");
    Brain.Screen.setFont(monoM);
    Brain.Screen.setFillColor("#39FF14");
  }

  if (AutoSelectorVal == 2)
  {
    Brain.Screen.setFillColor(black);

    Brain.Screen.setFont(monoXL);
    Brain.Screen.setPenColor("#39FF14");
    Brain.Screen.setCursor(3, 10);
    Brain.Screen.print("MATCHLOAD");
    Brain.Screen.setCursor(4, 10);
    Brain.Screen.print("StealAWP");
    Brain.Screen.setFont(monoM);
    Brain.Screen.setFillColor("#39FF14");
  }

  if (AutoSelectorVal == 3)
  {

    Brain.Screen.setFillColor(black);

    Brain.Screen.setFont(monoXL);
    Brain.Screen.setPenColor("#39FF14");
    Brain.Screen.setCursor(3, 10);
    Brain.Screen.print("GOAL SIDE");
    Brain.Screen.setCursor(4, 10);
    Brain.Screen.print("5 BALL");
    Brain.Screen.setFont(monoM);
    Brain.Screen.setFillColor("#39FF14");
  }

  if (AutoSelectorVal == 4)
  {

    Brain.Screen.setFillColor(black);

    Brain.Screen.setFont(monoXL);
    Brain.Screen.setPenColor("#39FF14");
    Brain.Screen.setCursor(3, 10);
    Brain.Screen.print("MATCHLOAD");
    Brain.Screen.setCursor(4, 10);
    Brain.Screen.print("Elim-Steal");
    Brain.Screen.setFont(monoM);
    Brain.Screen.setFillColor("#39FF14");
  }

  if (AutoSelectorVal == 5)
  {

    Brain.Screen.setFillColor(black);
    Brain.Screen.setFont(monoXL);
    Brain.Screen.setPenColor("#39FF14");
    Brain.Screen.setCursor(3, 10);
    Brain.Screen.print("GOAL SIDE");
    Brain.Screen.setCursor(4, 10);
    Brain.Screen.print("GS-AWP");
    Brain.Screen.setFont(monoM);
    Brain.Screen.setFillColor("#39FF14");
  }

  if (AutoSelectorVal == 6)
  {

    Brain.Screen.setFillColor(black);
    Brain.Screen.setFont(monoXL);
    Brain.Screen.setPenColor("#39FF14");
    Brain.Screen.setCursor(3, 10);
    Brain.Screen.print("MATCHLOAD");
    Brain.Screen.setCursor(4, 10);
    Brain.Screen.print("ONLY AWP");
    Brain.Screen.setFont(monoM);
    Brain.Screen.setFillColor("#39FF14");
  }

  if (AutoSelectorVal == 7)
  {

    Brain.Screen.setFillColor(black);
    Brain.Screen.setFont(monoXL);
    Brain.Screen.setPenColor("#39FF14");
    Brain.Screen.setCursor(3, 10);
    Brain.Screen.print("SKILLS");
    Brain.Screen.setCursor(4, 10);
    Brain.Screen.print("SKILLS");
    Brain.Screen.setFont(monoM);
    Brain.Screen.setFillColor("#39FF14");
  }

  Brain.Screen.setFillColor(black);
  Brain.Screen.setFont(monoXL);
  Brain.Screen.setPenColor("#39FF14");
  Brain.Screen.setCursor(5, 10);
  Brain.Screen.print("AUTO CONFIRMED");
  Brain.Screen.setPenColor("#ff0000");
  Brain.Screen.setCursor(10, 10);
  Brain.Screen.print("CODE IS STRAIGHT ASS, DELETE IMMEDIATELY");

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void)
{

  // PID Straight and turn arguments:
  //  MoveEncoderPID(TestPara, motor speed, encoder travel distance (inches), time to full speed(sec), relative heading(to starting position), braking?)
  //  TurnMaxTimePID(TestPara, Desired Heading -180 to 180, time out to calculate turn, Braking?)
  //  MoveTimePID(TestPara, motor speed, time traveled (sec), time to full speed, heading, false);

  // Do not change the below
  if (!EXIT)
  {
    EXIT = true;
    AutoSelectorVal = 1;
  }

  Zeroing(true, true);

  // can start editing if nessary
  // Put Auto route function into if statements to use autoselector
  if (AutoSelectorVal == 1) // double toggle
  {

    test();
  }

  if (AutoSelectorVal == 2) // awp mid steal
  {
    test2();
  }

  if (AutoSelectorVal == 3) // 5 Ball rush
  {
  }

  if (AutoSelectorVal == 4) // Elim-Steal
  {
  }

  if (AutoSelectorVal == 5) // empty
  {
    // test();
  }

  if (AutoSelectorVal == 6) // AWP only
  {
  }

  if (AutoSelectorVal == 7) // temporary prog skills
  {
  }
  // MoveTimePID(TestPara, -100, 0.5,0.1,-40,true);//score 2nd triball
  //(PID Parameters, motor speed -100 - 100, time for travel 0 - inf, time to accelerate to full speed, Absolute Heading, Braking?)

  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................

  CStop();
}
int RV;
int LV;

// int IntakeTask(void)
// {
//   while (true)
//   {
//     AntiTipTask(10.0, 0.3, 9.0, 6.0);
//   }
//   return 0;
// }

double lastPitch = 0;
bool tipActive = false;
bool antiTipEnabled = false;
bool wasLeftPressed = false;
int tipDirection = 0;   // +1 = tipped forward, -1 = tipped backward, 0 = not tipping

int DriveTask(void)
{
  const double triggerAngleA = 10.0;   // forward trigger
  const double triggerAngleB = -10.0;  // backward trigger
  const double releaseAngleA = 7.0;    // recovers once pitch drops below this (forward tip)
  const double releaseAngleB = -7.0;   // recovers once pitch rises above this (backward tip) — change later
  const double kP = 3.5;
  const double kD = 0.5;

  while (true)
  {
    double pitch = Gyro.roll(rotationUnits::deg);
    double pitchRate = (pitch - lastPitch) / 0.01;
    lastPitch = pitch;

    bool isLeftPressed = Controller1.ButtonLeft.pressing();
    if (isLeftPressed && !wasLeftPressed)
    {
      antiTipEnabled = !antiTipEnabled;
      tipActive = false;
      tipDirection = 0;
      Controller1.rumble(".");
    }
    wasLeftPressed = isLeftPressed;

    if (!tipActive && antiTipEnabled)
    {
      if (pitch > triggerAngleA)
      {
        tipActive = true;
        tipDirection = 1;
        Controller1.rumble(".");
      }
      else if (pitch < triggerAngleB)
      {
        tipActive = true;
        tipDirection = -1;
        Controller1.rumble("-");
      }
    }
    else if (tipActive)
    {
      if (tipDirection == 1 && pitch < releaseAngleA)
      {
        tipActive = false;
        tipDirection = 0;
        Controller1.rumble("..");
      }
      else if (tipDirection == -1 && pitch > releaseAngleB)
      {
        tipActive = false;
        tipDirection = 0;
        Controller1.rumble("..");
      }
    }

    if (!tipActive)
    {
      RV = -Controller1.Axis3.position(percent) + Controller1.Axis1.position(percent);
      LV = -Controller1.Axis3.position(percent) - Controller1.Axis1.position(percent);
      if (RV > 100) RV = 100;
      if (RV < -100) RV = -100;
      if (LV > 100) LV = 100;
      if (LV < -100) LV = -100;
      Move(LV, RV);
    }
    else
    {
      double correction = (pitch * kP) + (pitchRate * kD);
      if (correction > 100) correction = 100;
      if (correction < -100) correction = -100;
      Move(correction, correction);
    }

    wait(10, msec);
  }
  return 0;
}

int V;

// int ATask(void)
// {
//   bool matchloadActive = false;
//   double pow;
//   double intakeDistance = 0;
//   while (true)
//   {
//     if (Controller1.ButtonUp.pressing() && !matchloadActive)
//     {
//       matchloadActive = true;  // latch — only starts once per press
//     }

//     if (matchloadActive)
//     {
//       intakeDistance = intakeSensor.objectDistance(distanceUnits::in);
//       Roller.spin(forward, 100, velocityUnits::pct);  // spin one direction until object is captured

//       // if (intakeDistance <= 10.0)
//       // {
//       //   Roller.spin(forward, 0, velocityUnits::pct);  // stop the roller — object captured
//       //   IntakeBar.set(true);
//       //   wait(250, msec);
//       //   chainbar.spinToPosition(0, rotationUnits::deg, 100, velocityUnits::pct);
//       //   wait(250, msec);
//       //   Claw.set(true);
//       //   wait(250, msec);
//       //   chainbar.spinToPosition(812, rotationUnits::deg, 100, velocityUnits::pct);

//       //   matchloadActive = false;  // done — hand control back to the driver
//       // }
//     }
//     else
//     {
//       pow = (Controller1.ButtonR2.pressing() - Controller1.ButtonR1.pressing()) * 100;
//       Roller.spin(forward, -pow, velocityUnits::pct);
//     }

//     wait(20, msec);
//   }
//   return 0;
// }

int ButtonPressingX, XTaskActiv;
int ButtonPressingY, YTaskActiv;

// int PTask(void)
// {
//   while (true)
//   {
//     // Toggles Tilt
//     if (XTaskActiv == 0 && Controller1.ButtonX.pressing() && ButtonPressingX == 0)
//     {
//       ButtonPressingX = 1;
//       XTaskActiv = 1;
//       Tilt.set(true);
//     }

//     else if (!Controller1.ButtonX.pressing())
//       ButtonPressingX = 0;

//     else if (XTaskActiv == 1 && Controller1.ButtonX.pressing() && ButtonPressingX == 0)
//     {
//       ButtonPressingX = 1;
//       XTaskActiv = 0;
//       Tilt.set(false);
//     }
//     //----------------------
//     // Toggles Clamp
//     if (YTaskActiv == 0 && Controller1.ButtonY.pressing() && ButtonPressingY == 0)
//     {
//       ButtonPressingY = 1;
//       YTaskActiv = 1;
//       Clamp.set(true);
//     }

//     else if (!Controller1.ButtonY.pressing())
//       ButtonPressingY = 0;

//     else if (YTaskActiv == 1 && Controller1.ButtonY.pressing() && ButtonPressingY == 0)
//     {
//       ButtonPressingY = 1;
//       YTaskActiv = 0;
//       Clamp.set(false);
//     }
//   }
//   return 0;
// }

bool barUp = false;
int IntakeTask(void)
{
  while (true)
  {
    if (Controller1.ButtonR2.pressing())
    {
      RunRoller(-100);
      if (barUp){
        if (intakeSensor.objectDistance(distanceUnits::mm) < 100.0) {
          // RunRoller(100);
          // wait(100, msec);
          // IntakeBar.set(false);
          // wait(100, msec);
          // chainbar.spinToPosition(0, rotationUnits::deg, 100, velocityUnits::pct);
          // wait(100, msec);
          // duoLift(-100, true);
          // wait(100, msec);
          // Claw.set(true);
          // wait(100, msec);
          // chainbar.spinToPosition(812, rotationUnits::deg, 100, velocityUnits::pct);
        }
      }
    }
    else if (Controller1.ButtonR1.pressing())
    {
      RunRoller(100);
    }
    else
    {
      RunRoller(0); // neither held: stop
    }
    wait(20, msec);
  }
  return 0;
}

int LiftTask(void)
{
  liftL.setMaxTorque(100, percentUnits::pct);
  liftR.setMaxTorque(100, percentUnits::pct);

  while (true)
  {
    if (Controller1.ButtonL2.pressing())
    {
      liftL.spin(reverse, -100, percentUnits::pct);
      liftR.spin(forward, -100, percentUnits::pct); // L2 held: spin forward
    }
    else if (Controller1.ButtonL1.pressing())
    {
      liftL.spin(forward, -100, percentUnits::pct); // L1 held: spin backward
      liftR.spin(reverse, -100, percentUnits::pct); // L1 held: spin backward
    }
    else
    {
      liftL.setBrake(hold);
      liftR.setBrake(hold);
      liftL.stop();
      liftR.stop();
    }
  }
  return 0;
}

bool clawState = false;

void exeClaw(void)
{
  clawState = !clawState;
  Claw.set(clawState);
}

int ClawTask(void)
{
  Controller1.ButtonDown.pressed(exeClaw);  // register once — VEX calls exeClaw for you on each press
  return 0;
}




int IntakeBarTask(void)
{
  while (true)
  {
    if (Controller1.ButtonRight.pressing())
    {
      IntakeBar.set(true);
      barUp = true;
    }
    else
    {
      {
        IntakeBar.set(false);
        barUp = false;
      }
    }
  }
}

void moveChainbarToLow(){
  chainbar.spinToPosition(100, rotationUnits::deg, 850, velocityUnits::pct, true);{
  }
}

void moveChainbarToHigh(){
  chainbar.spinToPosition(100, rotationUnits::deg, 450, velocityUnits::pct, true);{
  }
}

void moveChainbarToZero(){
  chainbar.spinToPosition(100, rotationUnits::deg, 0, velocityUnits::pct, true);{
  }
}

int ChainbarTask(void)
{
  chainbar.setBrake(hold);  


  while (true)
  {
  Controller1.ButtonB.pressed(moveChainbarToLow);
  Controller1.ButtonA.pressed(moveChainbarToHigh);
  Controller1.ButtonX.pressed(moveChainbarToZero);
   
    wait(20, msec);
  }
  return 0;
}

//   if (vexDistanceDistanceGet(intakeSensor) < 3.0) {
//     isLoaded = true;
//     RunRoller(50);
//     wait 500, msec;
//     IntakeBar.set(true);
//     wait 500, msec;
//     chainbar.spinToPosition(0, rotationUnits::deg, 100, velocityUnits::pct);
//     wait 500, msec;
//     Claw.set(true);
//     wait 500, msec;
//     chainbar.spinToPosition(812, rotationUnits::deg, 100, velocityUnits::pct);
//   }
//   else {
//     RunRoller(0);

// }

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void)
{
  EXIT = true; // Force Exit Autosel once drivercontrol began.
               // User control code here, inside the loop

  // This is the main execution loop for the user control program.
  // Each time through the loop your program should update motor + servo
  // values based on feedback from the joysticks.

  task Dtask = task(DriveTask);
  // task Atask = task(ATask);
  // task Ptask = task(PTask);
  task Ltask = task(LiftTask);
  task Itask = task(IntakeBarTask);
  task Ctask = task(ClawTask);
  task CBtask = task(ChainbarTask);
  task IItask = task(IntakeTask);

  // ........................................................................
  // Insert user code here. This is where you use the joystick values to
  // update your motors, etc.
  // ........................................................................
  while (1)
  {
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//

int main()
{

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
