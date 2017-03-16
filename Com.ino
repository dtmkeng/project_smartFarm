#define  BUF_SIZE  20

typedef struct {
  tByte   buf[BUF_SIZE];
  tByte   head;
  tByte   tail;
} tComBuf;

typedef struct {
  tByte   state;
  tByte   cmd;
} tCom;

tCom m_com;
tComBuf cbuf;

//= Local functions ==========================================================

tByte fComIsBufFull(void) {
  tByte   next;

  next=(tByte)((cbuf.head+1)%BUF_SIZE);
  if (next==cbuf.tail) return 1;        // Yes
  return 0;                             // No
}

tByte fComIsBufEmpty(void) {
  if (cbuf.head==cbuf.tail) return 1;   // Yes
  return 0;                             // No
}

tByte fComBufIn(tByte c) {
  if (fComIsBufFull()) return bNull;
  cbuf.buf[cbuf.head]=c;
  cbuf.head=(tByte)((cbuf.head+1)%BUF_SIZE);  
  return 0;
}

tByte fComBufOut(void) {
  tByte   c;

  if (fComIsBufEmpty()) return bNull;
  c=cbuf.buf[cbuf.tail];
  cbuf.tail=(tByte)((cbuf.tail+1)%BUF_SIZE);
  return c;
} 

//- State Machine ----------------------------------

tByte fComState0(tCom *m) {

  if (Serial.available()>0) {
    if (fComBufIn(Serial.read())==bNull) return 0;
  }

  return 0;
}

tByte fComEntry(tCom *m) {

  switch (m->state) {
    case 0 : fComState0(m); break;
    //case 1 : fComState1(m); break;
    default : break;
  }
  return 0;
}

//---------------------------------------------------

tByte fComOpen(void) {

  //-------------------------
  m_com.state=0;

  return 0;
}

void fComRun(void) {

  fComEntry(&m_com);
}



