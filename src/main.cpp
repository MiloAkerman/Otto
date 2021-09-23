/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
vex::motor      DriveL1 = vex::motor( vex::PORT11);
vex::motor      DriveL2 = vex::motor( vex::PORT12);
vex::motor      DriveR1 = vex::motor( vex::PORT1, true);
vex::motor      DriveR2 = vex::motor( vex::PORT2, true);
vex::motor      LiftR = vex::motor( vex::PORT6, true);
vex::motor      LiftL = vex::motor( vex::PORT16);
vex::motor      ClawR = vex::motor( vex::PORT7, true);
vex::motor      ClawL = vex::motor( vex::PORT17);


vex::controller   Controller = vex::controller();

// define your global instances of motors and other devices here
motor_group DrivetrainL(DriveL1, DriveL2);
motor_group DrivetrainR(DriveR1, DriveR2);
motor_group Lift(LiftL, LiftR);
motor_group Claw(ClawL, ClawR);


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  DriveL1.setVelocity(100,velocityUnits::pct);
  DriveL2.setVelocity(100,velocityUnits::pct);
  DriveR1.setVelocity(100,velocityUnits::pct);
  DriveR2.setVelocity(100,velocityUnits::pct);
  ClawL.setVelocity(25,velocityUnits::pct);
  ClawR.setVelocity(25,velocityUnits::pct);
  LiftL.setVelocity(100,velocityUnits::pct);
  LiftR.setVelocity(100,velocityUnits::pct);

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

void autonomous(void) {
  
    DrivetrainL.setVelocity(50,velocityUnits::pct);
   DrivetrainR.setVelocity(50,velocityUnits::pct);
  DrivetrainL.spin(vex::directionType::fwd);
   DrivetrainR.spin(vex::directionType::fwd, Controller.Axis2.position(), vex::velocityUnits::pct);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
   int ClawSpeedPCT = 75;
   int LiftSpeedPCT = 70;


    DrivetrainL.spin(vex::directionType::fwd, Controller.Axis3.position(), vex::velocityUnits::pct);
   DrivetrainR.spin(vex::directionType::fwd, Controller.Axis2.position(), vex::velocityUnits::pct);
   

   if(Controller.ButtonR1.pressing()) {
     Claw.spin(directionType::fwd, ClawSpeedPCT, velocityUnits::pct);
     //ClawR.spin(directionType::fwd, ClawSpeedPCT, velocityUnits::pct);
    }
     else if (Controller.ButtonR2.pressing()){
     Claw.spin(directionType::rev, ClawSpeedPCT,velocityUnits::pct);
     //ClawR.spin(directionType::rev, ClawSpeedPCT,velocityUnits::pct);
   }
   else{
     Claw.setStopping(brake);
     Claw.stop(brakeType::brake);
   }


   if(Controller.ButtonL1.pressing()) {
      Lift.spin(directionType::fwd, LiftSpeedPCT, velocityUnits::pct);
    }
    
    else if (Controller.ButtonL2.pressing()) 
    
    {
      Lift.spin(directionType::rev, LiftSpeedPCT,velocityUnits::pct);
      Lift.spin(directionType::rev, LiftSpeedPCT,velocityUnits::pct);
      
    }
    else {
      //since nothing is touching either button, it stops
      
      LiftR.setStopping(brake); 
      LiftL.setStopping(brake);
      LiftR.stop(brakeType::brake);
      LiftL.stop(brakeType::brake);
    }
   
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
