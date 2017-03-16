#include "MPublic.h"
#include <TimerOne.h>
#include "Test.h"
#include "Key.h"
#include "KCon.h"
#include "Com.h"
#include "Cmd.h"
#include "Relay.h"
#define vcc1 12
#define vcc2 11
#define vcc3 10
#define Humiset 40
#define Tempset 34
float temp=0;
float humi=0;
void fTimerIsr(void);
void setup() {
  //mapPin
  pinMode(vcc1,OUTPUT);pinMode(vcc2,OUTPUT);pinMode(vcc3,OUTPUT);
  digitalWrite(vcc1,HIGH);
  digitalWrite(vcc2,HIGH);
  digitalWrite(vcc3,HIGH);
  Serial.begin(9600);
  Timer1.initialize(1000000);         // set timer=1000000 us = 1s 
  Timer1.attachInterrupt(fTimerIsr);  // attach ISR here 
  //------------
  //fTestOpen();
  fKeyOpen();
  fKConOpen();
  fComOpen();
  fCmdOpen();
  fRelayOpen();
 
}

void loop() {
  schZone = true;
  fCmdRun();
  fRelayRun();
  //fTestRun();
  //fKeyRun();
  //fKConRun();
  humi = fCmdGetH();
  humi = fCmdGetT();
  if(temp<38){
    Serial.println("Check.....");
    con =true;
    }else {Serial.println("Check ERROR...");
        con =false;
    }
 // fComRun();
  
  //fRelayStop();
}

void fTimerIsr(void) {

  schZone = false;
  //fTestRun();
  fRelayRun();
}



