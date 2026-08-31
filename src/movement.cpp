#include "vex.h"

#include "math.h"
#include "screen_gui.hpp"
#include "helper_functions.hpp"
#include "movement.hpp"

#include <iostream>

using namespace vex;

int turninverse=-1;//change this to -1 if turning is inversed

int JB;
int PB;
int PX;
int JX;

//General Sect;
//This section includes all general codes for drive and auto


/** Resets the robot's drive train and inertial sensor
 * 
 * @param dist the 
 * @param HDG the
 */
void Zeroing(bool dist, bool HDG)
{
  if(dist){
  LF.resetPosition();
  LM.resetPosition();
  LB.resetPosition();
  RF.resetPosition();
  RM.resetPosition();
  RB.resetPosition();
  }
  if(HDG){
    Gyro.setHeading(0,degrees);
  }
}

ChassisDataSet ChassisUpdate()
{
  ChassisDataSet CDS;
  CDS.Left=get_dist_travelled((LF.position(degrees)+LM.position(degrees)+LB.position(degrees))/3.0);
  CDS.Right=get_dist_travelled((RF.position(degrees)+RM.position(degrees)+RB.position(degrees))/3.0);
  CDS.Avg=(CDS.Left+CDS.Right)/2;
  CDS.Diff=CDS.Left-CDS.Right;
  CDS.HDG=Gyro.heading(degrees);

  return CDS;
}

void Move(int left, int right)
{
LF.setMaxTorque(100,percent);
LM.setMaxTorque(100,percent);
LB.setMaxTorque(100,percent);
RF.setMaxTorque(100,percent);
RM.setMaxTorque(100,percent);
RB.setMaxTorque(100,percent);

LF.spin(forward,(double)left/100.0*11,volt);
LM.spin(forward,(double)left/100.0*11,volt);
LB.spin(forward,(double)left/100.0*11,volt);
RF.spin(forward,(double)right/100.0*11,volt);
RM.spin(forward,(double)right/100.0*11,volt);
RB.spin(forward,(double)right/100.0*11,volt);
}

void BStop()
{
LF.setStopping(brake);
LM.setStopping(brake);
LB.setStopping(brake);
RF.setStopping(brake);
RM.setStopping(brake);
RB.setStopping(brake);

LF.stop();
LM.stop();
LB.stop();
RF.stop();
RM.stop();
RB.stop();
}

void CStop()
{
LF.setStopping(coast);
LM.setStopping(coast);
LB.setStopping(coast);
RF.setStopping(coast);
RM.setStopping(coast);
RB.setStopping(coast);

LF.stop();
LM.stop();
LB.stop();
RF.stop();
RM.stop();
RB.stop();
}




void RunRoller(int val)
{
Roller.setMaxTorque(100,percent);
Roller.spin(forward,(double)val/100.0*12,volt);
}

int PrevE;//Error at t-1

/** Moves the robot forward or backward. Negative speed moves
 * the robot forward. Positive value moves it backward. (Ik it's fucked up)
 * @param KVals the PID constants
 * @param Speed the speed, from -100 to 100
 * @param dist distance travelled, in inches
 * @param AccT time to max speed (s)
 * @param ABSHDG absolute heading of the robot
 * @param brake Brake at end, or coast
 */
