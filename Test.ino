// Template for state machines

typedef struct {
  tByte   state;
 
} tTest;

tTest  m_test;

//- State Machines ----------------------------------

tByte fTestState0(tTest *m) {


  return 0;
} 

tByte fTestState1(tTest *m) {

    
  return 0;
}

//---------------------------------

tByte fTestState100(tTest *m) {


  return 0;
} 

tByte fTestState101(tTest *m) {

    
  return 0;
}

tByte fTestEntry(tTest *m) {

  if (schZone) { // running in normal scheduling
    switch (m->state) {
      case 0 : fTestState0(m); break;
      case 1 : fTestState1(m); break;
      default : break;
    }
  } else {       // running in timer scheduling 
    switch (m->state) {
      case 100 : fTestState100(m); break;
      case 101 : fTestState101(m); break;
      default : break;  
    }  
  }
  return 0;
}

//--------------------------------------------------

tByte fTestOpen(void) {


  //-------------------------
  m_test.state=bNull;

  return 0;
}

void fTestRun(void) {

  fTestEntry(&m_test);
}
