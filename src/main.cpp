/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Authors:      Luke Lu, Daniel Yang                                      */
/*    Created:      Sat Jul 16 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// FL                   motor         1               
// FR                   motor         2               
// BL                   motor         11              
// BR                   motor         14              
// Controller1          controller                    
// Inertial21           inertial      21              
// LEncoder             encoder       C, D            
// REncoder             encoder       A, B            
// FEncoder             encoder       E, F            
// intakeroller         motor         15              
// flywheel             motor         18              
// indexer              motor         19              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include <vex.h>
#include <cmath>
#include <iostream>

using namespace vex;
competition Competition;
void pre_auton(void) {
  vexcodeInit();
}
double kP = 0.1; //error
double kI = 0.02; //corrects undershooting
double kD = 0.05; //speeds up and slows down (dampener)
double turnkP = 0.05;
double turnkI = 0.02;
double turnkD = 0.05;
int maxTurnIntegral = 300; // These cap the integrals
int maxIntegral = 300;
int integralBound = 3; //If error is outside the bounds, then apply the integral. This is a buffer with +-integralBound degrees
//auton settings
int desiredValue = 0;
int desiredSValue = 0;
int desiredTurnValue = 0;
//tuned drive values
int ffb = desiredValue * 1.3; //field drive
int flr = desiredTurnValue * 1.244; //field turn

//PID calculations
int error; //sensorvalue (current) - Desiredvalue : positional value -> speed -> acc
int prevError = 0; //position 20 miliseconds ago
int derivative; // error - prevError : calculates Speed (speeding up and slowing down the robot)
int totalError = 0; //totalError = totalError + error (keeps adding error/ covenrts position into absement)

int turnError; //SensorValue - DesiredValue : Position
int turnPrevError = 0; //Position 20 miliseconds ago
int turnDerivative; // error - prevError : Speed
int turnTotalError = 0; //totalError = totalError + error

bool resetDriveSensors = false;
//Variables modified for use
bool enableDrivePID = true;

double signnum_c(double x) {
  if (x > 0.0) return 1.0;
  if (x < 0.0) return -1.0;
  return x;
}
void stopDrive() {
  
}
int drivePID() {
  while(enableDrivePID) {
  BL.setStopping(brake);
  FL.setStopping(brake);
  BR.setStopping(brake);
  FR.setStopping(brake);

    if (resetDriveSensors) {
      resetDriveSensors = false;
      LEncoder.setPosition(0, degrees);
      REncoder.setPosition(0, degrees);
      FEncoder.setPosition(0, degrees);
    }

    //Get the position of both motors
    int leftMotorSide = LEncoder.position(degrees);
    int rightMotorSide = REncoder.position(degrees);
    int backSide = FEncoder.position(degrees);

    int averagePosition = leftMotorSide;
    //int averagePosition = (leftMotorSide + rightMotorSide)/2;
    //P
    error = desiredValue - averagePosition;
    //D
    derivative = error - prevError;
    //I
    if(abs(error) < integralBound){
    totalError+=error; 
    }  else {
    totalError = 0; 
    }
    totalError = abs(totalError) > maxIntegral ? signnum_c(totalError) * maxIntegral : totalError;
    double lateralMotorPower = error * kP + derivative * kD + totalError * kI;
    double lmposition = (leftMotorSide);
    double rmposition = (rightMotorSide);
    int turnDifference = lmposition - rmposition;
    turnError = turnDifference/2;
    //turnError = desiredTurnValue - turnDifference;
    turnDerivative = turnError - turnPrevError;
    turnTotalError += turnError; //(another calc)

    //This would cap the integral
    turnTotalError = abs(turnTotalError) > maxIntegral ? signnum_c(turnTotalError) * maxIntegral : turnTotalError;

    double turnMotorPower = turnError * turnkP + turnDerivative * turnkD + turnTotalError * turnkI;
    ///////////////////////////////////////////////////////////////////////////

    FL.spin(forward, lateralMotorPower + turnMotorPower, voltageUnits::volt);
    FR.spin(forward, lateralMotorPower - turnMotorPower, voltageUnits::volt);
    BL.spin(forward, lateralMotorPower + turnMotorPower, voltageUnits::volt);
    BR.spin(forward, lateralMotorPower - turnMotorPower, voltageUnits::volt);

    prevError = error;
    turnPrevError = turnError;
    vex::task::sleep(20);

  }

  return 1;
}
int printEncoderValues() {
  while(enableDrivePID) {
    std::cout << "LEncoder: " << LEncoder.position(degrees) << std::endl; //prints left shaft encoder data
    std::cout << "REncoder: " << REncoder.position(degrees) << std::endl; //prints right shaft encoder data
    std::cout << "FEncoder: " << FEncoder.position(degrees) << std::endl; //prints front shaft encoder data
    vex::task::sleep(10);
  }
  return 1;
}
//auton setting
void fullFieldDrive(void) {
  desiredTurnValue = 0;
  desiredValue = 2400;
}

void AutonRedRight(void) {
  //auton for red rightside
}
void AutonRedLeft(void) {
  //auton for red leftside
}
void AutonBlueRight(void) {
  //auton for blue rightside
}
void AutonBlueLeft(void) {
  //auton for blue leftside
}
void correctionValue() {
  
}
void autonomous(void) {
  vex::task autonpidtask(drivePID);
  vex::task ecoderPrinttask(printEncoderValues);
  resetDriveSensors = true;

  desiredValue = 50;
  desiredTurnValue = 0;

  vex::task::sleep(1700);
  resetDriveSensors = true;
  desiredValue = 100;
  desiredTurnValue = 0;
  intakeroller.spinFor(reverse,220,degrees);

  //fullFieldDrive();

  //AutonRedRight
  //AutonRedLeft
  //AutonBlueRight
  //AutonBlueLeft
}
void usercontrol(void) {
  // initial values config
  intakeroller.setVelocity(100,percent);
  flywheel.setVelocity(100,percent);
  indexer.setVelocity(100,percent);
  indexer.setStopping(hold);
  FR.setStopping(coast);
  FL.setStopping(coast);
  BR.setStopping(coast);
  BL.setStopping(coast);

  //while (true) {
  //}
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();
  while (true) {
    wait(20, msec);
  }
}
