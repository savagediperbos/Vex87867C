#include <robot-config.h>
#include <iostream>

int data() {
  while(true) {
    //std::cout << indexer.temperature(percent) << std::endl;
    //std::cout << flywheel.velocity(rpm) << std::endl;
    //std::cout << flywheel2.velocity(rpm) << std::endl;
    //std::cout << FL.temperature(percent) << std::endl;
    std::cout << flywheel.temperature(percent) << std::endl;
    std::cout << std::endl;
    wait(100,msec);
  }
}

int chassis() {
  while (true) {
    FL.spin(forward, (Controller1.Axis3.value() + Controller1.Axis1.value() + Controller1.Axis4.value()) * 12 / 127, volt);
    FR.spin(forward, (Controller1.Axis3.value() - Controller1.Axis1.value() - Controller1.Axis4.value()) * 12 / 127, volt);
    BL.spin(forward, (Controller1.Axis3.value() + Controller1.Axis1.value() - Controller1.Axis4.value()) * 12 / 127, volt);
    BR.spin(forward, (Controller1.Axis3.value() - Controller1.Axis1.value() + Controller1.Axis4.value()) * 12 / 127, volt);
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
      wait(10, msec);
    wait(10, msec);
    }
  }
}

bool onoff = false;
int flywheelToggle() {
  while (true) {
    if (Controller1.ButtonB.pressing()) {
      if (!onoff) {
        onoff = true;
        wait(200,msec);
      }
      else if (onoff) {
        onoff = false;
        wait(200,msec);
      }
    }
  }
}

//double maxRPM = 370.0;
double minRPM = 280.0;
double desiredRPM = minRPM;

int flywheelSpin() {
  double spinPower;
  while (true) {
    if (Controller1.ButtonB.pressing()){
      spinPower = (abs(flywheel.velocity(rpm)) <= desiredRPM) ? 12 : 7;
      flywheel.spin(reverse, spinPower, volt);
      flywheel2.spin(reverse, spinPower, volt);
      wait(10,msec);
    }
    else {
      flywheel.stop();
      flywheel2.stop();
      wait(10,msec);
    }
  }
}
/*
int flywheelSpin() {
  double spinPower;
  while (true) {
    if (onoff) {
      spinPower = (abs(flywheel.velocity(rpm)) <= desiredRPM) ? 12 : 7.3;
      flywheel.spin(reverse, spinPower, volt);
      flywheel2.spin(reverse, spinPower, volt);
      wait(10,msec);
    }
    else {
      flywheel.stop();
      flywheel2.stop();
    }
  }
}
*/

int expand() {
  while (true) {
    if (Controller1.ButtonX.pressing()) {
      expansion = true;
      wait(10000,msec);
      expansion = false;
    }
  }
}

/*
bool flywheelVelocityMax = false; // DO NOT TOUCH - keeps track of flywheel desired rpm for rpm toggle
    
int flywheelToggle() {
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
      wait(10,msec);
    }
  }
}

int flywheelToggle() {
  while (true) {
    if (Controller1.ButtonY.pressing()) {
      desiredRPM = (desiredRPM == minRPM) ? desiredRPM = maxRPM : desiredRPM = minRPM;
      wait(200,msec);
    }
    wait(10,msec);
  }
}
*/

int intake() {
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
      intakeroller.spin(reverse, 12, volt);
    }
    else if (!intakeStopped && intakeIn) {
      intakeroller.spin(forward, 10, volt);
    }
    else if (intakeStopped) {
      intakeroller.stop();
    }
    if (curCommand != lastCommand && !firstIter) {
      wait(200, msec);
    }
    firstIter = false;
    lastCommand = curCommand;
    wait(10,msec);
  }
}

void driverControl() {
  task t_chassis(chassis);
  task t_indexerArm(indexerArm);
  //task t_flywheelToggle(flywheelToggle);
  task t_flywheelSpin(flywheelSpin);
  task t_intake(intake);
  task t_expand(expand);

  task t_data(data);
}