/*while (1) {
  if (Controller1.ButtonR1.pressing()) {
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
  }
    if (Controller1.ButtonR2.pressing()) {
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
  }
  printf("%f \n",LEncoder.position(turns));
  printf("%f \n",REncoder.position(turns));
  FR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
  BR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  FL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  BL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() - Controller1.Axis4.value(), percent);

  while (Controller1.ButtonL1.pressing()) {
        intakeroller.spin(forward);
      
        if (Controller1.ButtonR1.pressing()) {
          indexer.spinFor(forward,95,degrees);
          wait(40, msec);
          indexer.spinFor(reverse,95,degrees);
        
  }
    if (Controller1.ButtonR2.pressing()) {
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
  }
  
  
  FR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
  BR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  FL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  BL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() - Controller1.Axis4.value(), percent);

        if (Controller1.ButtonL2.pressing()) {
        intakeroller.spin(reverse);
          if (Controller1.ButtonR1.pressing()) {
          indexer.spinFor(forward,95,degrees);
          wait(40, msec);
          indexer.spinFor(reverse,95,degrees);
  }
    if (Controller1.ButtonR2.pressing()) {
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
      indexer.spinFor(forward,95,degrees);
      wait(40, msec);
      indexer.spinFor(reverse,95,degrees);
  }
  FR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
  BR.spin(forward, Controller1.Axis3.value() - Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  FL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() + Controller1.Axis4.value(), percent);
  BL.spin(forward, Controller1.Axis3.value() + Controller1.Axis1.value() - Controller1.Axis4.value(), percent);
  
  if (Controller1.ButtonR1.pressing()) {
        indexer.spinFor(forward,95,degrees);
        wait(40, msec);
        indexer.spinFor(reverse,95,degrees);
  }
  
      wait(5, msec);
      }
      }
    }
  while (Controller1.ButtonL2.pressing()) {

      intakeroller.spinFor(reverse,50,degrees);
      wait(5, msec);
    }
  if (Controller1.ButtonDown.pressing()) {

      }
  }
*/