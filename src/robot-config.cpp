#include <vex.h>
#include <iostream>

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
double minRPM = 300.0; // the assigned minimum value rpm for the flywheel, activated by using the toggle
double fD = 50.0; // distance from desired rpm at which flywheel starts decreasing in rpm

// Status trackers
int curCommand; //DO NOT TOUCH - keeps track of intake function ran
int lastCommand; //DO NOT TOUCH - keeps track of previous intake function ran
double desiredRPM = maxRPM; // DO NOT TOUCH - rpm at which the flywheel is designated to spin, maximum rpm
double limiter = 0.0; // DO NOT TOUCH - limiter/value subtracted from flywheel rpm
bool flywheelVelocityMax = true; // DO NOT TOUCH - keeps track of flywheel desired rpm for rpm toggle
bool remoteControlCodeEnabled = true; // DO NOT TOUCH - keeps track of remote controller enable/disable
bool flywheelStopped = true; // DO NOT TOUCH - keeps track of flywheel motion status
bool intakeStopped = true; //DO NOT TOUCH - keeps track of intake motion status
bool runByForward; //DO NOT TOUCH - keeps track of intake activation status
bool intakeIn; //DO NOT TOUCH - keeps track of intake spin direction
bool firstIter = true; //DO NOT TOUCH - protects first iteration from intake wait time quota

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  while (true) {
    if (remoteControlCodeEnabled) {

      // turn controls
      FR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
      BR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
      FL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
      BL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() - Controller1.Axis4.value(), percent);

      if (Controller1.ButtonR1.pressing()) {
        // activates indexer and shoots one time
        indexer.spinFor(forward,95.0,degrees);
        indexer.spinFor(reverse,95.0,degrees);
      }

      if (Controller1.ButtonB.pressing()) {
        // controls flywheel activation and allows for consistent flywheel rpm - flywheel pid
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
      
      if (Controller1.ButtonL1.pressing() && !intakeStopped && runByForward) {
      // toggles intake/roller activation and direction
        intakeStopped = true;
        intakeIn = true;
        curCommand = 1;
      }
      else if (Controller1.ButtonL1.pressing()) {
        intakeStopped = false;
        intakeIn = true;
        runByForward = true;
        curCommand = 2;
      }
      else if (Controller1.ButtonL2.pressing() && !intakeStopped && !runByForward) {
        intakeStopped = true;
        intakeIn = false;
        curCommand = 3;
      }
      else if (Controller1.ButtonL2.pressing()) {
        intakeStopped = false;
        intakeIn = false;
        runByForward = false;
        curCommand = 4;
      }
      if (!intakeStopped && !intakeIn) {
        intakeroller.spin(reverse);
      }
      else if (!intakeStopped && intakeIn) {
        intakeroller.spin(forward);
      }
      else if (intakeStopped) {
        intakeroller.stop();
      }
      if (curCommand != lastCommand && !firstIter) {
        wait(200, msec);
      }
      firstIter = false;
      lastCommand = curCommand;

      // prints encoder information - track robot's position
      std::cout << "LEncoder: " << LEncoder.position(turns) << std::endl;
      std::cout << "REncoder: " << REncoder.position(turns) << std::endl;
      std::cout << "FEncoder: " << FEncoder.position(turns) << std::endl;

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