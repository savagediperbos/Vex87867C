#include <vex.h>
#include <robot-config.h>

double kP = 0.1; // proportional component - improve rise and settling time
double kI = 0.02; // integral component - corrects undershooting
double kD = 0.05; // derivative component - speeds up and slows down (dampener)
double turnkP = 0.05;
double turnkI = 0.02;
double turnkD = 0.05;

// integral caps
int maxIntegral = 300;
int maxTurnIntegral = 300;

int integralBound = 3; // If error is outside the bounds, then apply the integral. This is a buffer with +-integralBound degrees
int leftMotorSide;
int rightMotorSide;
int frontSide;
int averagePosition;

// motor values
double lateralMotorPower;
int turnDifference;
double turnMotorPower;

// auton settings
int desiredValue;
int desiredTurnValue;

// PID calculations
int error; // sensorvalue (current) - desiredValue : positional value -> speed -> acc
int prevError = 0; // position 20 miliseconds ago
int derivative; // error - prevError : calculates Speed (speeding up and slowing down the robot)
int totalError = 0; // totalError = totalError + error (keeps adding error/ covenrts position into absement)

int turnError; // sensorValue - desiredValue : position
int turnPrevError = 0; // position 20 miliseconds ago
int turnDerivative; // error - prevError : speed
int turnTotalError = 0; // totalError = totalError + error

// DO NOT TOUCH - toggle booleans
bool enableDrivePID = true;

double signnum_c(double x) {
  if (x > 0.0) {
    return 1.0;
  }
  if (x < 0.0) {
    return -1.0;
  }
  return x;
}

void resetSensors() {
  // resets shaft encoder data
  FL.setPosition(0, degrees);
  FR.setPosition(0, degrees);
}
int drivePID() {

  while (enableDrivePID) {

    // make movements more consistent - brake mode holds position
    BL.setStopping(brake);
    FL.setStopping(brake);
    BR.setStopping(brake);
    FR.setStopping(brake);

    // get robot's position via shaft encoders
    leftMotorSide = FL.position(degrees);
    rightMotorSide = FR.position(degrees);

    averagePosition = leftMotorSide + rightMotorSide;

    error = desiredValue - averagePosition;

    derivative = error - prevError; // calculates the derivative value

    if (abs(error) < integralBound) {
      // calculates the integral value
      totalError += error; 
    }
    else {
      totalError = 0; 
    }

    totalError = abs(totalError) > maxIntegral ? signnum_c(totalError) * maxIntegral : totalError;
    lateralMotorPower = error * kP + derivative * kD + totalError * kI;
    
    turnDifference = leftMotorSide - rightMotorSide;
    
    turnError = desiredTurnValue - turnDifference;
    turnDerivative = turnError - turnPrevError;
    turnTotalError += turnError;

    // this would cap the integral
    // ? = conditional operator
    turnTotalError = abs(turnTotalError) > maxIntegral ? signnum_c(turnTotalError) * maxIntegral : turnTotalError;

    turnMotorPower = turnError * turnkP + turnDerivative * turnkD + turnTotalError * turnkI;

    FL.spin(forward, lateralMotorPower + turnMotorPower, voltageUnits::volt);
    FR.spin(forward, lateralMotorPower - turnMotorPower, voltageUnits::volt);
    BL.spin(forward, lateralMotorPower + turnMotorPower, voltageUnits::volt);
    BR.spin(forward, lateralMotorPower - turnMotorPower, voltageUnits::volt);

    prevError = error;
    turnPrevError = turnError;

    wait(20, msec);

  }
  return 0;
}

//auton setting

void fullFieldDrive(void) {
}
