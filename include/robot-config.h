#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <v5.h>
#include <v5_vcs.h>

using namespace vex;

extern brain Brain;

extern motor FL;
extern motor FR;
extern motor BL;
extern motor BR;
extern controller Controller1;
extern encoder LEncoder;
extern encoder REncoder;
extern motor intakeroller;
extern motor flywheel;
extern motor flywheel2;
extern motor indexer;
extern inertial Inertial10;
extern digital_out expansion;


void driverControl();
void auton();