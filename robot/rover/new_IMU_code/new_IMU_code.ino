#include <elapsedMillis.h>
#include <SoftwareSerial.h>


#define LED 13
#define imu_serial Serial5
#define dev_serial Serial
elapsedMillis timeElapsed;

String imu_msg;
long time_interval = 500;
char cmd;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  imu_serial.begin(115200);
  // imu_serial.setTimeout(20);
  dev_serial.begin(115200);
  // dev_serial.setTimeout(20);
  delay(300);
  imu_serial.write(" ");
  }

void loop() {
  // put your main code here, to run repeatedly:

  if (timeElapsed > time_interval)
  {
    digitalWrite(LED, !digitalRead(LED));
    timeElapsed = 0;
  }

  if (dev_serial.available() > 0)
  {
    cmd = dev_serial.read();
    imu_serial.write(cmd);
  }

  if(imu_serial.available() > 0)
  {
    
    imu_msg = imu_serial.readStringUntil('\n');
    dev_serial.println(imu_msg);
  }  
  


}