void MoveEncoderPID(PIDDataSet KVals, int Speed, double dist,double AccT, double ABSHDG,bool brake){
  double CSpeed=0;
  Zeroing(true,false);
  ChassisDataSet SensorVals;
  SensorVals=ChassisUpdate();
  double PVal=0;
  double IVal=0;
  double DVal=0;
  double LGV=0;//define local gyro variable.
  PrevE=0;
  double Correction=0;
  Brain.Screen.clearScreen();

  while(fabs(SensorVals.Avg) <= fabs(dist))
  {
    //std::cout << SensorVals.Avg << " " << dist << std::endl;
if(fabs(CSpeed)<fabs((double)Speed))
{
  CSpeed+=Speed/AccT*0.02;
}

  SensorVals=ChassisUpdate();
  LGV=SensorVals.HDG-ABSHDG;
  if(LGV>180) LGV=LGV-360;
  PVal=KVals.kp*LGV;
  IVal=IVal+KVals.ki*LGV*0.02;
  DVal=KVals.kd*(LGV-PrevE);

  Correction=PVal+IVal+DVal/0.02;

  Move(CSpeed-Correction,CSpeed+Correction);
  PrevE=LGV;
  wait(20, msec);
  }
  if(brake){
    BStop();
    wait(120,msec);
  }
  else CStop();
}
/** Moves the robot forward or backward
 * @param KVals the PID constants for heading correctoin
 * @param DVals the PID constants for forward and back movement(deceleration)
 * @param Speed the max speed of the movement
 * @param dist the desired distance for the movement
 * @param AccT the time to accelerate to max speed
 * @param DeltaAngle the absolute heading to turn to
 * @param ABSHDG absolute heading of the robot
 * @param brake Brake at end, or coast\
 * @param time Max time before exiting movement(Use incase you get stuck on smt)
 */
void MovePID(PIDDataSet KVals, PIDDataSet Dvals, int Speed, double dist, double AccT, double ABSHDG, bool brake, double time) {
    double CSpeed = 0;
    Zeroing(true, false);
    ChassisDataSet SensorVals = ChassisUpdate();
    
    // PID Variables
    double PVal = 0, IVal = 0, DVal = 0;
    double Correction = 0;
    
    // Initialize PrevE to current heading error (prevents D-term spike on first loop)
    double init_err = SensorVals.HDG - ABSHDG;
    while (init_err > 180) init_err -= 360;
    while (init_err < -180) init_err += 360;
    double PrevE = init_err;
    
    // Distance PID Variables
    double LDGV = 0, PrevDE = 0;
    double PDVal = 0, IDVal = 0, DDVal = 0;
    
    Brain.Timer.reset();
    
    double prev_time = Brain.Timer.value();

    while ((fabs(dist) - fabs(SensorVals.Avg)) > 0.5 && Brain.Timer.value() <= time) {
        
        // 1. CALCULATE DT (Delta Time)
        double curr_time = Brain.Timer.value();
        double dt = curr_time - prev_time;
        prev_time = curr_time;
        if (dt < 0.01) dt = 0.01; // Safety minimum

        SensorVals = ChassisUpdate();
        double remaining_dist = dist - fabs(SensorVals.Avg);
  
        double dist_error = fabs(remaining_dist);
        PDVal = Speed * (dist_error / fabs(dist)) * Dvals.kp;
        IDVal += Dvals.ki * dist_error * dt; // Multiply by dt, don't hardcode 0.02
        DDVal = Dvals.kd * (dist_error - PrevDE);
        PrevDE = dist_error;

        double base_target_speed = PDVal + IDVal + DDVal; 

        // Safety Deceleration Logic
        const double MIN_DECEL_DIST = 3.0;
        if (fabs(dist) < MIN_DECEL_DIST) {
             double speed_limit = fabs(Speed) * (0.3 + 0.7*(fabs(dist)/MIN_DECEL_DIST));
             base_target_speed = copysign(fmin(fabs(base_target_speed), speed_limit), Speed);
             if (fabs(remaining_dist) < MIN_DECEL_DIST/2) {
                 base_target_speed *= 0.8;
             }
        }
        
        // Ensure Minimum Speed to prevent stalling
        base_target_speed = copysign(fmax(fabs(base_target_speed), 15.0), Speed);

        // Slew Rate (Acceleration)
        double ramp_rate = Speed / AccT;
        double max_change = fabs(ramp_rate * dt); // Limit change per loop

        if (fabs(CSpeed) < fabs(base_target_speed)) {
            // Accelerate
            CSpeed += copysign(max_change, Speed);
            // Clamp to target
            if (fabs(CSpeed) > fabs(base_target_speed)) CSpeed = base_target_speed;
        } else {
          
            CSpeed = base_target_speed;
        }

        // --- HEADING CORRECTION ---
        // (Matches MoveEncoderPID exactly)
        double current_heading = SensorVals.HDG; 
        double heading_error = current_heading - ABSHDG;
        while (heading_error > 180) heading_error -= 360;
        while (heading_error < -180) heading_error += 360;

        PVal = KVals.kp * heading_error;
        IVal = IVal + KVals.ki * heading_error * 0.02;
        DVal = KVals.kd * (heading_error - PrevE);
        PrevE = heading_error;
        Correction = PVal + IVal + DVal / 0.02;

        // Scale correction by speed to prevent overcorrection during ramp-up
        double SpeedScale = (fabs(Speed) > 0) ? fmin(fabs(CSpeed) / fabs(Speed), 1.0) : 1.0;
        Correction *= SpeedScale;

        double left_req = CSpeed - Correction;
        double right_req = CSpeed + Correction;

        double max_req = fmax(fabs(left_req), fabs(right_req));
        double limit = 100.0; 
        if (max_req > limit) {
            left_req = (left_req / max_req) * limit;
            right_req = (right_req / max_req) * limit;
        }

        Move(left_req, right_req);
        wait(20, msec);
    }

    if(brake){
        BStop();
        wait(80, msec);
    } else {
        CStop();
    }
}
/** Moves the robot forward or backward. Negative speed moves
 * the robot forward. Positive value moves it backward. (Ik it's fucked up)
 * @param KVals the PID constants
 * @param DeltaAngle the absolute heading to turn to
 * @param TE time to calculate turn (not time to turn)
 * @param brake Brake at end, or coast
 */
