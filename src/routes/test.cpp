#include "../movement.hpp"
#include "../helper_functions.hpp"
#include "vex.h"
//PID Straight and turn arguments:
// MoveEncoderPID(TestPara, motor speed, encoder travel distance (inches), time to full speed(sec), relative heading(to starting position), braking?)
// TurnMaxTimePID(TestPara, Desired Heading -180 to 180, time out to calculate turn, Braking?)
// MoveTimePID(TestPara, motor speed, time traveled (sec), time to full speed, heading, false);
//liftAuto (int deg, int speed, bool brake);
//chainbarAuto (int deg, int speed, bool brake);
//clawAuto (bool state);

void test() {
    // declare initial conditions
    liftL.resetPosition();
    liftR.resetPosition();
    chainbar.resetPosition();
    IntakeBar.set(false);
    liftL.setStopping(hold);
    liftR.setStopping(hold);
    chainbar.setStopping(hold);
    chainbar.setMaxTorque(100,percent);
    Claw.set(false);
    Gyro.calibrate();

    PIDDataSet DrivePara={0.3,0.05,0.1};
    PIDDataSet TurnPara={0.5,0.3,0.10};

    //chainbar.spinToPosition(823, rotationUnits::deg, 100, velocityUnits::pct); // zero position


    MoveTimePID(DrivePara, 100, 0.2, 0.1, 0, false); //toggle 1
    MoveTimePID(DrivePara, -100, 0.2, 0.2, 0, false); //back out
    MoveTimePID(DrivePara, 100, 0.3, 0.1, 0, false); //toggle 2
    
   
    MoveEncoderPID(DrivePara, 100 , 20.0 ,1,310,true); //back out 
    //TurnMaxTimePID(TurnPara, 110, 0.3, true); //turn face goal
    //MoveTimePID(DrivePara, -100, 1.0, 0.8, 90.0, false); //move to goal
    
    // wait (50, msec);
    // clawAuto(true); //drop pin
    // wait (50, msec);

    //MoveEncoderPID(DrivePara, 100 , 8.0 ,1,113,true); //back out
    //TurnMaxTimePID(TurnPara, 113, 0.3, true); 
    
    
    
   
    
        
    }
        
   
    
    // lift (100, 100, true);
    


    /*MoveEncoderPID(TestPara, 100 , 65 ,0.4,0,true);
    TurnMaxTimePID(TestPara, 90, 0.4, true);
    MoveEncoderPID(TestPara, 100 , 50 ,0.4,90,true);
    wait(1000,msec);
    MoveEncoderPID(TestPara, -100 , 90 ,0.4,90,true);
        MoveEncoderPID(TestPara, -100 , 90 ,0.4,0,true);*/
    // // moves forward
    // MoveEncoderPID(TestPara, -100 , 24 ,0.2,0,true);
    // // turns to 90 degrees, 0.4s for calculations, brake when heading reached
    // TurnMaxTimePID(TestPara, 90, 0.4, true);
    // // Move backwards at 80 power, 10 inches, 0.2s to max speed, 
    // MoveEncoderPID(TestPara, 80 , 10 ,0.2,90,true);
    // // Turn to 60 degrees, 0.4s for calculations, braking
    // TurnMaxTimePID(TestPara, 0, 0.4, true);
    // // Move foward at 50 power, for 10 inches, 0.2 to accelerate to max speed, absolute heading, braking
    // MoveEncoderPID(TestPara, -50 , 10 ,0.2,60,true);

    //MoveEncoderPID(TestPara, 100 , 560 ,0.2,0,true);
    // MovePID(TestPara, 100, 5, 0.5, 0, true, 2); // Move forward 24 inches
    // wait(500, sec);
    // MovePID(TestPara, -100, 5, 0.5, 0, true, 2); // Move backward 24 inches

void test2(){  
    
     PIDDataSet TestPara={1.5,0.1,0.15};
   
}