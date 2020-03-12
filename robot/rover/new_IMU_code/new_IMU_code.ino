#include <elapsedMillis.h>
#include <SoftwareSerial.h>


#define LED 13
#define imu_serial Serial2
#define dev_serial Serial


// Possible commands to send to the IMU to control the data 
#define imu_cmd_switch      " "   // to stop/resume data transmission 
#define imu_cmd_time        "t"   // to enable disable time stamps (not really useful)
#define imu_cmd_acc         "a"   // accelerometer data 
#define imu_cmd_gyro        "g"   // gyroscope data 
#define imu_cmd_mag         "m"   // magnetometer data 
#define imu_cmd_calc_raw    "c"   // to switch between raw/calculated readings
#define imu_cmd_quat        "q"   // quaternion readings
#define imu_cmd_euler       "e"   // Euler angles (pitch,roll,yaw)
#define imu_cmd_heading     "h"   // heading data 
#define imu_cmd_rate        "r"   // adjust rate of data transmission
#define imu_cmd_acc_range   "A"   // adjust accelerometer full-scale imu_cmd_acc_range
#define imu_cmd__gyro_range "G"   // adjust gyroscope full-scale range
#define imu_cmd_sd          "s"   // to switch SD card data logging



elapsedMillis timeElapsed;
String imu_msg;
long time_interval = 500;
char cmd;


void imu_send_command(char);



void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  imu_serial.begin(115200);
  // imu_serial.setTimeout(20);
  dev_serial.begin(115200);
  // dev_serial.setTimeout(20);
  delay(300);
  imu_send_command(imu_cmd_switch);
  imu_send_command(imu_cmd_calc_raw);
  imu_send_command(imu_cmd_euler);
  imu_send_command(imu_cmd_heading);
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
    imu_send_command(cmd);
  }

  if(imu_serial.available() > 0)
  {
    
    imu_msg = imu_serial.readStringUntil('\n');
    dev_serial.println(imu_msg);
  }  

}




void imu_send_command(char cmd)
{
  imu_serial.write(cmd);
}