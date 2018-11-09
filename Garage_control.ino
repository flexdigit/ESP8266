/*
 * from
 * http://esp8266-server.de/
 * 
 * Webserver as Accesspoint for ESP8266
 * 
 * Free for anyone!!!
 * 
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// configure Wifi
const char* SSID = "*************";
const char* PW   = "*************";          // set to "" for open access point w/o password

IPAddress ip(192,168,178,38);
IPAddress gateway(192,168,178,1);
IPAddress subnet(255,255,255,0);

// configure IOs
const int GPIO0 = 0;
const int GPIO2 = 2;
const int GPIO4 = 4;                    // For ESP8266-12E usage

int val = LOW;                          // "LOW/0" means switched 'off' on setup
int color = 0;                          // just to change the color of the action button
String Temp = "";

String html1 = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<meta content=\"text/html;\
charset=ISO-8859-1\" http-equiv=\"content-type\">\r\n<title>Garage\
control</title>\r\n</head>\r\n<body>\r\nPush the button to toggle \
the door opener!\r\n<br>\r\n<br>\r\n<form action=\"";

String html2 = "\">\r\n<input value=\"Open/Close\" style=\"";
String html3 = "width:14em;height:12em; font-size:24px;\" type=\"\
submit\"></form>\r\n</body>\r\n</html>";

ESP8266WebServer server(80);            // Setup server port

void WiFiconnect();                     // Prototypes

void Event_Index()                      // Executed if "http://<ip address>/" is called
{
    Temp  = html1 + "/toggle.htm";
    Temp += html2 + String((val) ? "BACKGROUND-COLOR: LightBlue;" : "BACKGROUND-COLOR: LightCoral;") + html3;
    server.send(200, "text/html", Temp);
}

void Event_Toggle()                     // Executed if "http://<ip address>/toggle.htm" is called
{
    // generate the html page and dispatch it to the client
    color =! color;
    Temp  = html1 + "/toggle.htm";
    Temp += html2 + String((color) ? "BACKGROUND-COLOR: LightBlue;" : "BACKGROUND-COLOR: LightCoral;") + html3;
    server.send(200, "text/html", Temp);
    
    // handle the LED/relay and do some serial print lines...
    val =! val;                         // toggle on
    Serial.print(F("LED is: "));
    Serial.print(val);
    digitalWrite(GPIO4, val);
    delay(500);                         // wait for 1/2 a secound to give the door control a chance to react
    val =! val;                         // toggle off
    Serial.println(val);
    digitalWrite(GPIO4, val);    
}

void setup()
{
    pinMode(GPIO0, INPUT_PULLUP);       // GPIO0 configured as input with Pullup
    pinMode(GPIO2, INPUT_PULLUP);       // GPIO2 configured as input with Pullup
    pinMode(GPIO4, OUTPUT);             // GPIO4 configured as output
    digitalWrite(GPIO4, val);           // initial condition 1 (Relay off)

    Serial.begin(115200);               // init serial inteface

    WiFiconnect();                      // Wifi settings

    Serial.println(F(""));              // You can pass flash-memory based strings to
    Serial.println(F("ESP8266-01...")); // Serial.print() by wrapping them with F().
    Serial.print(F("SDK-Version:             "));
    Serial.println(ESP.getSdkVersion());
    Serial.print(F("ESP8266 Chip-ID:         "));
    Serial.println(ESP.getChipId());
    Serial.print(F("ESP8266 Speed in MHz:    "));
    Serial.println(ESP.getCpuFreqMHz());
    Serial.print(F("Free Heap Size in Bytes: "));
    Serial.println(ESP.getFreeHeap());

    //Serial.print(F("getResetReason: "));
    //Serial.println(ESP.getResetReason());
    Serial.print(F("getFlashChipId:          "));
    Serial.println(ESP.getFlashChipId());
    Serial.print(F("getFlashChipSize:        "));
    Serial.println(ESP.getFlashChipSize());
    Serial.print(F("getFlashChipSpeed:       "));
    Serial.println(ESP.getFlashChipSpeed());
    Serial.print(F("getCycleCount:           "));
    Serial.println(ESP.getCycleCount());

    
    // treatment of the different events - before server.begin()!!!
    server.on("/", Event_Index);
    server.on("/toggle.htm", Event_Toggle);
    
    server.begin();                     // start the server
    Serial.println();
    Serial.println(F("HTTP Server gestartet.."));
    Serial.println();
}
 
void loop()
{
    server.handleClient();              // check for incomming client connections frequently in the main loop
    delay(1);
}

void WiFiconnect() {

    Serial.print(F("Start WLAN-Hotspot: "));
    Serial.println(SSID);
    
    WiFi.mode(WIFI_AP);                 // set access point modus
    WiFi.softAP(SSID,PW);
    WiFi.softAPConfig(ip, gateway, subnet);
     
    delay(500);                         // wait 0,5s
    
    Serial.print(F("IP Address: "));    // print of the current IP of the server
    Serial.println(WiFi.softAPIP());
    
    Serial.print(F("MAC Address: "));    // print of the MAC of the server
    Serial.println(WiFi.softAPmacAddress());
}
