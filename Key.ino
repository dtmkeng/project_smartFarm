#define MAX_KEY   2     // # of keys
#define BUF_SIZE  4     // Buffer Size
#define MAX_DBC   10    // # of deboucing

typedef struct {
  tByte   buf[BUF_SIZE];
  tByte   head;
  tByte   tail;
} tBuf;

typedef struct {
  tByte   state;

  tWord   port;
  tByte   id;
  tByte   sts;   // Key status: 1=close(on), 0=open(off)
  tWord   cnt;
  tWord   ver;
  tByte   sem;
} tKey;


tKey m_key[MAX_KEY];
tBuf kbuf;

//= functions ================================================================

tByte fKeyIsBufFull(void) {
  tByte   next;

  next=(tByte)((kbuf.head+1)%BUF_SIZE);
  if (next==kbuf.tail) return 1;
  return 0;
}

tByte fKeyIsBufEmpty(void) {
  if (kbuf.head==kbuf.tail) return 1;
  return 0;
}

tByte fKeyBufIn(tByte id) {
  if (fKeyIsBufFull()) return bNull;
  kbuf.buf[kbuf.head]=id;
  kbuf.head=(tByte)((kbuf.head+1)%BUF_SIZE);  
  return 0;
}

tByte fKeyBufOut(void) {
  tByte   k;

  if (fKeyIsBufEmpty()) return bNull;
  k=kbuf.buf[kbuf.tail];
  kbuf.tail=(tByte)((kbuf.tail+1)%BUF_SIZE);
  return k;
} 

//= State Machines ===========================================================

tByte fKeyState0(tKey *m) {

  if (!digitalRead(m->port)) {
    m->cnt++;
    m->ver=0;
    if (m->cnt>=MAX_DBC) {
      m->sts=sON;      // Set key state on
      m->cnt=0;
      if (m->sem) {Serial.println("On");fKeyBufIn(m->id);m->sem=0;}
    }
  } else {
    m->cnt=0;
    m->ver++;
    if (m->ver>=MAX_DBC) {
      m->ver=0;
      m->sem=1;
      m->state=1;
    }
  }
  return 0;
}

tByte fKeyState1(tKey *m) {

  if (digitalRead(m->port)) {
    m->cnt++;
    m->ver=0;
    if (m->cnt>=MAX_DBC) {
      m->sts=sOFF;     // Set key state off
      m->cnt=0;
      if (m->sem) {Serial.println("Off");fKeyBufIn(m->id | 0x80);m->sem=0;}
    }
  } else {
    m->cnt=0;
    m->ver++;
    if (m->ver>=MAX_DBC) {
      m->ver=0;
      m->sem=1;
      m->state=0;
    }   
  }
  return 0;
}

tByte fKeyEntry(tKey *m) {

  switch (m->state) {
    case 0 : fKeyState0(m);break;
    case 1 : fKeyState1(m);break;
    default : break;
  }
  return 0;
}


//= Public functions =========================================================

tByte fKeyHit(void) {
  if (fKeyIsBufEmpty()) return 0; else return 1;
}

tByte fKeyGet(void) {
  return fKeyBufOut();
}

tByte fKeyAreAllKeyRdy(void) {
  tByte   i;

  for (i=0;i<MAX_KEY;i++) {
    if (m_key[i].sts==sIDL) return 0;
  }

  return 1;
}

// Read Key Status, k=key number
tByte fKeyRd(tByte k) {
  if (k>MAX_KEY-1) return bNull;
  return m_key[k].sts;
}

//------------------------------------

tByte fKeyOpen(void) {
  tByte   i;

  pinMode(KEY0,INPUT_PULLUP);
  pinMode(KEY1,INPUT_PULLUP);
    
  //- Assign Key Port ------
  m_key[0].port=KEY0;
  m_key[1].port=KEY1; 
  //m_key[2].port=KEY2;
  //m_key[3].port=KEY3;
  
  //- Initialize -----------
  kbuf.head=0;
  kbuf.tail=0;
  for (i=0;i<MAX_KEY;i++) {
    if (digitalRead(m_key[i].port)) m_key[i].state=1; else m_key[i].state=0;
    m_key[i].id=i;
    m_key[i].sts=sIDL;
    m_key[i].cnt=0;
    m_key[i].ver=0;
    m_key[i].sem=1;
  }

  return 0;
}

void fKeyRun(void) {
  tByte   i;

  for (i=0;i<MAX_KEY;i++) {
    fKeyEntry(&m_key[i]);
  }
}
