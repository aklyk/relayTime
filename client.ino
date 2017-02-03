// client.pde
//
// Simple example of how to use VirtualWire to send and receive messages
// with a DR3100 module.
// Send a message to another arduino running the 'server' example, which
// should send a reply, which we will check
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: client.pde,v 1.1 2008/04/20 09:24:17 mikem Exp $

#include <VirtualWire.h>

#define relayPin 2

int count = 30;
byte timeOnHr = 16;
byte timeOnMin = 10;
byte timeOffHr = 01;
byte timeOffMin = 00;
int timeNow;
int timeOn = timeOnHr*60+timeOnMin;
int timeOff = timeOffHr*60+timeOffMin;

void setup()
{
    Serial.begin(9600);	// Debugging only
    vw_set_rx_pin(11);    
    pinMode(3, OUTPUT);
    pinMode(relayPin, OUTPUT);
    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);

    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{  
  uint8_t buf[VW_MAX_MESSAGE_LEN]; // Буфер для сообщения
  uint8_t buflen = VW_MAX_MESSAGE_LEN; // Длина буфера
  
  int hr = 0;
  int min = 0;
  int sec = 0;
  int day = 0;
  int mon = 0;
  int yr = 0;

  if (vw_get_message(buf, &buflen))
  {
    //for (int i = 0; i<=7; i++) {
     // if (buf[i]-48 != 10) {
     //   Serial.print(buf[i]-48);
      // }
     // else {
     //   Serial.print(":");
     // }
  //  }
   // Serial.println();

    hr = 0; min = 0; sec = 0; day = 0; mon = 0; yr = 0;
    
    for (int i = 0; i<=1; i++) {hr *= 10; hr += buf[i]-48;};
    for (int i = 3; i<=4; i++) {min *= 10; min += buf[i]-48;};
    for (int i = 6; i<=7; i++) {sec *= 10; sec += buf[i]-48;};
    
    for (int i = 17; i<=18; i++) {day *= 10; day += buf[i]-48;};
    for (int i = 14; i<=15; i++) {mon *= 10; mon += buf[i]-48;};
    for (int i = 9; i<=12; i++) {yr *= 10; yr += buf[i]-48;};

    Serial.print(hr); Serial.print(":"); Serial.print(min); Serial.print(":"); Serial.print(sec); Serial.print(" ");
    Serial.print(day); Serial.print("/"); Serial.print(mon); Serial.print("/"); Serial.print(yr);
    Serial.println();
    count = 30;

  timeNow = hr*60+min;
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
  
  count--;
  Serial.println(count);
  if (count <= 0) {
    digitalWrite(relayPin, LOW);
    beep(200);
    beep(200);
    beep(200);
  }
  delay(1000);
  
}

void beep(unsigned char delayms){
  analogWrite(3, 20);      // значение должно находится между 0 и 255
                           // поэкспериментируйте для получения хорошего тона
  delay(delayms);          // пауза delayms мс
  analogWrite(3, 0);       // 0 - выключаем пьезо
  delay(delayms);          // пауза delayms мс   
}
