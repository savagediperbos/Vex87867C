using namespace vex;

extern brain Brain;

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


void driverControl();
void auton();