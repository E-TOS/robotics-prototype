
#ifndef PARSER_H
#define PARSER_H
#include "PinSetup.h"
#include "RobotMotor.h"
#define MOTOR_NOT_COMMANDED "~" // this character means a motor is not to be moved
// this struct is specific to the arm teensy at the moment.
// it should either be generalized or put in the main code,
// but it can't be put in the main code because commandInfo
// needs to be defined where it's used as an input parameter
struct commandInfo {
  // commands that apply to the teensy in general
  bool pingCommand = false; // for ping command
  bool stopAllMotors = false; // for stopping all motors

  // commands that apply to a specific motor
  int whichMotor = 0; // which motor was requested to do something

  bool stopSingleMotor = false; // for stopping a single motor
  bool switchDir = false; // for switching the direction logic

  bool loopCommand = false; // for choosing between open loop or closed loop control
  int loopState = 0; // what type of loop state it is

  bool resetCommand = false; // indicates that something should be reset
  bool resetAngleValue = false; // mostly for debugging/testing, reset the angle variable
  bool resetJointPosition = false; // for moving a joint to its neutral position

  bool budgeCommand = false; // for turning without angle requests
  int directionsToMove[NUM_MOTORS] = {0, 0, 0, 0, 0, 0}; // direction of budge control
  bool multiMove = false; // for controlling multiple motors simultaneously
  bool motorsToMove[NUM_MOTORS] = {false, false, false, false, false, false}; // which motor to move
  float anglesToReach[NUM_MOTORS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // motor angles
};

class Parser {
  public:
    bool isValidNumber(String str, char type);
    void parseCommand(commandInfo & cmd, char * restOfMessage);
    //void parseRosCommand(commandInfo & cmd, String message);
    bool verifCommand(commandInfo cmd);
};

bool Parser::isValidNumber(String str, char type)
{
  if (str.length() == 0) {
    return false;
  }
  for (unsigned int i = 0; i < str.length(); i++) {
    if (type == 'f') {
      if (!isDigit(str[i]) && !(str[i] == '.') && !(str[i] == '-')) {
        return false;
      }
    }
    if (type == 'd') {
      if (!isDigit(str[i]) && !(str[i] == '-')) {
        return false;
      }
    }
  }
  return true;
}

void Parser::parseCommand(commandInfo & cmd, char * restOfMessage) {
  // check for emergency stop has precedence
  char * msgElem = strtok_r(restOfMessage, " ", & restOfMessage); // look for first element (first tag)
  if (String(msgElem) == "ping") {
    cmd.pingCommand = true;
#ifdef DEBUG_PARSING
    UART_PORT.println("$S,Success: parsed ping command");
#endif
  }
  else if (String(msgElem) == "stop") {
    // msgElem is a char array so it's safer to convert to string first
    cmd.stopAllMotors = true;
#ifdef DEBUG_PARSING
    UART_PORT.println("$S,Success: parsed emergency command to stop all motors");
#endif
  }
  // check for simultaneous motor control
  else if (String(msgElem) == "move" || String(msgElem) == "budge") {
    // msgElem is a char array so it's safer to convert to string first
    bool isValidCommand = true;
    bool isValidBudge = false;
    int i = 0;
    if (String(msgElem) == "budge") {
      isValidBudge = true;
    }
    do {
      msgElem = strtok_r(NULL, " ", & restOfMessage); // go to next msg element (motor 1 angle)
      if (isValidBudge) {
        if (String(msgElem) == "fwd") {
          cmd.directionsToMove[i] = COUNTER_CLOCKWISE;
          cmd.motorsToMove[i] = true; // set bool in move motors bool array to true
        }
        else if (String(msgElem) == "back") {
          cmd.directionsToMove[i] = CLOCKWISE;
          cmd.motorsToMove[i] = true; // set bool in move motors bool array to true
        }
        else if (String(msgElem) == MOTOR_NOT_COMMANDED) {
          // don't do anything to move motors bool array
#ifdef DEBUG_PARSING
          UART_PORT.print("$S,Success: parsed motor ");
          UART_PORT.print(i + 1);
          UART_PORT.println(" to maintain old desired position");
#endif
        }
        else {
          isValidBudge = false;
        }
      }
      else if (isValidNumber(String(msgElem), 'f')) {
        cmd.anglesToReach[i] = atof(msgElem); // update value in motor angles array
        cmd.motorsToMove[i] = true; // set bool in move motors bool array to true
#ifdef DEBUG_PARSING
        UART_PORT.print("$S,Success: parsed angle of ");
        UART_PORT.print(cmd.anglesToReach[i]);
        UART_PORT.print(" degrees for motor ");
        UART_PORT.println(i + 1);
#endif
      }
      else if (String(msgElem) == MOTOR_NOT_COMMANDED) {
        // don't do anything to move motors bool array
#ifdef DEBUG_PARSING
        UART_PORT.print("$S,Success: parsed motor ");
        UART_PORT.print(i + 1);
        UART_PORT.println(" to maintain old desired position");
#endif
      }
      else {
        isValidCommand = false;
#ifdef DEBUG_PARSING
        UART_PORT.print("$E,Error: parsed invalid angle of ");
        UART_PORT.print(msgElem);
        UART_PORT.print(" degrees for motor ");
        UART_PORT.println(i + 1);
#endif
      }
      i++;
    }
    while (i < NUM_MOTORS) ; // this skips everything after the 6th val but i want it to not skip but i can't get it to work
    if (isValidCommand && i == NUM_MOTORS) {
      if (isValidBudge) {
        cmd.budgeCommand = true;
      }
      else {
        cmd.multiMove = true;
      }
    }
    else if (i < NUM_MOTORS) {
#ifdef DEBUG_PARSING
      UART_PORT.println("$E,Error: parsed insufficient input arguments for simultaneous motor control");
#endif
    }
    else if (i > NUM_MOTORS) { // this doesn't actually get checked right now
#ifdef DEBUG_PARSING
      UART_PORT.println("$E,Error: parsed too many input arguments for simultaneous motor control");
#endif
    }
    else if (!isValidCommand) {
#ifdef DEBUG_PARSING
      UART_PORT.println("$E,Error: parsed invalid simultaneous motor control command");
#endif
    }
  }
  // check for motor command
  else if (String(msgElem) == "motor") {
    // msgElem is a char array so it's safer to convert to string first
    msgElem = strtok_r(NULL, " ", & restOfMessage); // go to next msg element (motor number)
    // if ( isValidNumber(String(msgElem),'d') )
    cmd.whichMotor = atoi(msgElem);
    if (cmd.whichMotor > 0 && cmd.whichMotor <= NUM_MOTORS) {
#ifdef DEBUG_PARSING
      UART_PORT.print("$S,Success: parsed motor ");
      UART_PORT.println(cmd.whichMotor);
#endif
      // check for motor stop command has precedence
      msgElem = strtok_r(NULL, " ", & restOfMessage); // find the next message element (direction tag)
      if (String(msgElem) == "stop") {
        // msgElem is a char array so it's safer to convert to string first
        cmd.stopSingleMotor = true;
#ifdef DEBUG_PARSING
        UART_PORT.println("$S,Success: parsed request to stop single motor");
#endif
      }
      // check for angle command
      else if (String(msgElem) == "angle") {
        // msgElem is a char array so it's safer to convert to string first
        msgElem = strtok_r(NULL, " ", & restOfMessage); // go to next msg element (desired angle value)
        if (isValidNumber(String(msgElem), 'f')) {
          cmd.motorsToMove[cmd.whichMotor - 1] = true;
          cmd.anglesToReach[cmd.whichMotor - 1] = atof(msgElem);
          cmd.multiMove = true;
#ifdef DEBUG_PARSING
          UART_PORT.print("$S,Success: parsed desired angle ");
          UART_PORT.println(cmd.anglesToReach[cmd.whichMotor - 1]);
#endif
        }
        else {
#ifdef DEBUG_PARSING
          UART_PORT.println("$E,Error: parsed desired angle is not a number");
#endif
        }
      }
      // check for loop state command
      else if (String(msgElem) == "loop") {
        // msgElem is a char array so it's safer to convert to string first
        cmd.loopCommand = true;
        msgElem = strtok_r(NULL, " ", & restOfMessage); // go to next msg element (desired angle value)
        if (String(msgElem) == "open") {
          cmd.loopState = OPEN_LOOP;
#ifdef DEBUG_PARSING
          UART_PORT.print("$S,Success: parsed open loop state (");
          UART_PORT.print(cmd.loopState);
          UART_PORT.println(") request");
#endif
        }
        else if (String(msgElem) == "closed") {
          cmd.loopState = CLOSED_LOOP;
#ifdef DEBUG_PARSING
          UART_PORT.print("$S,Success: parsed closed loop state (");
          UART_PORT.print(cmd.loopState);
          UART_PORT.println(") request");
#endif
        }
        else {
#ifdef DEBUG_PARSING
          UART_PORT.println("$E,Error: parsed unknown loop state");
#endif
        }
      }
      // check for angle reset command
      else if (String(msgElem) == "reset") {
        // msgElem is a char array so it's safer to convert to string first
        cmd.resetCommand = true;
        msgElem = strtok_r(NULL, " ", & restOfMessage); // go to next msg element (desired angle value)
        if (String(msgElem) == "angle") {
          cmd.resetAngleValue = true;

#ifdef DEBUG_PARSING
          UART_PORT.println("$S,Success: parsed request to reset angle value");
#endif

        }
        else if (String(msgElem) == "position") {
          cmd.resetJointPosition = true;
#ifdef DEBUG_PARSING
          UART_PORT.println("$S,Sucess: parsed request to reset joint position");
#endif
        }
        else {
#ifdef DEBUG_PARSING
          UART_PORT.println("$E,Error: parsed unknown reset request");
#endif
        }
      }
      else if (String(msgElem) == "switchdirection") {
        cmd.switchDir = true;
#ifdef DEBUG_PARSING
        UART_PORT.println("$S,Success: parsed request to switch direction logic");
#endif
      }
      else {
#ifdef DEBUG_PARSING
        UART_PORT.print("$E,Error: parsed unknown motor ");
        UART_PORT.print(cmd.whichMotor);
        UART_PORT.println(" command");
#endif
      }
    }
    else {
#ifdef DEBUG_PARSING
      UART_PORT.println("$E,Error: parsed invalid motor number");
#endif
    }
  }
  else {
#ifdef DEBUG_PARSING
    UART_PORT.println("$E,Error: parsed unknown motor command");
#endif
  }
}

bool Parser::verifCommand(commandInfo cmd) {
  if (cmd.pingCommand) {
#ifdef DEBUG_VERIFYING
    UART_PORT.println("$S,Success: verified ping command");
#endif
    return true;
  }
  else if (cmd.stopAllMotors) {
#ifdef DEBUG_VERIFYING
    UART_PORT.println("$S,Success: verified command to stop all motors");
#endif
    return true;
  }
  else if (cmd.budgeCommand) {
    for (int i = 0; i < NUM_MOTORS; i++) {
      if (cmd.motorsToMove[i]) {
        if (cmd.directionsToMove[i] == 0) {
          return false;
        }
      }
    }
#ifdef DEBUG_VERIFYING
    UART_PORT.println("$S,Success: verified command to budge all motors for directions: ");
    for (int i = 0; i < NUM_MOTORS; i++) {
      if (cmd.motorsToMove[i]) {
        UART_PORT.print(cmd.directionsToMove[i]);
      }
      else {
        UART_PORT.print(MOTOR_NOT_COMMANDED);
      }
      UART_PORT.print(" ");
    }
    UART_PORT.println("");
#endif
    return true;
  }
  else if (cmd.multiMove) {
    for (int i = 0; i < NUM_MOTORS; i++) {
      if (cmd.motorsToMove[i]) {
        if (cmd.anglesToReach[i] < MIN_JOINT_ANGLE || cmd.anglesToReach[i] > MAX_JOINT_ANGLE) {
#ifdef DEBUG_VERIFYING
          UART_PORT.print("$E,Error: invalid angle of ");
          UART_PORT.print(cmd.anglesToReach[i]);
          UART_PORT.print(" degrees for motor ");
          UART_PORT.println(i + 1);
#endif
          return false;
        }
        else {
#ifdef DEBUG_VERIFYING
          UART_PORT.print("$S,Success: verified angle of ");
          UART_PORT.print(cmd.anglesToReach[i]);
          UART_PORT.print(" degrees for motor ");
          UART_PORT.println(i + 1);
#endif
        }
      }
    }
#ifdef DEBUG_VERIFYING
    UART_PORT.println("$S,Success: verified command to move all motors for angles: ");
    for (int i = 0; i < NUM_MOTORS; i++) {
      if (cmd.motorsToMove[i]) {
        UART_PORT.print(cmd.anglesToReach[i]);
      }
      else {
        UART_PORT.print(MOTOR_NOT_COMMANDED);
      }
      UART_PORT.print(" ");
    }
    UART_PORT.println("");
#endif
    return true;
  }
  // 0 means there was an invalid command and therefore motors shouldn't be controlled
  else if (cmd.whichMotor > 0 && cmd.whichMotor <= RobotMotor::numMotors) {
    if (cmd.stopSingleMotor) {
#ifdef DEBUG_VERIFYING
      UART_PORT.print("$S,Success: verified command to stop motor ");
      UART_PORT.println(cmd.whichMotor);
#endif
      return true;
    }
    else if (cmd.loopCommand) {
      if (cmd.loopState == OPEN_LOOP || cmd.loopState == CLOSED_LOOP) {
#ifdef DEBUG_VERIFYING
        UART_PORT.print("$S,Success: verified command to set motor ");
        UART_PORT.print(cmd.whichMotor);
        if (cmd.loopState == OPEN_LOOP) {
          UART_PORT.println(" to open loop");
        }
        if (cmd.loopState == CLOSED_LOOP) {
          UART_PORT.println(" to closed loop");
        }
#endif
        return true;
      }
      else {
#ifdef DEBUG_VERIFYING
        UART_PORT.println("$E,Error: invalid loop state");
#endif
        return false;
      }
    }
    else if (cmd.resetCommand) {
      if (cmd.resetAngleValue || cmd.resetJointPosition) {
#ifdef DEBUG_VERIFYING
        UART_PORT.print("$S,Success: verified command to reset motor ");
        UART_PORT.print(cmd.whichMotor);
        if (cmd.resetAngleValue) {
          UART_PORT.println(" saved angle value");
        }
        if (cmd.resetJointPosition) {
          UART_PORT.println(" physical joint position");
        }
#endif
        return true;
      }
      else {
#ifdef DEBUG_VERIFYING
        UART_PORT.println("$E,Error: invalid reset request");
#endif
        return false;
      }
    }
    else if (cmd.switchDir) {
#ifdef DEBUG_VERIFYING
      UART_PORT.print("$S,Success: verified command to switch motor ");
      UART_PORT.print(cmd.whichMotor);
      UART_PORT.println(" direction logic");
#endif
      return true;
    }
    else {
#ifdef DEBUG_VERIFYING
      UART_PORT.print("$E,Error: invalid command for motor ");
      UART_PORT.println(cmd.whichMotor);
#endif
      return false;
    }
  }
  else if (cmd.whichMotor < 0 || cmd.whichMotor >= RobotMotor::numMotors) {
#ifdef DEBUG_VERIFYING
    UART_PORT.println("$E,Error: invalid motor index");
#endif
    return false;
  }
  else {
#ifdef DEBUG_VERIFYING
    UART_PORT.println("$E,Error: invalid motor command");
#endif
    return false;
  }
}

#endif
