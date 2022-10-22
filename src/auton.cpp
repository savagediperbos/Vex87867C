#include <robot-config.h>
#include <iostream>

// Drivetrain PID Configuration
double kP = 0.018; // proportional component - improve rise and settling time
double kI = 0.013; // integral component - corrects undershooting
double kD = 0.0018; // derivative component - speeds up and slows down (dampener)
double integralBound = 100; // only calculate total lateral error within the bound
double turnkP = 0.32;
double turnkI = 0.24;
double turnkD = 0.023;
double turnBound = 15; // only calculate total turn error within the bound

// DO NOT TOUCH
double desiredValue;
double desiredTurnValue;
bool enableDrivePID = true;

int drivePID() { // PROGRAM TO CALCULATE FIRST ITERATION LASTERROR RESET
  double averagePosition;
  double lateralPower;
  double turnPower;
  double adjAngle;
  double error; // sensorvalue (current) - desiredValue : positional value -> speed -> acc
  double prevError = desiredValue; // position 20 miliseconds ago
  double totalError = 0; // totalError = totalError + error (keeps adding error/ covenrts position into absement)
  double turnError; // sensorValue - desiredValue : position
  double turnPrevError = desiredTurnValue; // position 20 miliseconds ago
  double turnTotalError = 0; // totalError = totalError + error
  double lateralDeriv;
  double turnDeriv;
  while (enableDrivePID) {
    averagePosition = -0.5 * (LEncoder.position(deg) + REncoder.position(deg));
    if (abs(Inertial10.angle(deg)) <= 1) {
      adjAngle = 0;
    }
    if (Inertial10.angle(deg) <= 180) {
      adjAngle = Inertial10.angle(deg);
    }
    else if (Inertial10.angle(deg) > 180) {
      adjAngle = -1 * (360 - Inertial10.angle(deg));
    }
    
    error = desiredValue - averagePosition;
    if (abs(error) < integralBound && averagePosition < desiredValue) {
      totalError += error * 0.01; 
    }
    else {
      totalError = 0; 
    }
    lateralDeriv = (error - prevError) * 100;
    
    turnError = desiredTurnValue - adjAngle;
    if (abs(turnError) < turnBound) {
      turnTotalError += turnError * 0.01;
    }
    else {
      turnTotalError = 0;
    }
    turnDeriv = (turnError - turnPrevError) * 100;
    
    lateralPower = (error * kP) + (totalError * kI) + (lateralDeriv * kD);
    turnPower = (turnError * turnkP) + (turnTotalError * turnkI) + (turnDeriv * turnkD);

    FL.spin(forward, lateralPower + turnPower, volt);
    FR.spin(forward, lateralPower - turnPower, volt);
    BL.spin(forward, lateralPower + turnPower, volt);
    BR.spin(forward, lateralPower - turnPower, volt);
    
    //std::cout << adjAngle << std::endl;
    //std::cout << averagePosition << std::endl;
    prevError = error;
    turnPrevError = turnError;
    wait(10, msec);
  }
  return 0;
}

void resetSensors() {
  void suspend(int drivePID);
  desiredValue = 0;
  desiredTurnValue = 0;
  LEncoder.setPosition(0, deg);
  REncoder.setPosition(0, deg);
  Inertial10.setHeading(0, deg);
  void resume(int drivePID);
}

void move(double moving, int waitTime) {
  resetSensors();
  desiredValue = moving;
  wait(waitTime, msec);
}

void turnLR(double turning, int waitTime) {
  resetSensors();
  desiredTurnValue = turning;
  wait(waitTime, msec);
}

double fKV = 0.017;
double fKP = 0.008;
double RPM = 0;
int runFlywheelAt() {
  while (true) {
    double fVelo = -1 * flywheel.velocity(rpm);
    double fError = RPM - fVelo;
    if (fVelo < RPM - 80) {
      flywheels.spin(reverse, 12, volt);
    }
    else {
      flywheels.spin(reverse, RPM * fKV + fError * kP, volt);
    }
    wait(10,msec);
  }
}

void runFlywheel(double rpm) {
  RPM = rpm;
  task::resume(runFlywheelAt);
}

void stopFlywheel() {
  task::suspend(runFlywheelAt);
  flywheels.stop();
}

void indexerShoot(double shots, int indTime) {
  for (int i = 0; i < shots; i++) {
    indexer.spin(reverse, 10, volt);
    wait(95,msec);
    indexer.spin(forward, 10, volt);
    wait(95,msec);
    indexer.stop();
    if (shots - 1 != i) {
      wait(indTime,msec);
    }
  }
}

int autonIntake() {
  while (true) {
    intakeroller.spin(forward, 10, volt);
  }
}

void runIntake() {
  task::resume(autonIntake);
}

