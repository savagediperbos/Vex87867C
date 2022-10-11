#include <robot-config.h>
#include <iostream>

// Drivetrain PID Configuration
double kP = 0.016; // proportional component - improve rise and settling time
double kI = 0.001; // integral component - corrects undershooting
double kD = 0.124; // derivative component - speeds up and slows down (dampener)
double integralBound = 15; // only calculate total lateral error within the bound
double turnkP = 0.34;
double turnkI = 0.006;
double turnkD = 3.75;
double turnBound = 5; // only calculate total turn error within the bound

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
    if (Inertial10.angle(deg) <= 180) {
      adjAngle = Inertial10.angle(deg);
    }
    else if (Inertial10.angle(deg) > 180) {
      adjAngle = -1 * (360 - Inertial10.angle(deg));
    }
    
    error = desiredValue - averagePosition;
    if (abs(error) < integralBound) {
      totalError += error; 
    }
    else {
      totalError = 0; 
    }
    lateralDeriv = error - prevError;
    
    turnError = desiredTurnValue - adjAngle;
    if (abs(turnError) < turnBound) {
      turnTotalError += turnError;
    }
    else {
      turnTotalError = 0;
    }
    turnDeriv = turnError - turnPrevError;
    
    lateralPower = (error * kP) + (totalError * kI) + (lateralDeriv * kD);
    turnPower = (turnError * turnkP) + (turnTotalError * turnkI) + (turnDeriv * turnkD);

    FL.spin(forward, lateralPower + turnPower, volt);
    FR.spin(forward, lateralPower - turnPower, volt);
    BL.spin(forward, lateralPower + turnPower, volt);
    BR.spin(forward, lateralPower - turnPower, volt);
    
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

double RPM;
double lowPower;
int runFlywheelAt() {
  double spinPower;
  while (true) {
    spinPower = (abs(flywheel.velocity(rpm)) <= RPM) ? 12 : lowPower;
    flywheel.spin(reverse, spinPower, volt);
    flywheel2.spin(reverse, spinPower, volt);
    wait(10,msec);
  }
}

void runFlywheel(double rpm, double adj) {
  RPM = rpm;
  lowPower = adj;
  task::resume(runFlywheelAt);
}

void stopFlywheel() {
  task::suspend(runFlywheelAt);
  flywheel.stop();
  flywheel2.stop();
}

void indexerShoot(double shots, int indTime) {
  for (int i = 0; i < shots; i++) {
    indexer.spin(reverse, 10, volt);
    wait(90,msec);
    indexer.spin(forward, 10, volt);
    wait(90,msec);
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
  bool runComp2 = false; //middle start side
  bool runSkills = true;

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
    runFlywheel(593, 6);
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
    runFlywheel(583, 6);
    turnLR(-86, 700);
    move(300, 400);
    resetSensors(); //stop
    wait(1000,msec);
    indexerShoot(2, 700);
    stopIntake();
    wait(400,msec);
    stopFlywheel();
  }

  if (runComp2) {
    move(900, 500);
    runIntake();
    wait(500,msec);
    move(200, 300);
    runFlywheel(610, 8);
    turnLR(30, 600);
    move(400, 400);
    resetSensors(); //stop
    wait(500,msec);
    indexerShoot(3, 1000);
    wait(300,msec);
    stopFlywheel();
    move(-400, 400);
    turnLR(-70, 800);
    move(1300, 800);
    runFlywheel(610, 8);
    wait(500,msec);
    turnLR(90, 800);
    move(300, 400);
    resetSensors(); //stop
    wait(500,msec);
    indexerShoot(2, 1000);
    wait(300,msec);
    stopFlywheel();
    stopIntake();
    turnLR(90, 800);
    move(2900, 1600);
    turnLR(45, 400);
    runRoller();
    move(200, 500);
    stopRoller();
  }

  if (runSkills) {
    runRoller();
    move(250, 200);
    wait(100,msec);
    move(-720, 800);
    stopRoller();
    runIntake();
    turnLR(97, 800);
    move(500, 1000);
    move(650, 1000);
    stopIntake();
    runRoller();
    move(150,200);
    wait(100,msec);
    move(-300, 800);
    stopRoller();
    runIntake();
    turnLR(97, 800);
    move(2150, 2000);
    stopIntake();
    runFlywheel(295, 7);
    resetSensors(); //stop
    wait(500,msec);
    indexerShoot(3, 200);
    wait(300,msec);
    stopFlywheel();
    move(-1400, 1400);
    turnLR(49, 800);
    runIntake();
    move(1400, 900);
    move(1400, 1500);
    turnLR(-97, 800);
    stopIntake();
    runFlywheel(295, 7);
    resetSensors(); //stop
    wait(500,msec);
    indexerShoot(3, 200);
    wait(300,msec);
    stopFlywheel();
  }

  while (true) {
    wait(100, msec);
  }
}