#include <robot-config.h>
#include <iostream>

using namespace vex;
competition Competition;

bool runAuton = false; // config to run auton or not
bool runUsercontrol = true; // config to run auton or not

void pre_auton() {
  if (runAuton) {
    while (Inertial10.isCalibrating()) {
      wait(20, msec);
    }
  }
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
  pre_auton();
  aut();
  Competition.drivercontrol(usercontrol);
  while (true) {
    wait(20, msec);
  }
}