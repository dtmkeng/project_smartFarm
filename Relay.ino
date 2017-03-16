typedef struct {
  tByte   state;
  tByte   cnt;
  tByte   cntOn;
  tByte   cntOff;
} tRelay;

tRelay  m_Relay;

//- State Machines ----------------------------------

tByte fRelayState0(tRelay *m) {


  return 0;
} 

tByte fRelayState1(tRelay *m) {

    
  return 0;
}

//---------------------------------

tByte fRelayState100(tRelay *m) {

  m->cnt++;
  if (m->cnt>m->cntOn) {
    digitalWrite(RELAY,HIGH);
    m->cnt=0;
    m->state=101;
  }

  return 0;
} 

tByte fRelayState101(tRelay *m) {

  m->cnt++;
  if (m->cnt>m->cntOff) {
    digitalWrite(RELAY,LOW);
    m->cnt=0;
    m->state=100;
  }
    
  return 0;
}

tByte fRelayEntry(tRelay *m) {

  if (schZone) { // running in normal scheduling
    switch (m->state) {
      case 0 : fRelayState0(m); break;
      case 1 : fRelayState1(m); break;
      default : break;
    }
  } else {       // running in timer scheduling 
    switch (m->state) {
      case 100 : fRelayState100(m); break;
      case 101 : fRelayState101(m); break;
      default : break;  
    }  
  }
  return 0;
}

//--------------------------------------------------

void fRelaySet(tByte m,tByte n) {
  m_Relay.cntOn=m;
  m_Relay.cntOff=n;
  m_Relay.cnt=0; 
  m_Relay.state=100;
}

void fRelayStop(void) {
  digitalWrite(RELAY,LOW);
  m_Relay.state=bNull;
}
void fRelayOn(void) {
  digitalWrite(RELAY,HIGH);
 // m_Relay.state=b;
}

//--------------------------------------------------

tByte fRelayOpen(void) {

  //-------------------------
  m_Relay.state=bNull;

  return 0;
}

void fRelayRun(void) {

  fRelayEntry(&m_Relay);
}
