/*
#include <cmath> //allows mathematical calculations including trig functions and pow
#include <tuple> //allows use of tuples
#include <vector> //allows use of vectors
#include <limits> //allows use of infinity
#include <algorithm> //allows use of max_element
#include <stdlib.h> //allows use of random number generation
#include <time.h> //allows constant regeneration of seed for number generation
#include <iostream> //allows text output

int main() {
  char side;
  side = 'r';
  double x_location, y_location, x_displacement, y_displacement; //important variables for dimensions and calculations
  std::tuple <double, double> red_goal; 
  std::tuple <double, double> blue_goal;
  red_goal = std::make_tuple (17.78, 122.68); //location of red goal as a tuple
  blue_goal = std::make_tuple (122.68, 17.78); //location of blue goal as a tuple
    
    
  //the below block of code randomly generates valid coordinates to use as a testing method
  //the below block of code will not be used in the final product
  srand (time(NULL));
  x_location = rand() % 140 + 1;
  y_location = rand() % 140 + 1;
  if (side == 'r') {
    while (y_location > x_location or (0 <= y_location and y_location <= 47.81 and 92.6 <= x_location and x_location <= 140)) {
      srand (time(NULL));
      x_location = rand() % 140 + 1;
      y_location = rand() % 140 + 1;
    }
  }
  else if (side == 'b') {
    while (y_location < x_location or (0 <= x_location <= 47.81 and 92.6 <= y_location <= 140)) {
      srand (time(NULL));
      x_location = rand() % 140 + 1;
      y_location = rand() % 140 + 1;
    }
  }
  std::cout << x_location << std::endl;
  std::cout << y_location << std::endl;
      
      
  //the below block of code calculates the angle the robot must move to face the goal
  if (side == 'r') {
    //finds the distance from the goal
    x_displacement = std::abs(std::get<0>(red_goal) - x_location);
    y_displacement = std::abs(std::get<1>(red_goal) - y_location);
        
    //uses trigonometry to calculate the angle to the goal
    if (x_displacement != 0.0) {
      std::cout << (180.0 / 3.141592653589793238463) * atan(y_displacement / x_displacement) << std::endl;
    }
    else {
      std::cout << 180 << std::endl;
    }
        
    //uses trigonometry to calculate the angle to the goal
    if (y_displacement != 0.0) {
      std::cout << (180.0 / 3.141592653589793238463) * atan(x_displacement / y_displacement) << std::endl;
    }
    else {
      std::cout << 180 << std::endl;
    }
  }
  else if (side == 'b') {
    //finds the distance from the goal
    x_displacement = std::abs(std::get<0>(blue_goal) - x_location);
    y_displacement = std::abs(std::get<1>(blue_goal) - y_location);
        
    //uses trigonometry to calculate the angle to the goal
    if (x_displacement != 0) {
      std::cout << (180.0 / 3.141592653589793238463) * atan(y_displacement / x_displacement) << std::endl;
    }
    else {
      std::cout << 180 << std::endl;
    }
        
    //uses trigonometry to calculate the angle to the goal
    if (y_displacement != 0) {
      std::cout << (180.0 / 3.141592653589793238463) * atan(x_displacement / y_displacement) << std::endl;
    }
    else {
      std::cout << 180 << std::endl;
    }
  }
      
      
  //the below block of code calculates the trajectory of a disc with a factor of air pressure
  double g = 9.81; //gravitational constant
  double m = 0.065; //mass
  double rho = 1.22; //density
  double Cd = 0.47; //drag coefficient
  double A = (pow(0.075, 2) / 4) * 3.141592653589793238463; //area
  double drag = (rho * Cd * A) / 2; //drag
  double beta = drag / m; //drag divided by mass
      
  double X0 = 0; //initial position along x-axis
  double Y0 = 0; //initial position along y-axis
  int Vmax = 10; //max calculated velocity in meters per second
  int angl = 40; //angle at which disc is launched
      
  for (double i = 5; i < 101; i += 5) {
    //primary calculations for the trajectory below
    double percent = i / 100;
    double V = Vmax * percent;
    double Vx0 = V * cos(angl * (3.141592653589793238463 / 180.0));
    double Vy0 = V * sin(angl * (3.141592653589793238463 / 180.0));
    double steps = 50;
    double t_HIT = (2.0 * Vy0) / g;
    double dt = t_HIT / steps;
    std::vector<double> X = {X0};
    std::vector<double> Y = {Y0};
    std::vector<double> Vx = {Vx0};
    std::vector<double> Vy = {Vy0};
    for (int j = 1; j < std::numeric_limits<double>::infinity(); j++) {
      double curV = sqrt(pow(Vx.at(j - 1), 2) + pow(Vy.at(j - 1), 2));
      double ax = -1 * beta * Vx.at(j - 1) * curV;
      double ay = -1 * g - (beta * Vy.at(j - 1) * curV);
      double curVx = Vx.at(j - 1) + (ax * dt);
      double curVy = Vy.at(j - 1) + (ay * dt);
      Vx.push_back(curVx);
      Vy.push_back(curVy);
      double xPos = X.at(j - 1) + (Vx.at(j - 1) * dt) + (0.5 * ax * pow(dt, 2));
      double yPos = Y.at(j - 1) + (Vy.at(j - 1) * dt) + (0.5 * ay * pow(dt, 2));
           
      X.push_back(xPos);
      Y.push_back(yPos);
      if (Y.at(j) <= 0.0) {
        //returns velocity results for x and y axes
        std::cout << "velocity: " << V << ": " << *std::max_element(X.begin(), X.end()) << ", " << *std::max_element(Y.begin(), Y.end()) << std::endl;
        break;
      }
    }
  }
}
*/












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