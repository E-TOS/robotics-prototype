/* idea to use a loop for motor control. scrapped because was trying to access motor-unique functions */
/*
for(int i=0;i<NUM_MOTORS;i++){
  if (motorCommand.motorsToMove[i]){

    if(motorArray[i]->setDesiredAngle(motorCommand.anglesToReach[i])){
      if(motorArray[i]->isOpenLoop){
        motorArray[i]->calcCurrentAngle(); //does it actually need to do this?
        motorArray[i]->openLoopError=motorArray[i]->getDesiredAngle()-motorArray[i]->getCurrentAngle();
        motorArray[i]->calcDirection(motorArray[i]->openLoopError);
        if(motorArray[i]->motorType==DC_MOTOR){
          if(motorArray[i]->calcTurningDuration(motorArray[i]->openLoopError)){
            motorArray[i]->timeCount = 0;
            motorArray[i]->movementDone = false;
            UART_PORT.print("$S,Success: motor ");
                        UART_PORT.print(i+1);
                        UART_PORT.print(" to turn for ");
                        UART_PORT.print(motorArray[i]->numMillis);
                        UART_PORT.println(" milliseconds");
          }
          else{
            UART_PORT.println("$E,Error: requested angle is too close to current angle. Motor not changing course.");
          }
        }
        else if(motorArray[i]->motorType==STEPPER_MOTOR){
          if(motorArray[i]->calcNumSteps(motorArray[i]->openLoopError)){
          // I don't set stepCount to 0?
            motorArray[i]->enablePower();
            motorArray[i]->movementDone = false;
            UART_PORT.print("$S,Success: motor ");
                        UART_PORT.print(i+1);
                        UART_PORT.print(" to turn ");
                        UART_PORT.print(motorArray[i]->numSteps);
                        UART_PORT.println(" steps");
          }
          else{
            UART_PORT.println("$E,Error: requested angle is too close to current angle. Motor not changing course.");
          }
        }
        else if(motorArray[i]->motorType==SERVO_MOTOR){
          if(motorArray[i]->calcTurningDuration(motorArray[i]->openLoopError)){
            motorArray[i]->timeCount = 0;
            motorArray[i]->movementDone = false;
            UART_PORT.print("$S,Success: motor ");
                        UART_PORT.print(i+1);
                        UART_PORT.print(" to turn for ");
                        UART_PORT.print(motorArray[i]->numMillis);
                        UART_PORT.println(" milliseconds");
          }
          else{
            UART_PORT.println("$E,Error: requested angle is too close to current angle. Motor not changing course.");
          }
        }
      }
      else if(!(motorArray[i]->isOpenLoop)){
        if(motorArray[i]->motorType==STEPPER_MOTOR){
          motorArray[i]->enablePower();
        }
        motorArray[i]->movementDone = false;
      }
    }
  }
}
*/

/*  // create wrappers to circumvent C++ refusing to attach instance-dependent interrupts inside a class
  void m1WrapperISR(void) {
  motor1.encoder_interrupt();
  }
  void m2WrapperISR(void) {
  motor2.encoder_interrupt();
  }
  void m3WrapperISR(void) {
  motor3.encoder_interrupt();
  }
  void m4WrapperISR(void) {
  motor4.encoder_interrupt();
  }
  //void m5WrapperISR(void){ motor5.encoder_interrupt(); }
  //void m6WrapperISR(void){ motor6.encoder_interrupt(); }
*/

/*
       attachInterrupt(motor1.encoderPinA, m1WrapperISR, CHANGE);
      attachInterrupt(motor1.encoderPinB, m1WrapperISR, CHANGE);

      attachInterrupt(motor2.encoderPinA, m2WrapperISR, CHANGE);
      attachInterrupt(motor2.encoderPinB, m2WrapperISR, CHANGE);

      attachInterrupt(motor3.encoderPinA, m3WrapperISR, CHANGE);
      attachInterrupt(motor3.encoderPinB, m3WrapperISR, CHANGE);

      attachInterrupt(motor4.encoderPinA, m4WrapperISR, CHANGE);
      attachInterrupt(motor4.encoderPinB, m4WrapperISR, CHANGE);
*/
/*
       attachInterrupt(motor5.encoderPinA, m5WrapperISR, CHANGE);
  attachInterrupt(motor5.encoderPinB, m5WrapperISR, CHANGE);

  attachInterrupt(motor6.encoderPinA, m6WrapperISR, CHANGE);
  attachInterrupt(motor6.encoderPinB, m6WrapperISR, CHANGE);
*/

/*
  // for multiple steppers in same timer interrupt (incomplete, complicated to implement)
  void stepperInterrupt(void) {
  static int nextInterval = 0;
  int i = 3;
  // code to decide which motor to turn goes here, or code just turns all motors
  if (!stepperArray[i - 1].movementDone) {
    // code to decide how fast the motor will turn and in which direction
    int dir = CLOCKWISE;
    nextInterval = 25000;
    //set a minimum step period somewhere
    //nextInterval = stepperArray[i-1].pidController.updatePID(stepperArray[i-1].currentAngle, stepperArray[i-1].desiredAngle);
    stepperArray[i - 1].singleStep(dir);
    stepperTimer.update(nextInterval);
  }
  }
*/

/*
  // takes the key (speed, time, etc) and updates?outputs? the next value
  // this one is for unsigned ints, would need to define separate ones for other data types?
  parseKey(String key, unsigned int valueOut, String delimeter, unsigned int minVal, unsigned int maxVal) {
  char *msgElem = strtok_r(NULL, delimeter, &restOfMessage); // find the next message element
  if (String(msgElem) == key) { // msgElem is a char array so it's safer to convert to string first
    msgElem = strtok_r(NULL, delimeter, &restOfMessage); // find the next message element (time in seconds)
    unsigned int tempVar = atoi(msgElem); // converts to int
    //if (tempTimeVar <= MAX_BUDGE_TIME && tempTimeVar >= MIN_BUDGE_TIME) { // don't allow budge movements to last a long time
    if (tempVar <= maxVal && tempVar >= minVal) {
      //motorCommand.whichTime = tempTimeVar;
      valueOut = tempVar;
      Serial.print("parsed time interval "); Serial.print(motorCommand.whichTime); Serial.println("ms");
    }
  }
  }
*/

// ABTIN STUFF

//setup(){
/*
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 1000;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= ((1 << CS11) | (1 << CS10));
  interrupts();

  //start of the heartbeat timer
  startTime = millis();

  }
*/
/* void ISR(TIMER1_COMPA_vect)
  {
  for (int i = 0; i < NUM_STEPPERS; i++) {
   volatile stepperInfo& s = steppers[i];

    if (s.currentAngle >= 360) {
     s.currentAngle = 0;
   }
   if (s.currentAngle < 0) {
     s.currentAngle = 360;
   }

    if ( abs(s.currentAngle - s.desiredAngle) > 1.7 && !s.movementDone  ) {
     s.stepFunc();
     s.currentAngle += s.dir;

    } else {
     s.movementDone = true;
   }

    if (steppers[0].movementDone && steppers[1].movementDone) {  //very dirty. we dont want to check manually.
     TIMER1_INTERRUPTS_OFF
   }
  }
  }
*/

/* void startMovement() {
  OCR1A = c0;
  TIMER1_INTERRUPTS_ON
  }
*/