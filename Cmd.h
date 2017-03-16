extern "C" {tByte fCmdOpen(void);}
extern "C" {void fCmdRun(void);}

extern "C" {void fCmdNoBacklight(void);}
extern "C" {void fCmdBacklight(void);}
extern "C" {tFloat fCmdGetH(void);}
extern "C" {tFloat fCmdGetT(void);}

//- I/Os ------------------------------------
#define RELAY 9           // Relay
#define DHTPIN 2          // Digital pin connected to DHT22's OUT pin
#define DHTTYPE DHT11     // DHT22
