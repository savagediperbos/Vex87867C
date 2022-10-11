/*
#include <cmath> //allows mathematical calculations including trig functions and pow
#include <tuple> //allows use of tuples
#include <vector> //allows use of vectors
#include <limits> //allows use of infinity
#include <algorithm> //allows use of max_element
#include <stdlib.h> //allows use of random number generation
#include <time.h> //allows constant regeneration of seed for number generation
#include <iostream> //allows text output
  
  //the below block of code calculates the trajectory of a disc with a factor of air pressure
  double g = 9.81; //gravitational constant
  double m = 0.065; //mass
  double rho = 1.22; //density
  double Cd = 0.05; //drag coefficient
  double A = 0.01; //area
  double drag = (rho * Cd * A) / 2; //drag
  double mu = drag / m; //drag divided by mass
  double distance_to_target = 1.0;
  std::vector<double> y_interval = {0,50};
      
  double X0 = 0; //initial position along x-axis
  double Y0 = 0; //initial position along y-axis
  int Vmax = 3.81; //max calculated velocity in meters per second
  int angl = 40; //angle at which disc is launched

  int steps = 1000;
  double t_HIT = 2.0 * Vmax * sin(angl * (3.141592653589793238463 / 180.0)) / g;
  double dt = t_HIT / steps;
    
int main() {
  for (double i = 5; i < 101; i += 5) {
    //primary calculations for the trajectory below
    double percent = i / 100;
    double V = Vmax * percent;
    double Vx0 = V * cos(angl * (3.141592653589793238463 / 180.0));
    double Vy0 = V * sin(angl * (3.141592653589793238463 / 180.0));
    double t_HIT = (2.0 * Vy0) / g;
    double dt = t_HIT / steps;
    std::vector<double> X = {X0};
    std::vector<double> Y = {Y0};
    std::vector<double> Vx = {Vx0};
    std::vector<double> Vy = {Vy0};
    for (int j = 1; j < std::numeric_limits<double>::infinity(); j++) {
      double curV = sqrt(pow(Vx.at(j - 1), 2) + pow(Vy.at(j - 1), 2));
      double ax = -mu * Vx.at(j - 1) * curV;
      double ay = -g - mu * Vy.at(j - 1) * curV;
      double curVx = Vx.at(j - 1) + (ax * dt) * dt;
      double curVy = Vy.at(j - 1) + (ay * dt) * dt;
      double xPos = X.at(j - 1) + (Vx.at(j - 1) * dt) + (0.5 * ax * pow(dt, 2));
      double yPos = Y.at(j - 1) + (Vy.at(j - 1) * dt) + (0.5 * ay * pow(dt, 2));
      Vx.push_back(Vx.at(j - 1) + (ax * dt));
      Vy.push_back(Vy.at(j - 1) + (ay * dt));
      X.push_back(xPos);
      Y.push_back(yPos);
           
      if (xPos > distance_to_target && std::min_element(y_interval.begin(), y_interval.end()) <= yPos && yPos <= std::max_element(y_interval.begin(), y_interval.end()))
    }
  }
}
*/