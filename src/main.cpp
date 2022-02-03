/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       The Otto Worshippers                                      */
/*    Created:      Sep 2021                                                  */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
using namespace vex;

// A global instance of competition
competition Competition;
vex::motor DriveL1 = vex::motor(vex::PORT2, true);
vex::motor DriveL2 = vex::motor(vex::PORT11);
vex::motor DriveR1 = vex::motor(vex::PORT12, true);
vex::motor DriveR2 = vex::motor(vex::PORT1);
vex::motor LiftR = vex::motor(vex::PORT6);
vex::motor LiftL = vex::motor(vex::PORT16, true);
vex::motor Claw = vex::motor(vex::PORT20, true);

vex::controller Controller = vex::controller();

// Group setup
motor_group DriveL(DriveL1, DriveL2);
motor_group DriveR(DriveR1, DriveR2);
motor_group Drive(DriveL1, DriveL2, DriveR1, DriveR2);
motor_group Lift(LiftL, LiftR);

/*------------------------------  PRE-AUTON  --------------------------*/

void pre_auton(void) {
  int ClawSpeedPCT = 75;
  int LiftSpeedPCT = 70;

  vexcodeInit(); // DO NOT REMOVE JESUS CHRIST ARE YOU INSANE WHAT THE HELL ARE
                 // YOU DOING

  Claw.setVelocity(ClawSpeedPCT, velocityUnits::pct);
  LiftL.setVelocity(LiftSpeedPCT, velocityUnits::pct);
  LiftR.setVelocity(LiftSpeedPCT, velocityUnits::pct);

  // Reset motors which rotate based on degrees
  Claw.resetRotation();
  Lift.resetRotation();
}

/*------------------------------  AUTON  -------------------------------*/

int halfTurnMsecs = 430;\

void spinTo() {
  // Predictive
  int kP = 1000;
  int kI = 100;
  int kD = 10000;

  int offset = 30;
  float lastError = 0;
  float derivative = 0;
}

void grabGoal(bool close = true, int moveBack = 0) {
  int liftRaiseDegrees = 820;
  int liftApex = 150;

  if(close) {
    Lift.spinToPosition(liftApex, rotationUnits::deg);            // places lift on apex (~50 deg)
    Claw.spin(directionType::rev);                                // closes claw
    vex::task::sleep(200);                                        // waits to allow time for claw to close
    if(moveBack > 0) Drive.spinFor(reverse, moveBack, msec);      // allows for moving back fter grabbing (when raising)
    Lift.spinToPosition(liftRaiseDegrees, rotationUnits::deg);    // lifts lift to max
  } else {
    Lift.spinToPosition(liftApex, rotationUnits::deg);            // lowers lift to apex
    Claw.spin(directionType::fwd);                                // opens claw
  }
}

void grabNeutralGoal() {
  Drive.setVelocity(100, velocityUnits::pct);                     // max speed to avoid stealing
  int driveTimeMsecs = 1520;                                      // time it takes to get to goal

  Drive.spinFor(forward, driveTimeMsecs, msec);                   // drives to neutral goal
  grabGoal();                                                     // grab goal and raise

  Drive.spinFor(reverse, driveTimeMsecs*0.80, msec);              // drives halfway back to alliance zone
  Claw.spin(directionType::fwd);                                  // drops goal
  vex::task::sleep(200);

  Drive.spinFor(reverse, driveTimeMsecs*0.20, msec);              // finishes going back to starting spot
  Lift.spinToPosition(50, rotationUnits::deg);                    // lowers lift to apex

  Drive.setVelocity(80, velocityUnits::pct);                      // reduces speed again for better driver control
}

void allianceGoal() {
  Drive.setVelocity(80, velocityUnits::pct);

  Lift.spinToPosition(80, rotationUnits::deg);                   // lowers lift to apex
  Drive.spinFor(forward, 500, msec);                              // push goal
  grabGoal(false);                                                // open claw
  vex::task::sleep(200);                                          // wait for claw to open

  Lift.spinToPosition(0, rotationUnits::deg);                    // lowers lift to apex
  grabGoal();                                                     // grabs alliance goal

  Drive.spinFor(reverse, 1200, msec);                              // back up
  /* LEFT TURN
  DriveL.spin(reverse);                                           // spins both motors in 
  DriveR.spin(forward);                                           //    reverse directions to turn
  vex::task::sleep(halfTurnMsecs);                                // waits until turn is over
  Drive.stop(brakeType::brake);                                   // stops both motors*/

  // PROTO FULL SPIN + ACCOUNT FOR DRIFT
  /*DriveL.spin(reverse);
  DriveR.spin(forward);
  vex::task::sleep(halfTurnMsecs * 2 - 92);
  Drive.stop(brakeType::brake);*/
}

void autonomous(void) {
  allianceGoal();
  vex::task::sleep(500);
  grabNeutralGoal();

  Drive.setVelocity(100, velocityUnits::pct);
  Claw.setVelocity(100, velocityUnits::pct);
  Lift.setVelocity(100, velocityUnits::pct);
}

/*------------------------------  USER CONTROL  -----------------------------*/

void usercontrol(void) {
  while (1) {
    // Move drivetrain to controller stick posittion
    DriveL.spin(vex::directionType::fwd, Controller.Axis3.position(), vex::velocityUnits::pct);
    DriveR.spin(vex::directionType::fwd, Controller.Axis2.position(), vex::velocityUnits::pct);

    // Claw open and close (bless Cornelius)
    if (Controller.ButtonR1.pressing()) {
      Claw.spin(directionType::rev);
    } else if (Controller.ButtonR2.pressing()) {
      Claw.stop(brakeType::brake);
      Claw.spin(directionType::fwd);
    }

    // Lift up & down (bless Cornelius)
    if (Controller.ButtonL1.pressing()) {
      Lift.spin(directionType::fwd);
    } else if (Controller.ButtonL2.pressing()) {
      Lift.spin(directionType::rev);
    } else {
      Lift.setStopping(brake);
      Lift.stop(brakeType::brake);
    }

    wait(20, msec); // Sleep the task for a short amount of time to prevent
                    // wasted resources
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
