#include <robot-config.h>
#include <iostream>

using namespace vex;
competition Competition;

bool runAuton = false; // config to run auton or not
bool runUsercontrol = true; // config to run auton or not

void pre_auton() {
  while (Inertial10.isCalibrating()) {
    wait(20, msec);
  }
}
void aut() {
  if (runAuton) {
    FL.setStopping(hold);
    FR.setStopping(hold);
    BL.setStopping(hold);
    BR.setStopping(hold);
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
  //Competition.autonomous(aut);
  Competition.drivercontrol(usercontrol);
  //usercontrol();
  while (true) {
    wait(20, msec);
  }
}