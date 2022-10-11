using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor FL;
extern motor FR;
extern motor BL;
extern motor BR;
extern controller Controller1;
extern inertial Inertial21;
extern encoder LEncoder;
extern encoder REncoder;
extern encoder FEncoder;
extern motor intakeroller;
extern motor flywheel;
extern motor indexer;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );