


#define LED 13
#define serial_port Serial



unsigned long t_now = 0;
unsigned long t_last = 0;
long time_interval = 500;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  serial_port.begin(9600);
  }

void loop() {
  // put your main code here, to run repeatedly:
  t_now = millis();
  if (t_now - t_last > time_interval)
  {
    digitalWrite(LED, !digitalRead(LED));
    t_last = t_now;

  }
  
}
