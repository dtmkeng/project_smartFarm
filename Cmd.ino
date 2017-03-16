#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>

#define CMD_LEN_MAX   10  // Max length of commands
#define CMD_LEN_FIX   6//4   // Length of self-defined commands 

//---------------------------------

typedef struct {
  tByte   state;
  char    cmd[CMD_LEN_MAX]; 
  //tFloat  temp;// String storing an instruction
  tByte   cmd_i;         // Index for string cmd
} tCmd;

tCmd  m_cmd;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
//DS1302 rtc(8, 7, 6);

//- Functions -------------------------------------

void fCmdHelp(void) {
  Serial.println("");
  Serial.println("** MyLittlePLC -------------------");
  Serial.println("  GETT : Get Temperature");
  Serial.println("  GETH : Get Humidity");
  Serial.println("  RYOF : Turn off Relay");
  Serial.println("  RYON : Turn on Relay");
  Serial.println("  GETTI : Get Time");
  Serial.println("  GETDA : Get Date");
  Serial.println("  RYSET : Set Relay");
  Serial.println("  RYSTP : Stop Relay");  
  Serial.println("----------------------------------");
}

tByte fCmdStrLen(char *s) {
  tByte   i;

  for (i=0;i<CMD_LEN_MAX;i++) if (s[i]=='\0') return i;
  return bNull;
}

tByte fCmdApp(tCmd *m,char c) {

  if (m->cmd_i>=CMD_LEN_MAX-1) return bNull;    // Too long
  m->cmd[m->cmd_i]=c;
  m->cmd_i++;
  return 0;
}

tByte fCmdCmp(tCmd *m,char *s) {
  tByte   i,len,l;

  if (m->cmd_i-1>CMD_LEN_FIX) return 0;
  l=fCmdStrLen(s);if (l==bNull) return 0;
  if (m->cmd_i-1>l) len=m->cmd_i-1; else len=l; 
  for (i=0;i<len;i++) {
    if (m->cmd[i]!=s[i]) return 0;
  }
  return 1;
}

tFloat fCmdGetT(void) {
  float t = dht.readTemperature();
  Serial.print("Temp = ");
  Serial.println(t);

  lcd.clear();
  lcd.setCursor(0,0);lcd.print("GETT received");
  lcd.setCursor(0,1);lcd.print(t);
  return t;
}

tFloat fCmdGetH(void) {
  float h = dht.readHumidity();
    Serial.print("Humid = ");
    Serial.println(h);

  lcd.clear();
  lcd.setCursor(0,0);lcd.print("GETH received");
  lcd.setCursor(0,1);lcd.print(h);  
  return h;
}

void fCmdRyOn(void) {
  digitalWrite(RELAY,LOW);
  Serial.println("Relay On");

  lcd.clear();
  lcd.setCursor(0,0);lcd.print("Relay On");
  
}

void fCmdRyOf(void) {
  digitalWrite(RELAY,HIGH);
  Serial.println("Relay Off");

  lcd.clear();
  lcd.setCursor(0,0);lcd.print("Relay Off");  
}

void fCmdGetTime(void) {
  String  str;

  //str=rtc.getTimeStr();
  Serial.println(str);
}

void fCmdGetDate(void) {
  String  str;

//  str=rtc.getDateStr();
  Serial.println(str);
}

//- State Machine ----------------------------------

tByte fCmdState0(tCmd *m) {
  char   c;

  if (!fComIsBufEmpty()) {
    c=(char)fComBufOut();
    if (isAlpha(c)) fCmdApp(m,c);
    if (c==0x0D) {
      fCmdApp(m,'\0');
      m->state=1;         // Check command
    }
  }
  return 0;
} 

tByte fCmdState1(tCmd *m) {

  if (fCmdCmp(m,"GETT")) {fCmdGetT();} else
  if (fCmdCmp(m,"GETH")) {fCmdGetH();} else  
  if (fCmdCmp(m,"RYON")) {fCmdRyOn();} else
  if (fCmdCmp(m,"RYOF")) {fCmdRyOf();} else
  if (fCmdCmp(m,"GETTI")) {fCmdGetTime();} else
  if (fCmdCmp(m,"GETDA")) {fCmdGetDate();} else
  if (fCmdCmp(m,"RYSET")) {fRelaySet(3,1);} else
  if (fCmdCmp(m,"RYSTP")) {fRelayStop();} else  
  if (fCmdCmp(m,"H")) {fCmdHelp();}
  else {Serial.println("Invalid command, man!");}

  m->cmd_i=0;
  m->state=0;         
  return 0;
}
//tByte fstateContorl1(tCmd *){
//  tFloat temp = fCmdGetT();
//  tFloat hum = fCmdGetH();
//  }
//tByte fstateContorl0(tCmd *){
//  tFloat temp = fCmdGetT();
//  tFloat hum = fCmdGetH();
//  }
tByte fCmdEntry(tCmd *m) {
  if(con){
    fCmdRyOn();
    }if(con==false){
      fCmdRyOf();
      }
  switch (m->state) {
    case 0 : fCmdState0(m); break;
    case 1 : fCmdState1(m); break;
    default : break;
  }
  return 0;
}

//--------------------------------------------------

void fCmdNoBacklight(void) {
  lcd.noBacklight();
}

void fCmdBacklight(void) {
  lcd.backlight();
}

//--------------------------------------------------

tByte fCmdOpen(void) {

  pinMode(RELAY,OUTPUT);// Relay
  dht.begin();           // DHT22 begins
  lcd.begin();           // LCD begins
  lcd.backlight();       // LCD's backlight on
  
//  rtc.halt(false);
//  rtc.writeProtect(false);
//  rtc.setDOW(SATURDAY);       // Set Day-of-Week to FRIDAY
  //rtc.setTime(10, 50, 0);     // Set the time (24hr format)
  //rtc.setDate(25, 2, 2017);   // Set the date (d/m/y)
  //-------------------------
  fCmdHelp();
  fCmdRyOf();

  m_cmd.cmd_i=0;
  //m_cmd.temp=t;
  //-------------------------
  m_cmd.state=0;

  return 0;
}

void fCmdRun(void) {
  fCmdEntry(&m_cmd);
}
