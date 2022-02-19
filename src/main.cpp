/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Zachary R                                        */
/*    Created:      Mon Jan 31 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Ring                 motor         7               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;




///////////////////////////////
bool leftifnotright = false;
bool aiming4middle = false;
////////////////////////////////


//Voltage in MV for motors
int Max = 12000;
//Threshhold for drivetrain to prevent sticky drift on the controller
int THRESH = 5;
// define variables used for controlling motors based on controller inputs
int transmission_delay = 0;
// declares the int that is used for making the button
bool Controller1LeftShoulderControlMotorsStopped = true;
bool Controller1RightShoulderControlMotorsStopped = true;
bool Controller1UpDownButtonsControlMotorsStopped = true;
bool ConveyorStopped = true;
bool ConveyorUp = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;



//Drivetrain function ripped from califronia
const int SCALE = 120;
void set_tank(int l, int r) {
  lf.spin(fwd, l*SCALE, voltageUnits::mV);
  lb.spin(fwd, l*SCALE, voltageUnits::mV);
  rb.spin(fwd, r*SCALE, voltageUnits::mV);
  rf.spin(fwd, r*SCALE, voltageUnits::mV);
  Brain.Screen.print(l);
}
void brake_drive() {
  lf.setStopping(hold);
  lb.setStopping(hold);
  rf.setStopping(hold);
  rb.setStopping(hold);
}
void coast_drive() {
  lf.setStopping(coast);
  lb.setStopping(coast);
  rf.setStopping(coast);
  rb.setStopping(coast);
}

void set_posessor  (int input) { posessor.  spin(fwd, input*SCALE, voltageUnits::mV); }
void set_transmission(int input) { transmission.spin(fwd, input*SCALE, voltageUnits::mV); }
void set_lift  (int input) { lift.  spin(fwd, input*SCALE, voltageUnits::mV); }
void set_ring  (int input) { ring.  spin(fwd, input*SCALE, voltageUnits::mV); }
// Set position
void set_posessor_position  (int pos, int speed) { posessor.  startRotateTo(pos, rotationUnits::deg, speed, velocityUnits::pct); }
void set_lift_position  (int pos, int speed) { lift.  startRotateTo(pos, rotationUnits::deg, speed, velocityUnits::pct); }

bool WAS_RESET_SUCCESS = false;
/////////////////////////////////////////////
void pre_auton(void) {
  vexcodeInit();

  WAS_RESET_SUCCESS = true;
}

bool did_auto_finish = false;
/////////////////////////////////////////////
void autonomous(void) {
  posessor.setBrake(brakeType::brake);
  lift.setBrake(brakeType::brake);
  brake_drive();
  set_tank(120,120);
  set_lift(-120);
  set_ring(-25);
  set_posessor(120);
  wait(500,msec);
  set_lift(0);
  set_posessor(0);
  set_ring(0);
  ///////////////////// "setup while the bot runs to middle" above this line
  wait(650,msec);
  set_tank(0,0);
  set_lift(120);
  wait(50,msec);
  set_tank(-120,-120);
  wait(100,msec);

  ///////////////// "bot picks up and gets a hold of yellow goal" above this line
  
  if(leftifnotright == true){
    
  }
  ///////////////// "left side normal" above this line
  if (leftifnotright == false){
    set_lift(0);
  set_tank(-120,20);
  wait(600,msec);
  set_tank(0,0);
  wait(500,msec);
  set_tank(-45,-45);
  wait(1600,msec);
  set_tank(0,0);
  set_posessor(-80);
  wait(1000,msec);
  set_posessor(0);
  }
  ///////////////// "right side normal" above this line


  set_tank(0,0);
  set_posessor(0);
  set_lift(0);
  set_ring(0);
  wait(1000,msec);
  coast_drive();

  
///////////////// "ready for driving" above this line
  

  did_auto_finish = true;
}


////////////////////////////////////////////////////////////
void usercontrol(void) {
  while(WAS_RESET_SUCCESS == false){
    wait(10, msec);
  }
  while (1) {
   // calculate the drivetrain motor velocities from the controller joystick
      // axies left = Axis3 right = Axis2
        int l_joy = abs(Controller1.Axis3.value())>THRESH ? Controller1.Axis3.value() : 0;
        int r_joy = abs(Controller1.Axis2.value())>THRESH ? Controller1.Axis2.value() : 0;
        set_tank(l_joy, r_joy);
      // check the ButtonL1/ButtonL2 status to control Lift
      if (Controller1.ButtonL2.pressing()) {
        lift.spin(reverse, Max, vex::voltageUnits::mV);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonL1.pressing()) {
        lift.spin(forward, Max, vex::voltageUnits::mV);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        lift.setBrake(brakeType::brake);
        lift.stop();
        // set the toggle so that we don't constantly tell the motor to stop
        // when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
      // check the ButtonR1/ButtonR2 status to control Posessor
      if (Controller1.ButtonR1.pressing()) {
        posessor.spin(reverse, Max, vex::voltageUnits::mV);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR2.pressing()) {
        posessor.spin(forward, Max, vex::voltageUnits::mV);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        posessor.setBrake(brakeType::brake);
        posessor.stop();
        // set the toggle so that we don't constantly tell the motor to stop
        // when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }
      if (Controller1.ButtonLeft.pressing() && transmission_delay == 100000) {
      transmission.spin(reverse, Max, vex::voltageUnits::mV);
      } else if (Controller1.ButtonLeft.pressing() && transmission_delay < 100000) {
        transmission_delay= transmission_delay + 1;
      } else if (Controller1.ButtonDown.pressing() && transmission_delay == 100000){
      transmission.spin(forward, Max, vex::voltageUnits::mV);
      } else if (transmission_delay == 100000){
        transmission.stop();
      }

      // check the ButtonA/ButtonB/ButtonX/ButtonY Satus to control Ring but shittily logic'd because I dont care
      if (Controller1.ButtonA.pressing() && (ConveyorUp == false)) {
        ConveyorUp = true;
      }
      if (Controller1.ButtonB.pressing() && (ConveyorUp == true)) {
        ConveyorUp = false;
      }
      if (Controller1.ButtonX.pressing()) {
        ConveyorStopped = true;
      }
      if (Controller1.ButtonY.pressing()) {
        ConveyorStopped = false;
      }
      if (ConveyorStopped == false) {
          if (ConveyorUp == false) {
            ring.spin(forward, Max, vex::voltageUnits::mV);
          } else {
        ring.spin(reverse, Max, vex::voltageUnits::mV);
          }
       } else {
         ring.stop();
        }
  }
  // wait before repeating the process
  wait(10, msec);
}






int main() {
  //reset all positions
  pre_auton();
  //main functions
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  
 //time LOOP
 while(true) {
   wait(100,msec);
 }
}
