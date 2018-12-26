
#ifndef DCMOTOR_H
#define DCMOTOR_H
#include <Arduino.h>
#include "PinSetup.h"
#include "RobotMotor.h"
class DcMotor:public RobotMotor
{
  public:
    static int numDcMotors;
    // DcMotor(int pwmPin, int encA, int encB); // for sabertooth
    // for cytron
    DcMotor(int dirPin, int pwmPin, float gearRatio);
    bool calcTurningDuration(float angle); // guesstimates how long to turn at the preset open loop motor speed to get to the desired position
    void stopRotation(void);
    void setVelocity(int motorDir, float motorSpeed); // currently this actually activates the dc motor and makes it turn at a set speed/direction
    // stuff for open loop control
    float openLoopError; // public variable for open loop control
    int openLoopSpeed; // angular speed (degrees/second)
    float openLoopGain; // speed correction factor
    unsigned int numMillis; // how many milliseconds for dc motor to reach desired position
    elapsedMillis timeCount; // how long has the dc motor been turning for
  private:
    int directionPin; // for new driver
    int pwmPin;
};

int DcMotor::numDcMotors = 0; // must initialize variable outside of class
// for sabertooth
// DcMotor::DcMotor(int pwmPin, int encA, int encB):
// pwmPin(pwmPin), encA(encA), encB(encB)
// for new driver
DcMotor::DcMotor(int dirPin, int pwmPin, float gearRatio):// if no encoder
directionPin(dirPin), pwmPin(pwmPin)
{
  numDcMotors++;
  // variables declared in RobotMotor require the this-> operator
  this -> gearRatio = gearRatio;
  this -> gearRatioReciprocal = 1 / gearRatio; // preemptively reduce floating point calculation time
  hasEncoder = false;
  openLoopSpeed = 0; // no speed by default;
  openLoopGain = 1.0; // temp open loop control
}

void DcMotor::stopRotation(void)
{
  analogWrite(pwmPin, 0); // 0 for cytron, different implementation for sabertooth
  movementDone = true;
}

void DcMotor::setVelocity(int motorDir, float motorSpeed)
{
  if (!isOpenLoop)
    motorSpeed = fabs(motorSpeed);
  // makes sure the speed is within the limits set in the pid during setup
  if (motorSpeed * motorDir > pidController.maxOutputValue)
  {
    motorSpeed = pidController.maxOutputValue;
  }
  if (motorSpeed * motorDir < pidController.minOutputValue)
  {
    motorSpeed = pidController.minOutputValue;
  }
  switch (motorDir)
  {
    case CLOCKWISE:
      digitalWriteFast(directionPin, LOW);
      break;
    case COUNTER_CLOCKWISE:
      digitalWriteFast(directionPin, HIGH);
      break;
  }
  int dutyCycle = motorSpeed * 255 / 100;
  analogWrite(pwmPin, dutyCycle);
}

bool DcMotor::calcTurningDuration(float angle)
{
  // if the error is big enough to justify movement
  if (fabs(angle) > pidController.jointAngleTolerance)
  {
    // here we have to multiply by the gear ratio to find the angle actually traversed by the motor shaft
    numMillis = (fabs(angle) * gearRatio / openLoopSpeed) * 1000.0 * openLoopGain; // calculate how long to turn for
    return true;
  }
  else
  {
    return false;
  }
}

#endif
