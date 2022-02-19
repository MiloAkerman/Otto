using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor ring;
extern motor lift;
extern motor transmission;
extern motor posessor;
extern controller Controller1;
extern motor lf;
extern motor lb;
extern motor rf;
extern motor rb;
extern inertial gyroscope;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );