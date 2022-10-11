#include <vex.h>
#include <robot-config.h>
#include <iostream>

// Drivetrain PID Configuration
double kP = 0.0105; // proportional component - improve rise and settling time
double kI = 0.016; // integral component - corrects undershooting
double kD = 0.04; // derivative component - speeds up and slows down (dampener)
double turnkP = 0.05;
double turnkI = 0.0;
double turnkD = 0;

// integral caps
double maxIntegral = 300;
double maxTurnIntegral = 300;

double integralBound = 3; // If error is outside the bounds, then apply the integral. This is a buffer with +-integralBound degrees
double turnBound = 10;
double averagePosition;

// motor values
double lateralPower;
double turnPower;

// auton settings
double desiredValue = 0;
double desiredTurnValue = 0;

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
  LEncoder.setPosition(0,deg);
  REncoder.setPosition(0,deg);
  Inertial10.setRotation(0,deg); //try setHeading if doesnt work
  desiredValue = 0;
  desiredTurnValue = 0;
}

void moveTurn(double moving, double turning) {
  desiredValue = moving;
  desiredTurnValue = turning;
  wait(500,msec);
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
    //totalError = abs(totalError) > maxIntegral ? signnum_c(totalError) * maxIntegral : totalError;
    lateralPower = (error * kP) + (totalError * kI) + ((error - prevError) * kD);
    
    turnError = desiredTurnValue - Inertial10.angle(degrees);
    if (abs(turnError) < turnBound) {
      turnTotalError += turnError;
    }
    else {
      turnTotalError = 0;
    }
    //turnTotalError = abs(turnTotalError) > maxIntegral ? signnum_c(turnTotalError) * maxIntegral : turnTotalError;
    turnPower = (turnError * turnkP) + (turnTotalError * turnkI) + ((turnError - turnPrevError) * turnkD);

    FL.spin(forward, lateralPower + turnPower, volt);
    FR.spin(forward, lateralPower - turnPower, volt);
    BL.spin(forward, lateralPower + turnPower, volt);
    BR.spin(forward, lateralPower - turnPower, volt);
    prevError = error;
    turnPrevError = turnError;
    
    std::cout << Inertial10.angle(degrees);
    std::cout << std::endl;
    //std::cout << turnError;
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
  //moveTurn(400,0);
  //intakeroller.spinFor(-200,deg);
  //  wait(500,msec);
  //moveTurn(-400,0);
  moveTurn(0,100);
  //moveTurn(600,0);

  while (true) {
    wait(100,msec);
  }
}