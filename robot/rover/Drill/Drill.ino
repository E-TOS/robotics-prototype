#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define SERVO_STOP 1500
#define SERVO_MAX_CW 1250
#define SERVO_MAX_CCW 1750

#define LIMIT_TOP         19
#define LIMIT_BOTTOM      20
#define TABLE_SWITCH_PIN  21
#define TRIGGER_DELAY     50


int drill_speed(int input_drill_speed);//max 165RPM
float elevator_feed(int input_elevator_feed);//max 0.107inch/s


volatile bool isTriggered = false;
volatile bool isContacted = false;
bool isActualPress = false;
bool isPushed = false;
unsigned long triggerTime;

bool isActivated = false;
int drill = 6;
int drill_direction = 7;
int elevator = 4;
int elevator_direction = 5;
int tablePin = 13;
int pump1A = 34;
int pump1B = 36;
int pump2A = 38;
int pump2B = 40;
int pump3A = 42;
int pump3B = 44;
int pump4A = 46;
int pump4B = 48;
int pump5A = 50;
int pump5B = 52;
int led1 = 11;
int led2 = 12;
int photoresistor = A2;
int maxVelocity = 255;
int drillSpeed;
int elevatorSpeed;
float val = 0;
float voltage = 0;
volatile char tableDirection = 'n'; // n for neutral, i for increasing, d for decreasing
volatile int tablePosition[13];
int cuvette = 0;
int desiredPosition = 0;
bool turnTableFree = true;
bool elevatorInUse = false;
volatile char previousElevatorState = 'n'; // n for neutral, u for up, d for down
int i = 0;

//void elevatorTopInterrupt (void);
void elevatorBottomInterrupt (void);

void cuvettePosition (void);
void turnTable (int cuvette, int desiredPosition);

void debouncing(void);

Servo table;

