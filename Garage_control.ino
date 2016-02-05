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
const char* SSID = "*********";
const char* PW   = "********";          // set to "" for open access point w/o passwortd

// configure IO
const int GPIO0 = 0;                    // GPIO0 used for the relay
const int GPIO2 = 2;                    // GPIO2 not used, just to configure as input with Pullup in the setup routine
//const int GPIO12 = 12;                  // For ESP8266-12E usage

int val = HIGH;                         // "HIGH/1" means switched 'off' on setup
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
    //digitalWrite(GPIO0, val);
    Serial.print(F("LED is: "));
    Serial.print(val);
    val =! val;                         // toggle on
    digitalWrite(GPIO0, val);
    delay(500);                         // wait for 1/2 a secound to give the door control a chance to react
    Serial.println(val);
    val =! val;                         // toggle off
    digitalWrite(GPIO0, val);    
}

void setup()
{
    pinMode(GPIO0, OUTPUT);             // GPIO0 configured as output
    pinMode(GPIO2, INPUT_PULLUP);       // GPIO2 configured as input with Pullup
    digitalWrite(GPIO0, val);           // initial condition 1 (Relay off)
    
    Serial.begin(115200);               // init serial inteface

    Serial.println(F(""));              // You can pass flash-memory based strings to
    Serial.println(F("ESP8266-01...")); // Serial.print() by wrapping them with F().
    Serial.print(F("SDK-Version: "));
    Serial.println(ESP.getSdkVersion());
    Serial.print(F("ESP8266 Chip-ID: "));
    Serial.println(ESP.getChipId());
    Serial.print(F("ESP8266 Speed in MHz: "));
    Serial.println(ESP.getCpuFreqMHz());
    Serial.print(F("Free Heap Size in Bytes: "));
    Serial.println(ESP.getFreeHeap());
    
    Serial.print(F("Start WLAN-Hotspot: "));
    Serial.println(SSID);
    
    WiFi.mode(WIFI_AP);                 // set access point modus
    WiFi.softAP(SSID, PW);              // start as access point
    delay(500);                         // wait 0,5s
    
    Serial.print(F("IP Address: "));    // print of the current IP of the server
    Serial.println(WiFi.softAPIP());
    
    // treatment of the different events - before server.begin()!!!
    server.on("/", Event_Index);
    server.on("/toggle.htm", Event_Toggle);
    
    server.begin();                     // start the server
    Serial.println(F("HTTP Server gestartet.."));
}
 
void loop()
{
    server.handleClient();              // check for incomming client connections frequently in the main loop
    delay(1);
}
