#include <vex.h>
#include <robot-config.h>
#include <iostream>

// Flywheel configuration
double maxRPM = 350.0; // higher rpm value in toggle
double minRPM = 270.0; // lower rpm value in toggle
double fD = 150.0; // flywheel distance: rpm begins decrease at desiredRPM-fD
double aF = 500.0; // acceleration factor: initial rpm addon; decreases falloff
double fP = 2.0; // falloff proportion: increases falloff

// Global tracker
double desiredRPM = minRPM; // DO NOT TOUCH - currently set flywheel rpm

int frontLeft() {
  while (true) {
    FL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  }
}

int frontRight() {
  while (true) {
    FR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
  }
}

int backLeft() {
  while (true) {
    BL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
  }
}

int backRight() {
  while (true) {
    BR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  }
}

int indexerShoot() {
  while (true) {
    if (Controller1.ButtonR1.pressing()) {
      // activates indexer and shoots one time
      indexer.spinFor(-95.0,degrees,200.0,rpm);
      indexer.spinFor(96.0,degrees,200.0,rpm);
    }
  }
}

int flywheelSpin() {
  double limiter = 0; // DO NOT TOUCH - limiter/value subtracted from flywheel rpm
  bool flywheelStopped = true; // DO NOT TOUCH - keeps track of flywheel motion status
  while (true) {
    if (Controller1.ButtonB.pressing()) {
      // controls flywheel activation and allows for consistent flywheel rpm - flywheel pid
      if (fabs(flywheel.velocity(rpm)) > (desiredRPM - fD)) {
        limiter = fabs(flywheel.velocity(rpm)) + fD - desiredRPM;
      }
      flywheel.spin(reverse, desiredRPM + (aF / pow(sqrt(2.0), sqrt(limiter * fP))), rpm);
      flywheel2.spin(reverse, desiredRPM + (aF / pow(sqrt(2.0), sqrt(limiter * fP))), rpm);
      flywheelStopped = false;
    }
    else if (!flywheelStopped) {
      flywheel.stop();
      flywheel2.stop();
      flywheelStopped = true;
    }
  }
}

int flywheelToggle() {
  bool flywheelVelocityMax = false; // DO NOT TOUCH - keeps track of flywheel desired rpm for rpm toggle
  while (true) {
    if (Controller1.ButtonY.pressing()) {
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
  task rc_frontLeft(frontLeft);
  task rc_frontRight(frontRight);
  task rc_backLeft(backLeft);
  task rc_backRight(backRight);
  task rc_indexerShoot(indexerShoot);
  task rc_flywheelSpin(flywheelSpin);
  task rc_flywheelToggle(flywheelToggle);
  task rc_intakeToggle(intakeToggle);
}