//--------------------------------------------------------------------------
// Wifi Modul ESP8266 Parameter als Accesspoint setzen
//--------------------------------------------------------------------------
// Basis war das Beispiel von Ray Wang  http://rayshobby.net/?p=9734
//--------------------------------------------------------------------------

#define Debug_Serial Serial     // For debugging purpose on the serial monitor
//#define EspSerial mySerial      // For command purpose for the ESP8266


#include <SoftwareSerial.h>
SoftwareSerial EspSerial(10,11); // RX, TX

#define SSID  "astral"     // WiFi SSID
#define PASS  "12345678"  // WiFi Passwort
#define WIFI_CANNEL 8
#define WIFI_SECURE 3
// 0 = offen ohne Passwort
// 1 = WEB
// 2 = WPA_PSK
// 3 = WPA2_PSK
// 4 = WPA_WPA2_PSK

#define WIFI_ERROR_NONE 0
#define WIFI_ERROR 1

//---------------------------------------------------------------------------
int WIFI_Setup_Error = 0;
#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];
//---------------------------------------------------------------------------
// LED Handling
#define LEDMODE_AUS 0
#define LEDMODE_EIN 1
#define LEDMODE_BLINK 2
#define LEDPIN 13
int led_value=LOW;
int led_mode=LEDMODE_AUS;
unsigned long led_nextpuls=0;
int led_pulsdauer = 500;

//-----------------------------------------------------------------------
// prototypes

int WIFI_Setup();
void Led_Handling();
void Set_Led(int p);

//--------------------------------------------------------------------------------------
void setup() {
  pinMode(LEDPIN,OUTPUT);

  Debug_Serial.begin(115200);
  Debug_Serial.setTimeout(5000);        // What for????

  //EspSerial.begin(9600);
  EspSerial.begin(115200);
  //----------------------------------
  // 1 sec warten
  delay (1000);

  Set_Led(HIGH);

  WIFI_Setup_Error = WIFI_Setup();

  if (WIFI_Setup_Error) {
    Debug_Serial.print("WIFI_Setup_Error: ");
    Debug_Serial.println(WIFI_Setup_Error);
    Debug_Serial.println("Error in WIFI_Setup()...");
  }

  if (WIFI_Setup_Error) led_mode=LEDMODE_BLINK;
  else                  Set_Led(LOW);

}
//--------------------------------------------------------------------------
void loop() {

  Led_Handling();
}

//----------------------------------------------------------------------
int WIFI_Setup() {

  //---------------------------------------------------------------------------
  // WiFi Modus setzen

  EspSerial.println("AT+CWMODE=2");
  Debug_Serial.println("AT+CWMODE=2");
  delay(1000);
  // Normale Antwort AT+CWMODE=1 0x0D 0x0D 0x0A OK <crlf> 

  //if(EspSerial.find("Error")){
  //  Debug_Serial.println(WIFI_ERROR);
  //  return WIFI_ERROR;
  //}

  //EspSerial.println("AT+CIPMUX=1");
  //Debug_Serial.println("AT+CIPMUX=1");
  //delay(1000);
  
  //EspSerial.println("AT+CIPSERVER=1,9768");
  //Debug_Serial.println("AT+CIPSERVER=1,9768");
  //delay(1000);
  
//-------------------------------------------------------------------
// Modul resetten
  EspSerial.println("AT+RST");
  Debug_Serial.println("AT+RST");
  delay(3000);

  // tut nicht
  //EspSerial.readBytesUntil('\n', buffer, BUFFER_SIZE);
  //delay(3000);
  //Debug_Serial.println(buffer);
  
  // "Normale Antwort AT+RST 0xD 0xD 0xA 0xD 0xA OK 0xD 0xA 0xD 0xA ets Jan  ... ready 0xD 0xA
  if(!EspSerial.find("ready")) {
    return WIFI_ERROR;
  }
  Debug_Serial.println("So!");
  
  // Generiert nur Zahlen.....
  //int value = 0;
  //while ((value = EspSerial.read()) != -1)
  //{
  //    Debug_Serial.println(value);
  //    value = 0;
  //}
  
  //Debug_Serial.println("Und nuuu?");
  // ---------------------------------------------------------------
  // Netzwerk Parameter setzen
  EspSerial.print("AT+CWSAP=\"");
  EspSerial.print(SSID);
  EspSerial.print("\",\"");
  EspSerial.print(PASS);
  EspSerial.print("\",");
  EspSerial.print(WIFI_CANNEL);
  EspSerial.print(",");
  EspSerial.println(WIFI_SECURE);
  delay(1000);

  //Debug_Serial.println(EspSerial.read());
  if(!EspSerial.find("OK")){
    Debug_Serial.println("Bin hier!!!");
    return WIFI_ERROR;
  }
  else Debug_Serial.println("Kein Fehler");
  
  Debug_Serial.print("AT+CWSAP=\"");
  Debug_Serial.print(SSID);
  Debug_Serial.print("\",\"");
  Debug_Serial.print(PASS);
  Debug_Serial.print("\",");
  Debug_Serial.print(WIFI_CANNEL);
  Debug_Serial.print(",");
  Debug_Serial.println(WIFI_SECURE);
  delay(1000);

  // get IP
  //Debug_Serial.print(F("ip address : "));
  //Debug_Serial.print("ip address : ");
  //Debug_Serial.println( GetResponse("AT+CIFSR", 10) );
  //delay(1000);

  Debug_Serial.println("Setup okay");

  return WIFI_ERROR_NONE;
}

//--------------------------------------------------------------------
// Get the data from the WiFi module and send it to the debug serial port
String GetResponse(String AT_Command, int wait){
  String tmpData;
  
  EspSerial.println(AT_Command);
  delay(1000);
  while (EspSerial.available() >0 )  {
    char c = EspSerial.read();
    tmpData += c;
    
    if ( tmpData.indexOf(AT_Command) > -1 )         
      tmpData = "";
    else
      tmpData.trim();       
  }
  return tmpData;
}

//--------------------------------------------------------------------
void Led_Handling(){

  if (led_mode==LEDMODE_AUS && led_value != LOW) Set_Led(LOW);
  if (led_mode==LEDMODE_EIN && led_value != HIGH) Set_Led(HIGH);
  if (led_mode==LEDMODE_BLINK && millis() > led_nextpuls){ 
    led_nextpuls = millis()+(unsigned long)led_pulsdauer;
    if (led_value == LOW) Set_Led(HIGH);
    else Set_Led(LOW);
  }
}

//--------------------------------------------------------------------
void Set_Led(int p){
  led_value = p;
  digitalWrite(LEDPIN,led_value);
}



