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
 
String html1 = "<!DOCTYPE html>\r\n<html>\r\n\
<head>\r\n<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\r\n\
<title>Garage control</title>\r\n\
<form action=\"";
// String((val)?"/1.html":"/0.html")
String html2 = "\">\r\n<input value=\"Open/Close\" style=\"";
String html3 = " width:15em;height:13em; font-size: 24px;\" type=\"submit\">\
</form>\r\n</head>\r\n<body>\r\n</body>\r\n</html>";
 
ESP8266WebServer server(80);        // Setup server port
int val = 1;                        // switched off on setup
String Temp = "";
 
void Ereignis_SchalteON()           // Executed if "http://<ip address>/1.html" is called
{
    val = 0;                        // Relay off
    digitalWrite(0, val);           // GPIO0
    Temp = html1 + String((val) ? "/1.html" : "/0.html");
    Temp += html2 + String((val) ? "BACKGROUND-COLOR: DarkGray;" : "BACKGROUND-COLOR: Chartreuse;") + html3;
    server.send(200, "text/html", Temp);
}
 
void Ereignis_SchalteOFF()          // Executed if "http://<ip address>//0.html" is called
{
    val = 1;                        // Relay on
    digitalWrite(0, val);           // GPIO0
    Temp = html1 + String((val) ? "/1.html" : "/0.html");
    Temp += html2 + String((val) ? "BACKGROUND-COLOR: DarkGray;" : "BACKGROUND-COLOR: Chartreuse;") + html3;
    server.send(200, "text/html", Temp);
}
 
void Ereignis_Index()               // Executed if "http://<ip address>/" is called
{
    Temp = html1 + String((val) ? "/1.html" : "/0.html");
    Temp += html2 + String((val) ? "BACKGROUND-COLOR: DarkGray;" : "BACKGROUND-COLOR: Chartreuse;") + html3;
    server.send(200, "text/html", Temp);
}
 
void setup()
{
    //digitalWrite(0, 1);                   // Anfangszustand 1 (Relais ausgeschaltet)
    pinMode(0, OUTPUT);                   // GPIO0 configured as output
    pinMode(2, INPUT_PULLUP);             // GPIO2 configured as input with Pullup
    digitalWrite(0, 1);                   // initial condition 1 (Relay off)
    
    Serial.begin(115200);                 // init seriel inteface
    Serial.println("");                   // print empty line
    Serial.println("Starte WLAN-Hotspot \"astral\"");
    WiFi.mode(WIFI_AP);                   // access point modus
    WiFi.softAP("astral", "12345678");    // name of the Wifi net
    delay(500);                           // wait 0,5s
    Serial.print("IP Adresse ");          // print of the current IP of the server
    Serial.println(WiFi.softAPIP());
    
    // treatment of the different events - before server.begin()!!!
    server.on("/",       Ereignis_Index);
    server.on("/1.html", Ereignis_SchalteON);
    server.on("/0.html", Ereignis_SchalteOFF);
    
    server.begin();                       // start the server
    Serial.println("HTTP Server gestartet");
}
 
void loop()
{
    server.handleClient();
    delay(500);
  
//  if (!digitalRead(2))          //Wenn Taster an GPIO2 betetigt wurde
//  {
//    val = !val;                 // Schaltzuschtand andern
//    digitalWrite(0, val);
//    while (!digitalRead(2))server.handleClient(); // Warten bis der Taster losgelassen wird.
//  }
}

