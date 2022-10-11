#include <robot-config.h>

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor FL = motor(PORT1, ratio18_1, true);
motor FR = motor(PORT2, ratio18_1, false);
motor BL = motor(PORT11, ratio18_1, true);
motor BR = motor(PORT16, ratio18_1, false);
controller Controller1 = controller(primary);
encoder LEncoder = encoder(Brain.ThreeWirePort.E);
encoder REncoder = encoder(Brain.ThreeWirePort.C);
motor intakeroller = motor(PORT15, ratio18_1, false);
motor flywheel = motor(PORT18, ratio6_1, true);
motor flywheel2 = motor(PORT19, ratio6_1, false);
motor indexer = motor(PORT17, ratio18_1, true);
inertial Inertial10 = inertial(PORT10);
digital_out expansion = digital_out(Brain.ThreeWirePort.H);