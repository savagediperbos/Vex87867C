#include <vex.h>
#include <robot-config.h>
#include <iostream>

using namespace vex;
competition Competition;

bool runAuton = false; //config to run auton or not
bool runUsercontrol = true; //config to run auton or not

int encoderValues() {
  while (true) {
    //prints left shaft encoder data
    std::cout << "LEncoder: " << LEncoder.position(degrees) << std::endl;
    //prints right shaft encoder data
    std::cout << "REncoder: " << REncoder.position(degrees) << std::endl;
    //prints front shaft encoder data
    std::cout << "FEncoder: " << FEncoder.position(degrees) << std::endl;
  }
}

void pre_auton() {
  // initial config
  intakeroller.setVelocity(100,percent);
  flywheel.setVelocity(100,percent);
  indexer.setVelocity(100,percent);
  indexer.setStopping(hold);
}

void aut() {
  if (runAuton) {
    auton();
  }
}

void usercontrol() {
  if (runUsercontrol) {
    driverControl();
  }
}

int main() {
  task printEncoders(encoderValues);
  
  pre_auton();
  aut();
  Competition.drivercontrol(usercontrol);

  while (true) {
    wait(20, msec);
  }
}