void stopIntake() {
  task::suspend(autonIntake);
  intakeroller.stop();
}

int autonRoller() {
  while (true) {
    intakeroller.spin(reverse, 12, volt);
  }
}

void runRoller() {
  task::resume(autonRoller);
}

void stopRoller() {
  task::suspend(autonRoller);
  intakeroller.stop();
}

void auton() { //fullfield: 5100
  bool runComp1 = false; //3 stack side
  bool runComp2 = true; //middle start side
  bool runSkills = false;

  task aut_drivePID(drivePID);
  if (runComp1) {
    move(160, 300);
    runRoller();
    wait(200,msec);
    stopRoller();
    move(-140, 300);
    turnLR(-135, 800);
    move(1200, 700);
    runIntake();
    wait(200,msec);
    move(100, 300);
    move(-250, 400);
    runFlywheel(593);
    turnLR(-77, 600);
    move(150, 400);
    resetSensors(); //stop
    wait(1000,msec);
    indexerShoot(3, 700);
    wait(300,msec);
    stopFlywheel();
    move(-150, 400);
    turnLR(77, 600);
    move(1000, 1000);
    runFlywheel(583);
    turnLR(-86, 700);
    move(300, 400);
    resetSensors(); //stop
    wait(1000,msec);
    indexerShoot(2, 700);
    task::suspend(aut_drivePID);
    stopIntake();
    wait(400,msec);
    stopFlywheel();
  }

  if (runComp2) {
    runFlywheel(386);
    move(700, 800);
    runIntake();
    turnLR(26, 600);
    move(400, 700);
    move(-200, 200);
    move(200, 200);
    wait(1200,msec);
    indexerShoot(2, 800);
    wait(200,msec);
    turnLR(-71, 700);
    move(1400, 1000);
    wait(500,msec);
    turnLR(94, 1000);
    move(100, 300);
    indexerShoot(2, 800);
    wait(200,msec);
    stopFlywheel();
    stopIntake();
    turnLR(88, 800);
    move(2975, 1800);
    turnLR(45, 600);
    runRoller();
    move(400, 400);
    wait(110,msec);
    task::suspend(aut_drivePID);
    stopRoller();
  }

  if (runSkills) {
    runRoller();
    move(200, 200);
    wait(100,msec);
    move(-250, 200);
    stopRoller();
    runIntake();
    turnLR(135, 1000);
    move(700, 1000);
    turnLR(-47, 800);
    move(650, 1000);
    stopIntake();
    runRoller();
    wait(100,msec);
    stopRoller();
    move(-500, 800);
    runFlywheel(360);
    turnLR(88, 1000);
    move(2000, 1600);
    wait(500,msec);
    indexerShoot(4, 150);
    wait(300,msec);
    stopFlywheel();
    move(-1400, 1600);
    turnLR(47, 800);
    runIntake();
    runFlywheel(360);
    move(1200, 1000);
    move(1200, 1000);
    turnLR(-90, 1000);
    move(400, 600);
    stopIntake();
    indexerShoot(4, 150);
    wait(300,msec);
    stopFlywheel();
    move(-400, 600);
    runIntake();
    turnLR(135, 1200);
    move(750, 800);
    turnLR(-47, 800);
    move(725, 1000);
    turnLR(-47, 800);
    move(1100, 1100);
    turnLR(88, 1000);
    move(800, 800);
    turnLR(-88, 1000);
    move(800, 800);
    stopIntake();
    runRoller(); //roller
    move(150, 200);
    move(-150, 200);
    stopRoller();
    move(-900, 1000);
    turnLR(88, 1000);
    move(800, 800);
    runRoller();
    move(150, 200);
    move(-500, 200);
    stopRoller();
    turnLR(88, 1000);
    runFlywheel(360);
    move(2000, 2000);
    wait(500,msec);
    indexerShoot(4, 150);
    wait(300,msec);
    move(-2000, 2000);
    turnLR(-135, 1000);
    expansion = true;
    /*
    move(-1650, 2000);
    turnLR(47, 800);
    runIntake();
    move(1200, 1000);
    move(1200, 1200);
    turnLR(-88, 1000);
    move(200, 400);
    stopIntake();
    indexerShoot(4, 150);
    wait(300,msec);
    stopFlywheel();
    runIntake();
    move(-200, 400);
    turnLR(88, 1000);
    move(1250, 1000);
    move(1250, 1000);
    runFlywheel(380);
    stopIntake();
    turnLR(-135, 1200);
    move(1000, 1000);
    indexerShoot(4, 150);
    wait(300,msec);
    stopFlywheel();
    move(-1500, 1200);
    turnLR(-47, 800);
    task::suspend(aut_drivePID);
    expansion = true;
    */
  }

  while (true) {
    wait(100, msec);
  }
}