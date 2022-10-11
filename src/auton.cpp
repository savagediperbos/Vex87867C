#include <vex.h>
#include <robot-config.h>
#include <iostream>

// Drivetrain PID Configuration
double kP = 0.011; // proportional component - improve rise and settling time
double kI = 0.002; // integral component - corrects undershooting
double kD = 0.06; // derivative component - speeds up and slows down (dampener)
double turnkP = 0.115;
double turnkI = 0.0048;
double turnkD = 0.2;

// integral caps
double maxIntegral = 300;
double maxTurnIntegral = 300;

double integralBound = 40; // If error is outside the bounds, then apply the integral. This is a buffer with +-integralBound degrees
double turnBound = 15;
double averagePosition;

// motor values
double lateralPower;
double turnPower;

// auton settings
double desiredValue;
double desiredTurnValue;

// PID calculations
double error; // sensorvalue (current) - desiredValue : positional value -> speed -> acc
double prevError = 0; // position 20 miliseconds ago
double totalError = 0; // totalError = totalError + error (keeps adding error/ covenrts position into absement)

double turnError; // sensorValue - desiredValue : position
double turnPrevError = 0; // position 20 miliseconds ago
double turnTotalError = 0; // totalError = totalError + error

// DO NOT TOUCH - bool for starting/ending auton
bool enableDrivePID = true;

double signnum_c(double x) { //evaluate later
  // checks values so total errors remain positive
  if (x > 0.0) {
    return 1.0;
  }
  if (x < 0.0) {
    return -1.0;
  }
  return x;
}

void resetEncoders() {
  void suspend(int drivePID);
  desiredValue = 0;
  desiredTurnValue = 0;
  LEncoder.setPosition(0,deg);
  REncoder.setPosition(0,deg);
  Inertial10.setRotation(0,deg); //try setHeading if doesnt work
  void resume(int drivePID);
}

void move(double moving) {
  desiredValue = moving;
  wait(500,msec);
  resetEncoders();
}

void turnL(double turning) {
  desiredTurnValue = turning;
  wait(2000,msec);
  resetEncoders();
}

int drivePID() {
  while (enableDrivePID) {
    averagePosition = (-1 * LEncoder.position(degrees)) + (-1 * REncoder.position(degrees));
    error = desiredValue - averagePosition;
    if (abs(error) < integralBound) {
      totalError += error; 
    }
    else {
      totalError = 0; 
    }
    totalError = abs(totalError) > maxIntegral ? signnum_c(totalError) * maxIntegral : totalError;
    lateralPower = (error * kP) + (totalError * kI) + ((error - prevError) * kD);
    
    turnError = desiredTurnValue - Inertial10.angle(degrees);
    if (abs(turnError) < turnBound) {
      turnTotalError += turnError;
    }
    else {
      turnTotalError = 0;
    }
    turnTotalError = abs(turnTotalError) > maxTurnIntegral ? signnum_c(turnTotalError) * maxTurnIntegral : turnTotalError;
    turnPower = (turnError * turnkP) + (turnTotalError * turnkI) + ((turnError - turnPrevError) * turnkD);

    FL.spin(forward, lateralPower + turnPower, volt);
    FR.spin(forward, lateralPower - turnPower, volt);
    BL.spin(forward, lateralPower + turnPower, volt);
    BR.spin(forward, lateralPower - turnPower, volt);
    prevError = error;
    turnPrevError = turnError;
    
    std::cout << Inertial10.angle(deg);
    std::cout << std::endl;
    //std::cout << turnTotalError;
    //std::cout << std::endl;

    wait(20, msec);
  }
  return 0;
}

void auton() {
  task aut_drivePID(drivePID);
  while (Inertial10.isCalibrating()) {
	  wait(20,msec);
  }
  resetEncoders();

  //fullfield: 10200
  turnL(2);
  move(400);
  intakeroller.spinFor(-200,deg);
    wait(500,msec);
  move(-600);
  turnL(100);
  move(600);

  while (true) {
    wait(100,msec);
  }
}