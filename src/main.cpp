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
vex::inertial Inertial = vex::inertial(vex::PORT10);

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
double percentageAllianceGoal = 57.5;
int allianceGoalRotation = 2000;
int msecNeutralGoal = 2800;
int msecAllianceGoal = 1600;
int msecBigGoal = 4000;

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

  Inertial.calibrate();
  while( Inertial.isCalibrating() ) { wait(10,msec); }

  // might not be executing during testing
  Drive.setVelocity(90, velocityUnits::pct);
  Claw.setVelocity(90, velocityUnits::pct);
  Lift.setVelocity(90, velocityUnits::pct);
  Mogo.setVelocity(100, velocityUnits::pct);
  Conveyor.setVelocity(90, velocityUnits::pct);

  // Reset motors which rotate based on degrees 
  Claw.resetRotation(); // claw starts CLOSED
  Mogo.resetRotation(); // Mogo starts UP
  Lift.resetRotation(); // Lift starts DOWN

  Lift.setStopping(brakeType::hold);
  Mogo.setStopping(brakeType::hold);
  Claw.setStopping(brakeType::hold);
}

/*------------------------------  AUTON  -------------------------------*/

void grabNeutralGoal() {
  Drive.setVelocity(100, velocityUnits::pct); // max speed to avoid stealing

  Drive.spinFor(forward, msecNeutralGoal, msec); // drives to neutral goal
  toggleClaw();                                  // grab goal

  Drive.spinFor(reverse, msecNeutralGoal * (percentageAllianceGoal/100),
                msec);           // drives halfway back to alliance zone
  //toggleClaw();                  // drops goal
}
void grabAllianceGoal() {
  DriveL.spin(reverse);
  DriveR.spin(forward);
  waitUntil(Inertial.heading() < 285 && Inertial.heading() > 50); // drift
  Drive.stop(brakeType::brake);
  Drive.spinFor(forward, 400,
                msec);           // drives to alliance

  toggleMogo(true);
  Drive.spinFor(reverse, msecAllianceGoal,
                msec);           // drives to alliance
  wait(0.2, timeUnits::sec);
  toggleMogo(true);
  wait(0.2, timeUnits::sec);
  Drive.spinFor(forward, msecAllianceGoal,
                msec);           // drives back
  toggleConveyor();
}
void grabBigGoal() {
  toggleClaw();
  DriveL.spin(forward);
  DriveR.spin(reverse);
  waitUntil(Inertial.heading() > 315 && Inertial.heading() < 340); // drift

  Drive.spinFor(forward, msecBigGoal,
                msec);           // drives to alliance
  toggleClaw();
}

void autonomous(void) {
  // clear claw
  Lift.spinToPosition(80, rotationUnits::deg); // allow for claw to open
  toggleClaw();                                // opens claw
  Lift.spinToPosition(0, rotationUnits::deg);  // prepare for closing

  grabNeutralGoal();
  grabAllianceGoal();
  //grabBigGoal();
}

/*------------------------------  USER CONTROL  -----------------------------*/

void usercontrol(void) {
  // worth a try
  Drive.setVelocity(100, velocityUnits::pct);
  Claw.setVelocity(100, velocityUnits::pct);
  Lift.setVelocity(90, velocityUnits::pct);
  Mogo.setVelocity(100, velocityUnits::pct);
  Conveyor.setVelocity(100, velocityUnits::pct);

  bool pressing[3] = {false, false, false};

  while (1) {
    // Move drivetrain to controller stick position
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
  Brain.Screen.render(true,false); //enable double buffering for smoother drawing

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    Brain.Screen.clearLine(1,color::black);
    Brain.Screen.clearLine(2,color::black);
    Brain.Screen.setCursor(1,0);
    Brain.Screen.print("Sensor heading: %f degrees", Inertial.heading());
    Brain.Screen.render(); //push data to the LCD all at once to prevent image flickering

    wait(100, msec);
  }
}
