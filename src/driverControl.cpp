#include <robot-config.h>
#include <iostream>

int data() {
  while(true) {
    //std::cout << indexer.temperature(percent) << std::endl;
    //std::cout << flywheel.velocity(rpm) << std::endl;
    //std::cout << flywheel2.velocity(rpm) << std::endl;
    std::cout << FL.temperature(percent) << std::endl;
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
    wait(25, msec);
    }
  }
}

bool onoff = false;
int flywheelToggle() {
  while (true) {
    if (Controller1.ButtonB.pressing()) {
      if (!onoff) {
        onoff = true;
        while (Controller1.ButtonB.pressing()) wait(10,msec);
      }
      else if (onoff) {
        onoff = false;
        while (Controller1.ButtonB.pressing()) wait(10,msec);
      }
    }
  }
}

int flywheelSpin() {
  double desiredRPM = 375;
  double kV = 0.017;
  double kP = 0.008;

  while (true) {
    if (onoff) {
      double fVelo = -1 * flywheel.velocity(rpm);
      double fError = desiredRPM - fVelo;
      if (fVelo < desiredRPM - 80) {
        flywheels.spin(reverse, 12, volt);
      }
      else {
        flywheels.spin(reverse, desiredRPM * kV + fError * kP, volt);
      }
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
  bool stopped = true;
  bool io = false;
  while (true) {
    if (Controller1.ButtonL1.pressing()) {
      stopped = (stopped == true) ? false : true;
      if (stopped == false) {
        io = true;
        intakeroller.spin(forward, 10, volt);
        while (Controller1.ButtonL1.pressing()) wait(10,msec); //move to spot
      }
      else {
        io = false;
        intakeroller.stop();
        while (Controller1.ButtonL1.pressing()) wait(10,msec);
      }
      //move here, use 1
    }
    else if (Controller1.ButtonL2.pressing() && stopped == true) {
      stopped = false;
      io = false;
      intakeroller.spin(reverse, 12, volt);
    }
    else if (!Controller1.ButtonL2.pressing() && stopped == false && io == false) {
      stopped = true;
      intakeroller.stop();
    }
    wait(10, msec);
  }
}

void driverControl() {
  task t_chassis(chassis);
  task t_indexerArm(indexerArm);
  task t_flywheelToggle(flywheelToggle);
  task t_flywheelSpin(flywheelSpin);
  task t_expand(expand);
  task t_intake(intake);
  //task t_data(data);
}