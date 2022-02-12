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

competition Competition;

// motor setup
vex::motor DriveL1 = vex::motor(vex::PORT16, true);
vex::motor DriveL2 = vex::motor(vex::PORT17);
vex::motor DriveR1 = vex::motor(vex::PORT18);
vex::motor DriveR2 = vex::motor(vex::PORT19, true);
vex::motor Mogo = vex::motor(vex::PORT15);
vex::motor Lift = vex::motor(vex::PORT6);
vex::motor Claw = vex::motor(vex::PORT12, true);
vex::motor Conveyor = vex::motor(vex::PORT8, true);
vex::inertial Inertial = vex::inertial(vex::PORT21);

// button setup
vex::controller Controller = vex::controller();
vex::controller::button MogoToggle = Controller.ButtonR1;
vex::controller::button ClawToggle = Controller.ButtonR2;
vex::controller::button LiftUp = Controller.ButtonL1;
vex::controller::button LiftDown = Controller.ButtonL2;
vex::controller::button ConveyorToggle = Controller.ButtonLeft;

// Group setup
motor_group DriveL(DriveL1, DriveL2);
motor_group DriveR(DriveR1, DriveR2);
motor_group Drive(DriveL1, DriveL2, DriveR1, DriveR2);

// Autonomous values
double allianceGoalRotation = 40.0;
int msecNeutralGoal = 2800;
int msecAllianceGoal = 1500;

/*------------------------------  HELPERS  --------------------------*/
bool mogoUp = true;
bool clawUp = false;
bool conveyorOn = false;

void toggleClaw(bool waitForCompletion = false) {
  Claw.stop();
  if (clawUp) {
    Claw.spinTo(0, rotationUnits::deg, waitForCompletion);
  } else {
    Claw.spinTo(360, rotationUnits::deg, waitForCompletion);
  }
  clawUp = !clawUp;
}
void toggleMogo(bool waitForCompletion = false) {
  Mogo.stop();
  if (mogoUp) {
    Mogo.spinTo(500, rotationUnits::deg, waitForCompletion);
  } else {
    Mogo.spinTo(0, rotationUnits::deg, waitForCompletion);
  }
  mogoUp = !mogoUp;
}
void toggleConveyor() {
  if (!conveyorOn) {
    Conveyor.spin(directionType::rev);
  } else {
    Conveyor.stop();
  }
  conveyorOn = !conveyorOn;
}

/*------------------------------  PRE-AUTON  --------------------------*/

void pre_auton(void) {
  vexcodeInit(); // DO NOT REMOVE JESUS CHRIST ARE YOU INSANE WHAT THE HELL ARE
                 // YOU DOING

  Inertial.startCalibration();
  waitUntil(!Inertial.isCalibrating());

  // might not be executing during testing
  Drive.setVelocity(100, velocityUnits::pct);
  Claw.setVelocity(90, velocityUnits::pct);
  Lift.setVelocity(90, velocityUnits::pct);
  Mogo.setVelocity(100, velocityUnits::pct);
  Conveyor.setVelocity(100, velocityUnits::pct);

  // Reset motors which rotate based on degrees
  Claw.resetRotation(); // claw starts CLOSED
  Mogo.resetRotation(); // Mogo starts UP
  Lift.resetRotation(); // Lift starts DOWN
}

/*------------------------------  AUTON  -------------------------------*/

void grabNeutralGoal() {
  Drive.setVelocity(100, velocityUnits::pct); // max speed to avoid stealing

  Drive.spinFor(forward, msecNeutralGoal, msec); // drives to neutral goal
  toggleClaw();                                  // grab goal

  Drive.spinFor(reverse, msecNeutralGoal * 0.61,
                msec);           // drives halfway back to alliance zone
}

void allianceGoal() {
  Drive.setVelocity(80, velocityUnits::pct);

  DriveL.spin(forward);
  DriveR.spin(reverse);
  waitUntil(Inertial.heading() > 90);
  Drive.stop(brakeType::brake);  

  /* LEFT TURN
  DriveL.spin(reverse);                                           // spins both
  motors in DriveR.spin(forward);                                           //
  reverse directions to turn vex::task::sleep(halfTurnMsecs); // waits until
  turn is over Drive.stop(brakeType::brake); // stops both motors*/

  // PROTO FULL SPIN + ACCOUNT FOR DRIFT
  /*DriveL.spin(reverse);
  DriveR.spin(forward);
  vex::task::sleep(halfTurnMsecs * 2 - 92);
  Drive.stop(brakeType::brake);*/
}

void autonomous(void) {
  /*allianceGoal();
  vex::task::sleep(500);
  grabNeutralGoal();*/

  // clear claw
  Lift.spinToPosition(50, rotationUnits::deg); // allow for claw to open
  toggleClaw();                                // opens claw
  Lift.spinToPosition(0, rotationUnits::deg);  // prepare for closing

  grabNeutralGoal();

  DriveL.spin(reverse);
  DriveR.spin(forward);
  waitUntil(Inertial.heading() > 200 && Inertial.heading() < 290); // drift
  Drive.stop(brakeType::brake);

  toggleMogo();
  Drive.spinFor(reverse, msecAllianceGoal,
                msec);           // drives to alliance
  toggleMogo();
  Drive.spinFor(forward, msecAllianceGoal,
                msec);           // drives back
  toggleConveyor();

  Drive.setVelocity(100, velocityUnits::pct);
  Claw.setVelocity(100, velocityUnits::pct);
  Lift.setVelocity(100, velocityUnits::pct);
}

/*------------------------------  USER CONTROL  -----------------------------*/

void usercontrol(void) {
  // worth a try
  Drive.setVelocity(100, velocityUnits::pct);
  Claw.setVelocity(90, velocityUnits::pct);
  Lift.setVelocity(90, velocityUnits::pct);
  Mogo.setVelocity(100, velocityUnits::pct);
  Conveyor.setVelocity(100, velocityUnits::pct);

  bool pressing[3] = {false, false, false};
  Lift.setStopping(brakeType::hold);
  Mogo.setStopping(brakeType::hold);
  Claw.setStopping(brakeType::hold);


  while (1) {
    Controller.Screen.newLine();
    Controller.Screen.print("%f", Inertial.heading(rotationUnits::deg));
    // Move drivetrain to controller stick posittion
    DriveL.spin(vex::directionType::fwd, Controller.Axis3.position(),
                vex::velocityUnits::pct);
    DriveR.spin(vex::directionType::fwd, Controller.Axis2.position(),
                vex::velocityUnits::pct);

    // Claw close and open (bless Cornelius)
    if (ClawToggle.pressing() && !pressing[1]) {
      toggleClaw();
    }
    // Mogo up and down (bless Cornelius)
    if (MogoToggle.pressing() && !pressing[0]) {
      toggleMogo();
    }
    // Claw close and open (bless Cornelius)
    if (ConveyorToggle.pressing() && !pressing[2]) {
      toggleConveyor();
    }

    // Lift up & down (bless Cornelius)
    if (LiftUp.pressing()) {
      Lift.spin(directionType::fwd);
    } else if (LiftDown.pressing()) {
      Lift.spin(directionType::rev);
    } else {
      Lift.stop();
    }

    pressing[0] = MogoToggle.pressing();
    pressing[1] = ClawToggle.pressing();
    pressing[2] = ConveyorToggle.pressing();

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
