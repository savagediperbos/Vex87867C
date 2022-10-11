#include <vex.h>
#include <robot-config.h>

// Configuration
double maxRPM = 450.0; // the assigned maximum value rpm for the flywheel, default value
double minRPM = 250.0; // the assigned minimum value rpm for the flywheel, activated by using the toggle
double fD = 150.0; // distance from desired rpm at which flywheel starts decreasing in rpm

// Status tracker
double desiredRPM = maxRPM; // DO NOT TOUCH - rpm at which the flywheel is designated to spin, maximum rpm

int turnControls() {
  while (true) {
    FR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
    BR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
    FL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
    BL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
  }
}

int indexerShoot() {
  while (true) {
    if (Controller1.ButtonR1.pressing()) {
      // activates indexer and shoots one time
      indexer.spinFor(forward,95.0,degrees);
      indexer.spinFor(reverse,96.0,degrees);
    }
  }
}

int flywheelSpin() {
  double limiter = 0.0; // DO NOT TOUCH - limiter/value subtracted from flywheel rpm
  bool flywheelStopped = true; // DO NOT TOUCH - keeps track of flywheel motion status
  while (true) {
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
  }
}

int flywheelToggle() {
  bool flywheelVelocityMax = true; // DO NOT TOUCH - keeps track of flywheel desired rpm for rpm toggle
  while (true) {
    if (Controller1.ButtonY.pressing() && !Controller1.ButtonB.pressing()) {
      // toggles flywheel velocity between max rpm (default) and minimum rpm
      if (flywheelVelocityMax) {
        desiredRPM = minRPM;
        flywheelVelocityMax = false;
        wait(200, msec);
      }
      else if (!flywheelVelocityMax) {
        desiredRPM = maxRPM;
        flywheelVelocityMax = true;
        wait(200, msec);
      }
    }
  }
}

int intakeToggle() {
  bool intakeStopped = true; //DO NOT TOUCH - keeps track of intake motion status
  bool runByForward; //DO NOT TOUCH - keeps track of intake activation status
  bool intakeIn; //DO NOT TOUCH - keeps track of intake spin direction
  bool firstIter = true; //DO NOT TOUCH - protects first iteration from intake wait time quota
  int curCommand; //DO NOT TOUCH - keeps track of intake function ran
  int lastCommand; //DO NOT TOUCH - keeps track of previous intake function ran
  while (true) {
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
  }
}

void driverControl(void) {
  task rc_turnControls(turnControls);
  task rc_indexerShoot(indexerShoot);
  task rc_flywheelSpin(flywheelSpin);
  task rc_flywheelToggle(flywheelToggle);
  task rc_intakeToggle(intakeToggle);
}