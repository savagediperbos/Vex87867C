#include "vex.h"

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

// Configuration
double maxRPM = 400.0; // the assigned maximum value rpm for the flywheel, default value
double minRPM = 250.0; // the assigned minimum value rpm for the flywheel, activated by using the toggle
double fD = 50.0; // distance from desired rpm at which flywheel starts decreasing in rpm

// Status trackers
double desiredRPM = maxRPM; // DO NOT TOUCH - rpm at which the flywheel is designated to spin, maximum rpm
double limiter = 0.0; // DO NOT TOUCH - limiter/value subtracted from flywheel rpm
bool flywheelVelocityMax = true; // DO NOT TOUCH - keeps track of flywheel desired rpm for rpm toggle
bool remoteControlCodeEnabled = true; // DO NOT TOUCH - keeps track of remote controller enable/disable
bool flywheelStopped = true; // DO NOT TOUCH - keeps track of flywheel motion status
bool intakeStopped = true; //DO NOT TOUCH - keeps track of intake motion status

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  while (true) {
    if (remoteControlCodeEnabled) {
      if (Controller1.ButtonB.pressing()) {
        // allows for consistent flywheel rpm - flywheel pid
        if (flywheel.velocity(rpm) > (desiredRPM - fD)) {
          limiter = flywheel.velocity(rpm) - desiredRPM + fD;
        }
        else {
          limiter = 0.0;
        }
        flywheel.spin(reverse, (desiredRPM + fD - limiter), rpm);
        flywheelStopped = false;
      }
      else if (!flywheelStopped) {
        flywheel.stop();
        flywheelStopped = true;
      }

      if (Controller1.ButtonY.pressing() && !Controller1.ButtonB.pressing()) {
      // toggles flywheel velocity between max rpm (default) and minimum rpm
        if (flywheelVelocityMax) {
          desiredRPM = minRPM;
          flywheelVelocityMax = false;
        }
        else if (!flywheelVelocityMax) {
          desiredRPM = maxRPM;
          flywheelVelocityMax = true;
        }
      }

      if (Controller1.ButtonL1.pressing() && !Controller1.ButtonL2.pressing()) {
      // intake in
        intakeroller.spin(forward);
        intakeStopped = false;
        wait(5, msec);
      }
      else if (Controller1.ButtonL1.pressing() && !intakeStopped) {
        intakeroller.spinFor(forward, 95.0, degrees);
        intakeroller.stop();
        intakeStopped = true;
      }

      if (Controller1.ButtonL2.pressing() && !Controller1.ButtonL1.pressing()) {
      // intake out
        intakeroller.spin(reverse);
        intakeStopped = false;
        wait(5, msec);
      }
      else if (Controller1.ButtonL2.pressing() && !intakeStopped) {
        intakeroller.spinFor(reverse, 95.0, degrees);
        intakeroller.stop();
        intakeStopped = true;
      }
    }
    wait(20, msec); // checks conditions every 20 milliseconds
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
}