void TurnMaxTimePID(PIDDataSet KVals,double DeltaAngle,double TE, bool brake){
  double CSpeed=0;
  Zeroing(true,false);
  ChassisDataSet SensorVals;
  SensorVals=ChassisUpdate();
  double PVal=0;
  double IVal=0;
  double DVal=0; 
  double LGV=0;
  PrevE=0;
  double Correction=0;
  Brain.Timer.reset();

  while(Brain.Timer.value() <= TE)
  {
  SensorVals=ChassisUpdate();
  LGV=SensorVals.HDG-DeltaAngle;
  if(LGV>180) LGV=LGV-360;
  PVal=KVals.kp*LGV;
  IVal=IVal+KVals.ki*LGV*0.02;
  DVal=KVals.kd*(LGV-PrevE);

  Correction=PVal+IVal+DVal/0.02;

  Move(CSpeed-Correction,CSpeed+Correction);
  PrevE=LGV;
  wait(20, msec);
  }
  if(brake){BStop();
  wait(180,msec);}
  else CStop();
}

void MoveTimePID(PIDDataSet KVals, int Speed, double TE,double AccT,double ABSHDG, bool brake){
  double CSpeed=0;
  Zeroing(true,false);
  ChassisDataSet SensorVals;
  SensorVals=ChassisUpdate();
  double PVal=0;
  double IVal=0;
  double DVal=0;
  double LGV=0;
  PrevE=0;
  double Correction=0;
  Brain.Timer.reset();

  while(Brain.Timer.value() <= TE)
  {
if(fabs(CSpeed)<fabs((double)Speed))
{
  CSpeed+=Speed/AccT*0.02;
}

  SensorVals=ChassisUpdate();
    LGV=SensorVals.HDG-ABSHDG;
  if(LGV>180) LGV=LGV-360;
  PVal=KVals.kp*LGV;
  IVal=IVal+KVals.ki*LGV*0.02;
  DVal=KVals.kd*(LGV-PrevE);

  Correction=PVal+IVal+DVal/0.02;

  Move(-CSpeed-Correction,-CSpeed+Correction);
  PrevE=LGV;
  wait(20, msec);
  }
  if(brake){BStop();
  wait(200,msec);}
  else CStop();
}

void MoveTime(int Speed, double TE,double AccT, bool brake){
  double CSpeed=0;
  Zeroing(true,false);
  ChassisDataSet SensorVals;
  SensorVals=ChassisUpdate();
  Brain.Timer.reset();

  while(Brain.Timer.value() <= TE)
  { 
    if(fabs(CSpeed)<fabs((double)Speed)){
      CSpeed+=Speed/AccT*0.02;
    }
    Move(CSpeed, CSpeed);
    wait(20,msec);

  }
  if(brake){BStop();
  wait(200,msec);}
  else CStop();
}