#include <vex.h>
#include <robot-config.h>
#include <iostream>

// Flywheel Configuration
double maxRPM = 400.0; // higher rpm value in toggle
double minRPM = 300.0; // lower rpm value in toggle
double kp = 0.25;
double ki = 0.01;
double kd = 0.1;
double integralStart = 10; // rpm from desired when integral activates
double desiredRPM = maxRPM;

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
      indexer.spinFor(-51.0, degrees, 200, rpm);
      indexer.spinFor(51.0, degrees, 200, rpm);
    }
  }
}

int flywheelSpin() {
  double error = 0;
  double prevError = 0;
  double totalError = 0;
  double spinPower = 0;
  bool flywheelStopped = true; // keeps track of flywheel motion status
  while (true) {
    if (Controller1.ButtonB.pressing()) {
      error = desiredRPM - (-1 * flywheel.velocity(rpm));
      if (error != 0 && abs(error) < integralStart) {
        totalError += error;
      }
      else {
        totalError = 0;
      }
      spinPower = (error * kp) + (totalError * ki) + ((error - prevError) * kd);
      if (spinPower < 0) {
        spinPower = 0;
      }
      flywheel.spin(reverse, spinPower, volt);
      flywheel2.spin(reverse, spinPower, volt);
      prevError = error;
      flywheelStopped = false;
    }
    else if (!flywheelStopped) {
      flywheel.stop();
      flywheel2.stop();
      error = 0;
      prevError = 0;
      totalError = 0;
      spinPower = 0;
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
  bool intakeStopped = true; // DO NOT TOUCH - keeps track of intake motion status
  bool runByForward; // DO NOT TOUCH - keeps track of intake activation status
  bool intakeIn; // DO NOT TOUCH - keeps track of intake spin direction
  bool firstIter = true; // DO NOT TOUCH - protects first iteration from intake wait time quota
  int curCommand; // DO NOT TOUCH - keeps track of intake function ran
  int lastCommand; // DO NOT TOUCH - keeps track of previous intake function ran
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