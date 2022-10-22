#include <robot-config.h>
#include <iostream>

int data() {
  while(true) {
    //std::cout << flywheel.velocity(rpm) << std::endl;
    //std::cout << FL.temperature(percent) << std::endl;
    //std::cout << flywheel.voltage(volt) << std::endl;
    std::cout << std::endl;
    wait(200,msec);
  }
}

int chassis() {
  while (true) {
    /*
    if (Controller1.Axis2.value() == 0 && Controller1.Axis3.value() == 0)  {
      FL.setStopping(brake);
      FR.setStopping(brake);
      BL.setStopping(brake);
      BR.setStopping(brake);
    }
    else {
      FL.setStopping(coast);
      FR.setStopping(coast);
      BL.setStopping(coast);
      BR.setStopping(coast);
    }
    */
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
      indexer.spin(reverse, 12, volt);
      wait(90, msec);
      indexer.spin(forward, 12, volt);
      wait(90, msec);
      indexer.stop();
      wait(20, msec);
    }
    wait(10, msec);
  }
}

int flywheelSpin() {
  double output;
  double fVelo;
  double fError;
  double tError;
  
  double desiredRPM = 355;
  double kV = 0.017;
  double kP = 0.008;
  double kI = 0;
  int bound = 80;

  while (true) {
    fVelo = -1 * flywheel.velocity(rpm);
    fError = desiredRPM - fVelo;
    tError = (fError < bound && fError > -bound) ? tError + fError : 0;
    
    if (Controller1.ButtonB.pressing()) {
      if (fError > bound) output = 12;
      else if (fError < -bound) output = 0;
      else output = desiredRPM * kV + fError * kP + tError * kI;
      
      flywheels.spin(reverse, output, volt);
    }
    else flywheels.stop();
    
    wait(10, msec);
  }
}

int expand() {
  while (true) if (Controller1.ButtonX.pressing()) expansion = true;
}

int intake() {
  bool ri = false;
  bool ro = false;
  while (true) {
    if (Controller1.ButtonL1.pressing()) {
      ri = (ri == false) ? true : false;
      ro = (ri == true) ? false : ro;
      (ri == true) ? intakeroller.spin(forward, 10, volt) : intakeroller.stop();
      while (Controller1.ButtonL1.pressing()) wait(10,msec);
    }
    else if (Controller1.ButtonL2.pressing()) {
      ro = (ro == false) ? true : false;
      ri = (ro == true) ? false : ri;
      (ro == true) ? intakeroller.spin(reverse, 12, volt) : intakeroller.stop();
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