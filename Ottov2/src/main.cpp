/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       The Otto Worshippers                                      */
/*    Created:      Sep 2021                                                  */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
vex::motor      DriveL1 = vex::motor(vex::PORT11);
vex::motor      DriveL2 = vex::motor(vex::PORT12);
vex::motor      DriveR1 = vex::motor(vex::PORT1, true);
vex::motor      DriveR2 = vex::motor(vex::PORT2, true);
vex::motor      LiftR = vex::motor(vex::PORT6, true);
vex::motor      LiftL = vex::motor(vex::PORT16);
//vex::motor      ClawR = vex::motor(vex::PORT7, true);
vex::motor      Claw = vex::motor(vex::PORT17);


vex::controller   Controller = vex::controller();

// Group setup
motor_group DrivetrainL(DriveL1, DriveL2);
motor_group DrivetrainR(DriveR1, DriveR2);
motor_group Lift(LiftL, LiftR);
//kill me
motor_group Drive(DriveL1, DriveL2, DriveR1, DriveR2);


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
  int ClawSpeedPCT = 75;
  int LiftSpeedPCT = 70;
  
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  DriveL1.setVelocity(100, velocityUnits::pct);
  DriveL2.setVelocity(100, velocityUnits::pct);
  DriveR1.setVelocity(100, velocityUnits::pct);
  DriveR2.setVelocity(100, velocityUnits::pct);
  Claw.setVelocity(ClawSpeedPCT, velocityUnits::pct);
  LiftL.setVelocity(LiftSpeedPCT, velocityUnits::pct);
  LiftR.setVelocity(LiftSpeedPCT, velocityUnits::pct);
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
  //be not afraid
  //spin forward about 5 feet, intake, then drive backwards about 5 feet, outtake
  Drive.spinFor(forward,1432,msec,100,velocityUnits::pct);
  Claw.spinFor(forward,200,msec, 100, velocityUnits::pct);
  Drive.spinFor(reverse,1432,msec,100,velocityUnits::pct);
  Claw.spinFor(reverse,200,msec, 100,velocityUnits::pct);
  //make the spin command in robotics cause idfk rn im in spanish like im doing what i can
  //btw its supposed to spin about 50 degrees clockwise (relative to how the robot is facing, spin forward 1.5 feet, then intake)
  //drive back about 2 feet, then outtake, hopefully thinking about adding a spin counterclockwise
  Drive.spinFor(forward,430,msec, 100, velocityUnits::pct);
  //hEY OVERALLS
  Claw.spinFor(forward,200,msec,100,velocityUnits::pct);
  Drive.spinFor(reverse,573,msec,100,velocityUnits::pct);
  Claw.spinFor(reverse,200,msec,100,velocityUnits::pct);
  //maybe add another spin command here??? idk i guess to leave it where it was but idk lol

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
    //move drivetrain to controller stick position
   int ClawSpeedPCT = 75;
   int LiftSpeedPCT = 70;

   DrivetrainL.spin(vex::directionType::fwd, Controller.Axis3.position(), vex::velocityUnits::pct);
   DrivetrainR.spin(vex::directionType::fwd, Controller.Axis2.position(), vex::velocityUnits::pct); 

   // claw open and close (bless Cornelius)
   if(Controller.ButtonR1.pressing()) {
     Claw.spin(directionType::fwd, ClawSpeedPCT, velocityUnits::pct);
    }
     else if (Controller.ButtonR2.pressing()){
     Claw.spin(directionType::rev, ClawSpeedPCT,velocityUnits::pct);
   }
   else{
     Claw.setStopping(brake);
     Claw.stop(brakeType::brake);
   }

   // Lift up and down (bless Cornelius)

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
   //you cannot kill me in a way that matters

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

// Main will set up the competition functions and callbacks.
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
