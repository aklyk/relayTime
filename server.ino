//http://christianscode.blogspot.com/2015/07/arduino-timestamp-function.html
//https://github.com/max246/blogscripts/blob/master/arduino/time/time.ino

#include <stdio.h>
#include <DS1302.h>
#include <VirtualWire.h>

#define relayPin 12

namespace {

const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock

byte timeOnHr = 16;
byte timeOnMin = 10;
byte timeOffHr = 01;
byte timeOffMin = 00;
int timeNow;
int timeOn = timeOnHr*60+timeOnMin;
int timeOff = timeOffHr*60+timeOffMin;

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Format the time and date and insert into the temporary buffer.
  char ddate[30];
  char ttime[20];
  char secondRelay[50];
  snprintf(ddate, sizeof(ddate), "%04d-%02d-%02d",
           t.yr, t.mon, t.date);
  snprintf(ttime, sizeof(ttime), "%02d:%02d:%02d",
           t.hr, t.min, t.sec);
  secondRelay[0] = 0;  
  strcat(secondRelay,ttime);
  strcat(secondRelay," ");
  strcat(secondRelay,ddate);         
  //Serial.println(ddate);
  //Serial.println(ttime);
  Serial.println(secondRelay);
 

  vw_send((uint8_t *)secondRelay, strlen(secondRelay));
  vw_wait_tx(); // Ждем пока передача будет окончена

  
  timeNow = t.hr*60+t.min;
  int conversion = 0;
  if (timeOffHr-timeOnHr < 0) conversion = 1;
  else conversion = 1;

      if (conversion == 1 && timeNow >= timeOn || timeNow <= timeOff)
      {
        digitalWrite(relayPin, HIGH);
        Serial.println("relay is on");
      }
      else if (conversion == 1) {
        digitalWrite(relayPin, LOW);
      }; 
      
       if (conversion == 0 && timeNow >= timeOn && timeNow <= timeOff)
      {
        digitalWrite(relayPin, HIGH);
        Serial.println("relay is on");
      }
      else if (conversion == 0) {
        digitalWrite(relayPin, LOW);
      }; 
}

}  // namespace

void setup() {
  digitalWrite(relayPin, LOW);
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);
  
  vw_set_tx_pin(9); //9
  vw_set_ptt_inverted(true); // Необходимо для DR3100
  vw_setup(2000); // Устанавливаем скорость передачи (бит/с)
  
  //rtc.writeProtect(false);
  //rtc.halt(false);

  // Make a new time object to set the date and time.
  // Sunday, September 22, 2013 at 01:38:50.
  //Time t(2016, 12, 23, 17, 58, 00, Time::kSunday);

  // Set the time and date on the chip.
  //rtc.time(t);
}

// Loop and print the time every second.
void loop() {
  printTime();
  delay(1000);
}


