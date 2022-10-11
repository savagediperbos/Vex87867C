#include <vex.h>
#include <robot-config.h>

// Configuration
double maxRPM = 400.0; // the assigned maximum value rpm for the flywheel, default value
double minRPM = 325.0; // the assigned minimum value rpm for the flywheel, activated by using the toggle
double fD = 100.0; // distance from desired rpm at which flywheel starts decreasing in rpm
double desiredRPM = minRPM; // rpm value at which flywheel is first configured at (min/max)

int frontLeft() {
  // keeps track of controller commands for front left wheel
  while (true) {
    FL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  }
}

int frontRight() {
  // keeps track of controller commands for front right wheel
  while (true) {
    FR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
  }
}

int backLeft() {
  // keeps track of controller commands for back left wheel
  while (true) {
    BL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
  }
}

int backRight() {
  // keeps track of controller commands for back right wheel
  while (true) {
    BR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  }
}

int indexerShoot() {
  // activates indexer and shoots one time
  while (true) {
    if (Controller1.ButtonR1.pressing()) {
      indexer.spinFor(-95.0,degrees,200.0,rpm);
      indexer.spinFor(96.0,degrees,200.0,rpm);
    }
  }
}

int flywheelSpin() {
  // controls flywheel activation and allows for consistent flywheel rpm
  double limiter = 0.0; // DO NOT TOUCH - limiter/value subtracted from flywheel rpm
  bool flywheelStopped = true; // DO NOT TOUCH - keeps track of flywheel motion status
  while (true) {
    if (Controller1.ButtonB.pressing()) {
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
  // toggles flywheel velocity between max rpm (default) and minimum rpm
  bool flywheelVelocityMax = false; // DO NOT TOUCH - keeps track of flywheel desired rpm for rpm toggle
  while (true) {
    if (Controller1.ButtonY.pressing()) {
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
  // toggles intake/roller activation and direction
  bool intakeStopped = true; //DO NOT TOUCH - keeps track of intake motion status
  bool runByForward; //DO NOT TOUCH - keeps track of intake activation status
  bool intakeIn; //DO NOT TOUCH - keeps track of intake spin direction
  bool firstIter = true; //DO NOT TOUCH - protects first iteration from intake wait time quota
  int curCommand; //DO NOT TOUCH - keeps track of intake function ran
  int lastCommand; //DO NOT TOUCH - keeps track of previous intake function ran
  while (true) {
    if (Controller1.ButtonL1.pressing() && !intakeStopped && runByForward) {
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

void driverControl() {
  // coast allows for more fluid driving
  FL.setStopping(coast);
  FR.setStopping(coast);
  BL.setStopping(coast);
  BR.setStopping(coast);

  task rc_frontLeft(frontLeft);
  task rc_frontRight(frontRight);
  task rc_backLeft(backLeft);
  task rc_backRight(backRight);
  task rc_indexerShoot(indexerShoot);
  task rc_flywheelSpin(flywheelSpin);
  task rc_flywheelToggle(flywheelToggle);
  task rc_intakeToggle(intakeToggle);
}