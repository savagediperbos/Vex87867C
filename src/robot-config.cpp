#include <vex.h>
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
motor BR = motor(PORT14, ratio18_1, false);
controller Controller1 = controller(primary);
inertial Inertial21 = inertial(PORT21);
encoder LEncoder = encoder(Brain.ThreeWirePort.C);
encoder REncoder = encoder(Brain.ThreeWirePort.A);
encoder FEncoder = encoder(Brain.ThreeWirePort.E);
motor intakeroller = motor(PORT15, ratio18_1, false);
motor flywheel = motor(PORT18, ratio6_1, false);
motor indexer = motor(PORT19, ratio18_1, false);