void setup() {

  Serial.begin(115200); Serial.setTimeout(10);

  table.attach(tablePin);
  pinMode(drill, OUTPUT);
  pinMode(drill_direction, OUTPUT);
  pinMode(elevator, OUTPUT);
  pinMode(elevator_direction, OUTPUT);
  pinMode(pump1A, OUTPUT);
  pinMode(pump1B, OUTPUT);
  pinMode(pump2A, OUTPUT);
  pinMode(pump2B, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(LIMIT_TOP, INPUT_PULLUP);
  pinMode(LIMIT_BOTTOM, INPUT_PULLUP);
  pinMode(photoresistor, INPUT_PULLUP);
  //  attachInterrupt(digitalPinToInterrupt(LIMIT_TOP), elevatorTopInterrupt, HIGH);
  attachInterrupt(digitalPinToInterrupt(LIMIT_BOTTOM), debouncing, FALLING );

  //  pinMode(TABLE_SWITCH_PIN, INPUT_PULLUP);
  //  attachInterrupt(digitalPinToInterrupt(TABLE_SWITCH_PIN), debouncing, CHANGE);

  analogWrite(drill, 0);
  analogWrite(elevator, 0);
  digitalWrite(pump1A, LOW);
  digitalWrite(pump1B, LOW);
  digitalWrite(pump2A, LOW);
  digitalWrite(pump2B, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  table.writeMicroseconds(SERVO_STOP);

  for (i = 0; i <= 12; i++) {
    tablePosition[i] = i;
  }

  delay(1000);

  Serial.print("\nsetup complete");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');

    if (isActivated == false) {

      Serial.print("cmd: ");
      Serial.println(cmd);

    }
    if (cmd == "activate") {
      isActivated = true;
    }
    else if (cmd == "who") {
      Serial.println("drill");
    }
    else if (isActivated == true) {

      if (cmd == "drill") {
        //turns drill at desired speed
        int drillDigiDirection = 0;
        int drillSpeedPercent = 0;
        Serial.println("What is the desired drill direction? CCW=1 CW=0");
        while (!Serial.available()) {
          ;
        }
        drillDigiDirection = (Serial.readStringUntil('\n')).toInt();
        Serial.println(drillDigiDirection);

        Serial.println("What percentage of max speed?");
        while (!Serial.available()) {
          ;
        }
        drillSpeedPercent = (Serial.readStringUntil('\n')).toInt();

        Serial.println(drill_speed(drillSpeedPercent));
        Serial.println("RPM\n");
        if (drillDigiDirection == 1)Serial.println("CCW\n");
        else if (drillDigiDirection == 0)Serial.println("CW\n");
        analogWrite(drill, 0);
        delay(50);
        digitalWrite(drill_direction, drillDigiDirection);
        analogWrite(drill, drillSpeedPercent * 255 / 100);
      }
      if (cmd == "dccw") {
        //turns drill counter-clockwise
        analogWrite(drill, 0);
        delay(100);
        digitalWrite(drill_direction, HIGH);
        analogWrite(drill, maxVelocity);
        Serial.println("dccw");
      }
      else if (cmd == "dcw") {
        //turns drill clockwise
        analogWrite(drill, 0);
        delay(100);
        digitalWrite(drill_direction, LOW);
        analogWrite(drill, maxVelocity);
        Serial.println("dcw");
      }
      else if (cmd == "ds") {
        //stops drill
        analogWrite(drill, 0);
        Serial.println("ds");
      }
      if (cmd == "elevator") {
        //turns drill at desired speed
        int elevatorDigiDirection = 0;
        int elevatorFeedPercent = 0;
        Serial.println("What is the desired elevator direction? Up=1 Down=0");
        while (!Serial.available()) {
          ;
        }
        elevatorDigiDirection = (Serial.readStringUntil('\n')).toInt();
        Serial.println(elevatorDigiDirection);

        Serial.println("What percentage of max feed?");
        while (!Serial.available()) {
          ;
        }
        elevatorFeedPercent = (Serial.readStringUntil('\n')).toInt();

        Serial.println(elevator_feed(elevatorFeedPercent));
        Serial.println("inch/min\n");
        if (elevatorDigiDirection == 1)Serial.println("Up\n");
        else if (elevatorDigiDirection == 0)Serial.println("Down\n");
        analogWrite(elevator, 0);
        delay(50);
        digitalWrite(elevator_direction, elevatorDigiDirection);
        analogWrite(elevator, elevatorFeedPercent * 255 / 100);
        if (elevatorDigiDirection == 1)previousElevatorState = 'u';
        else previousElevatorState = 'd';
      }
      else if (cmd == "eup") {
        //turns elevator clockwise
        analogWrite(elevator, 0);
        delay(100);
        digitalWrite(elevator_direction, HIGH);
        analogWrite(elevator, maxVelocity);
        previousElevatorState = 'u';
        Serial.println("eup");
      }
      else if (cmd == "edown") {
        //turns elevator counter-clockwise
        analogWrite(elevator, 0);
        delay(100);
        digitalWrite(elevator_direction, LOW);
        analogWrite(elevator, maxVelocity);
        previousElevatorState = 'd';
        Serial.println("edown");
      }
      else if (cmd == "es") {
        //stops elevator
        analogWrite(elevator, 0);
        previousElevatorState = 'n';
        Serial.println("es");
      }
      else if (cmd == "goto") {
        //sends table to wanted position
        //eventually need split function for a single string command

        Serial.println("What is the cuvette of interest?");
        while (!Serial.available()) {
          ;
        }
        cuvette = (Serial.readStringUntil('\n')).toInt();
        Serial.println(cuvette);

        Serial.println("What is the desired position?");
        while (!Serial.available()) {
          ;
        }
        desiredPosition = (Serial.readStringUntil('\n')).toInt();
        Serial.println(desiredPosition);

        if (cuvette >= 13) {
          Serial.println("Error. Chose cuvette number from 0 to 12");
        }
        if (desiredPosition >= 13) {
          Serial.println("Error. Chose position number from 0 to 12");
        }
        turnTable (cuvette, desiredPosition);

      }
      else if (cmd == "tccw") {
        //turns table counter-clockwise
        table.writeMicroseconds(SERVO_STOP);
        delay(100);
        table.writeMicroseconds(SERVO_MAX_CCW);
        tableDirection = 'i';
        Serial.println("tccw");
      }
      else if (cmd == "tcw") {
        //turns table clockwise
        table.writeMicroseconds(SERVO_STOP);
        delay(100);
        table.writeMicroseconds(SERVO_MAX_CW);
        tableDirection = 'd';
        Serial.println("tcw");
      }
      else if (cmd == "ts") {
        //stops table
        table.writeMicroseconds(SERVO_STOP);
        tableDirection = 'n';
        turnTableFree = true;
        Serial.println("ts");
      }
      else if (cmd == "p1ccw") {
        //turns pump1 counter-clockwise
        digitalWrite(pump1A, HIGH);
        digitalWrite(pump1B, LOW);
        delay(100);
        Serial.println("p1ccw");
      }
      else if (cmd == "p1cw") {
        //turns pump1 clockwise
        digitalWrite(pump1A, LOW);
        digitalWrite(pump1B, HIGH);
        delay(100);
        Serial.println("p1cw");
      }
      else if (cmd == "p2ccw") {
        //turns drill counter-clockwise
        digitalWrite(pump2A, HIGH);
        digitalWrite(pump2B, LOW);
        delay(100);
        Serial.println("p2ccw");
      }
      else if (cmd == "p2cw") {
        //turns drill clockwise
        digitalWrite(pump2A, LOW);
        digitalWrite(pump2B, HIGH);
        delay(100);
        Serial.println("p2cw");
      }
      else if (cmd == "ps") {
        //stops pumps
        digitalWrite(pump1A, LOW);
        digitalWrite(pump1B, LOW);
        digitalWrite(pump2A, LOW);
        digitalWrite(pump2B, LOW);
        Serial.println("ps");
      }
      else if (cmd == "led1") {
        digitalWrite(led1, HIGH);
        delay(100);
        val = analogRead(photoresistor);
        voltage = val * (5.0 / 1023.0);

        Serial.print("Voltage On =");
        Serial.print(voltage);
        Serial.println("led1");
      }
      else if (cmd == "led1s") {
        digitalWrite(led1, LOW);
        delay(100);
        val = analogRead(photoresistor);
        voltage = val * (5.0 / 1023.0);

        Serial.print("Voltage Off =");
        Serial.print(voltage);
        Serial.println("led1s");
      }
      else if (cmd == "led2") {
        digitalWrite(led2, HIGH);
        delay(100);
        val = analogRead(photoresistor);
        voltage = val * (5.0 / 1023.0);

        Serial.print("Voltage On =");
        Serial.print(voltage);
        Serial.println("led2");
      }
      else if (cmd == "led2s") {
        digitalWrite(led2, LOW);
        delay(100);
        val = analogRead(photoresistor);
        voltage = val * (5.0 / 1023.0);

        Serial.print("Voltage Off =");
        Serial.print(voltage);
        Serial.println("led2s");
      }

      else if (cmd == "deactivate") {
        //stops all
        isActivated = false;
        table.writeMicroseconds(SERVO_STOP);
        analogWrite(elevator, 0);
        analogWrite(drill, 0);
        digitalWrite(pump1A, LOW);
        digitalWrite(pump1B, LOW);
        digitalWrite(pump2A, LOW);
        digitalWrite(pump2B, LOW);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        turnTable (0, 0);
        tableDirection = 'n';
        turnTableFree = true;
        previousElevatorState = 'n';
        Serial.print("cmd: ");
        Serial.println(cmd);
      }
    }
  }
  // this works but doesn't consider the possibility of both switches
  // being triggered at the same time which shouldn't ever actually happen
  if (isTriggered) {
    if ( (millis() - triggerTime) >= TRIGGER_DELAY) {
      // if the last interrupt was a press (meaning it's stabilized and in contact)
      // then there's a real press
      Serial.println("preContacted");
      if (isContacted) {
        isActualPress = true;   // otherwise it's not a real press
      }                         // so the limit switch state should stay whatever it used to be
                                // and so should actualPress
      isTriggered = false;      // either way, we should reset the triggered bool in wait for the next trigger
    }
  }
  if (isActualPress) {
    Serial.println("isContacted");
    Serial.println(isContacted);
    Serial.println(previousElevatorState);
    if (previousElevatorState == 'n')cuvettePosition();
    else if (previousElevatorState == 'u')elevatorBottomInterrupt();
    else if (previousElevatorState == 'd')elevatorBottomInterrupt();
  }
  // now that the behaviour is complete we can reset these in wait for the next trigger to be confirmed
  isActualPress = false;

  if ((turnTableFree == false) && (tablePosition[desiredPosition] == cuvette)) {
    table.writeMicroseconds(SERVO_STOP);
    tableDirection = 'n';
    turnTableFree = true;
  }
}
/*
  void elevatorTopInterrupt () {
  //stops elevator
  unsigned long timer = millis();

  analogWrite(elevator, 0);
  digitalWrite(elevator_direction, LOW);
  analogWrite(elevator, maxVelocity);
  while ((millis() - timer) < 500) {
    ;
  }
  analogWrite(elevator, 0);
          previousElevatorState = 'n';
  }
*/
void elevatorBottomInterrupt () {
  //stops elevator
  Serial.print("\ninside function");
  unsigned long timer = millis();

  analogWrite(elevator, 0);
  digitalWrite(elevator_direction, HIGH);
  analogWrite(elevator, maxVelocity);
  while ((millis() - timer) < 500) {
    ;
  }
  analogWrite(elevator, 0);
  previousElevatorState = 'n';
}

void cuvettePosition() {
  //gives the integer value of the cuvette of the table 1 to 12, chute is cuvettePosition 0
  if (tableDirection == 'n') {
  }
  else if (tableDirection == 'i') {
    for (i = 0; i <= 12; i++) {
      tablePosition[i] = (tablePosition[i] + 1) % 13;
    }
    Serial.print("tablePosition[0]: ");
    Serial.println(tablePosition[0]);
  }
  else if (tableDirection == 'd') {
    int temp = tablePosition[0];
    for (i = 0; i <= 12; i++) {
      tablePosition[i] = (tablePosition[i] - 1) % 13;
      if (tablePosition[i] == -1) {
        tablePosition[i] = 12;
      }
    }
    Serial.print("tablePosition[0]: ");
    Serial.println(tablePosition[0]);
  }
}

void turnTable (int cuvette, int desiredPosition) {
  int initialPosition = 0;
  int difference = 0;

  for (i = 0; i <= 12; i++) {
    initialPosition = i;
    if (tablePosition[i] == cuvette)break;
  }

  Serial.print("initialPosition: ");
  Serial.println(initialPosition);

  difference = desiredPosition - initialPosition;

  Serial.print("difference: ");
  Serial.println(difference);

  if ( (difference > -7 && difference < 0) || (difference > 7 && difference < 13)) {
    tableDirection = 'i';
    table.writeMicroseconds(SERVO_MAX_CCW);
  }
  else if ( (difference > -13 && difference < -7) || (difference > 0 && difference < 7)) {
    tableDirection = 'd';
    table.writeMicroseconds(SERVO_MAX_CW);
  }
  turnTableFree = false;
}

void debouncing(void) {
  // if this is the first time the switch was pressed in a while,
  // alert loop() that the switch was pressed and set up the timer
  if (!isTriggered && !isContacted) {
    isTriggered = true;
    triggerTime = millis();
  }

  /* every interrupt, update isContacted based on the pin state */
  // if the contact is connected, the pin will read low so set isContacted to true
  if (previousElevatorState == 'n') {
    if (digitalRead(TABLE_SWITCH_PIN) == LOW) {
      isContacted = true;
      Serial.println("minus");
    }
    // otherwise the contact is bouncing so set it to false
    else if (digitalRead(TABLE_SWITCH_PIN) == HIGH) {
      isContacted = false;
      Serial.println("plus");
    }
  }
  else if (previousElevatorState == 'u') {
    if (digitalRead(LIMIT_TOP) == LOW) {
      isContacted = true;
      Serial.println("minus");
    }
    // otherwise the contact is bouncing so set it to false
    else if (digitalRead(LIMIT_TOP) == HIGH) {
      isContacted = false;
      Serial.println("plus");
    }
  }
  else if (previousElevatorState == 'd') {
    if (digitalRead(LIMIT_BOTTOM) == LOW) {
      isContacted = true;
      Serial.println("minus");
    }
    // otherwise the contact is bouncing so set it to false
    else if (digitalRead(LIMIT_BOTTOM) == HIGH) {
      isContacted = false;
      Serial.println("plus");
    }
  }
}


int drill_speed(int input_drill_speed) {
  if (input_drill_speed < 0) {
    input_drill_speed = 0;
  }
  if (input_drill_speed > 100) {
    input_drill_speed = 100;
  }
  return (input_drill_speed * 165 / 100);
}

float elevator_feed(int input_elevator_feed) {
  if (input_elevator_feed < 0) {
    input_elevator_feed = 0;
  }
  if (input_elevator_feed > 100) {
    input_elevator_feed = 0.1066;
  }
  return input_elevator_feed * 0.1066 / 100;
}


