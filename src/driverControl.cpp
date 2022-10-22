#include <robot-config.h>
#include <iostream>

int data() {
  while(true) {
    //std::cout << indexer.temperature(percent) << std::endl;
    //std::cout << flywheel.velocity(rpm) << std::endl;
    //std::cout << flywheel2.velocity(rpm) << std::endl;
    //std::cout << FL.temperature(percent) << std::endl;
    //std::cout << Controller1.Axis4.value() << std::endl;
    //std::cout << flywheel.voltage(volt) << std::endl;
    std::cout << std::endl;
    wait(200,msec);
  }
}

int chassis() {
  while (true) {
    FL.spin(forward, Controller1.Axis3.value() * 12 / 127, volt);
    FR.spin(forward, Controller1.Axis2.value() * 12 / 127, volt);
    BL.spin(forward, Controller1.Axis3.value() * 12 / 127, volt);
    BR.spin(forward, Controller1.Axis2.value() * 12 / 127, volt);
    wait(10,msec);
  }
}

int indexerArm() {
  while (true) {
    if (Controller1.ButtonR1.pressing()) {
      indexer.spin(reverse, 10, volt);
      wait(95, msec);
      indexer.spin(forward, 10, volt);
      wait(95, msec);
      indexer.stop();
    wait(30, msec);
    }
  }
}

int flywheelSpin() {
  double output;

  double desiredRPM = 375;
  double kV = 0.017;
  double kP = 0.008;
  int bound = 80;

  while (true) {
    if (Controller1.ButtonB.pressing()) {
      double fVelo = -1 * flywheel.velocity(rpm);
      double fError = desiredRPM - fVelo;
      if (fError > bound) {
        output = 12;
      }
      else if (fError < -bound) {
        output = 0;
      }
      else {
        output = desiredRPM * kV + fError * kP;
      }
      flywheels.spin(reverse, output, volt);
    }
    else {
      flywheels.stop();
    }
    wait(10,msec);
  }
}

int expand() {
  while (true) {
    if (Controller1.ButtonX.pressing()) {
      expansion = true;
    }
  }
}

int intake() {
  bool ri = false;
  bool ro = false;
  while (true) {
    if (Controller1.ButtonL1.pressing()) {
      ri = (ri == false) ? true : false;
      if (ri == true) {
        ro = false;
        intakeroller.spin(forward, 10, volt);
      }
      else {
        ri = false;
        intakeroller.stop();
      }
      while (Controller1.ButtonL1.pressing()) wait(10,msec);
    }
    else if (Controller1.ButtonL2.pressing()) {
      ro = (ro == false) ? true : false;
      if (ro == true) {
        ri = false;
        intakeroller.spin(reverse, 12, volt);
      }
      else {
        ro = false;
        intakeroller.stop();
      }
      while (Controller1.ButtonL2.pressing()) wait(10,msec);
    }
    wait(10, msec);
  }
}

void driverControl() {
  task t_chassis(chassis);
  task t_indexerArm(indexerArm);
  task t_flywheelSpin(flywheelSpin);
  task t_expand(expand);
  task t_intake(intake);
  //task t_data(data);
}