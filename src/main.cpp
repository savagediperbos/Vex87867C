#include <vex.h>
#include <robot-config.h>
#include <iostream>

using namespace vex;
competition Competition;

int drivetrainSpeed = 75; // percentage of velocity drivetrain runs at

bool runAuton = false; // config to run auton or not
bool runUsercontrol = true; // config to run auton or not
bool printSensorValues = false; // config to print sensor values or not

int sensorValues() {
  while (true) {
    if (printSensorValues) {
      // prints left shaft encoder data
      std::cout << LEncoder.position(degrees) << std::endl;
      // prints right shaft encoder data
      std::cout << REncoder.position(degrees) << std::endl << std::endl;

      wait(300,msec);
    }
  }
}

void pre_auton() {
  // initial config
  FL.setVelocity(drivetrainSpeed,percent);
  FR.setVelocity(drivetrainSpeed,percent);
  BL.setVelocity(drivetrainSpeed,percent);
  BR.setVelocity(drivetrainSpeed,percent);
  intakeroller.setVelocity(100,percent);
  flywheel.setVelocity(100,percent);
  indexer.setVelocity(100,percent);

  indexer.setStopping(hold);
}

void aut() {
  if (runAuton) {
    FL.setStopping(brake);
    FR.setStopping(brake);
    BL.setStopping(brake);
    BR.setStopping(brake);
    auton();
  }
}

void usercontrol() {
  if (runUsercontrol) {
    FL.setStopping(coast);
    FR.setStopping(coast);
    BL.setStopping(coast);
    BR.setStopping(coast);
    driverControl();
  }
}

int main() {
  //task sensorData(sensorValues);
  pre_auton();
  aut();
  Competition.drivercontrol(usercontrol);

  while (true) {
    wait(20, msec);
  }
}