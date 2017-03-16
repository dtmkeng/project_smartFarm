#define tByte   unsigned char   // 8  Bits
#define tWord   unsigned int    // 16 Bits
#define tDWord  unsigned long   // 32 Bits

#define tFloat  float

#define bNull   0xFF
#define wNull   0xFFFF
#define dNull   0xFFFFFFFF
#define pNull   NULL

boolean schZone,con;       /* Dividing 2 scheduling spaces
                        true = normal scheduling
                        false = timer scheduling */

//== Switch States =====================
#define sON    1       // Switch state=ON
#define sOFF   0       // Switch state=OFF
#define sIDL   bNull   // Switch state=IDLE


