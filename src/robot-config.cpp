#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor lift = motor(PORT6, ratio36_1, false);
motor transmission = motor(PORT12, ratio18_1, true);
motor ring = motor(PORT8, ratio18_1, true);
motor posessor = motor(PORT15, ratio18_1, false);
motor lf = motor(PORT16, ratio18_1, true);
motor lb = motor(PORT17, ratio18_1, false);
motor rf = motor(PORT18, ratio18_1, false);
motor rb = motor(PORT19, ratio18_1, true);
inertial gyroscope(PORT21);


// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  //nope
}