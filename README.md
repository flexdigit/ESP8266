# ESP8266

Server einrichtgen
--------------------------------------------------------------------------
WLAN-Connect

1.) AT+CWMODE=2             1 - Station mode, 2 - AP-Mode, 3 - beides

Server-Start
2.) AT+CIPMUX=1             Enable Multiple connections
3.) AT+CIPSERVER=1,9768     Server auf Port 9768, default IP: 192.168.4.1

4.) AT+CIFSR                Check module IP address
5.) AT+CIPSTATUS            Get connection status
6.) AT+CWSAP="ESP4711","12345678",7,3
                            ESP als Access Point mit SSID "ESP4711" und dem
                            Passwort "12345678". Das Passwort muss eine
                            Mindestlänge von 8 Zeichen haben.
                            Under AP mode!!!
                            7: Channel
                            3: WPA2_PSK

7.) AT+CWSAP?               Abfrage der Einstellungen

*********************************************************************
Nach AT+RST muss ich 2.) bis 3.) absetzen, dass ich mich 
wieder mit dem Smartphone verbinden kann.
*********************************************************************


Allgemein werdenempfangene Daten durch +IPD gekennzeichnet. Danach folgen zwei Zahlen.
Die erste Zahl zeigt, welche der bestehenden Verbindungen die Daten geschickt hat. Da
z.Z. nur ein Rechner die Seite geöffnet hat, steht dort eine 0. Eine zweite Anfrage
würde dann den Index 1 bekommen usw. Die zweite Zahl gibt die Länge der empfangenen
Daten an. Hier wurden also anscheinend 376 Zeichen empfangen. Wer möchte kann ja mal
nachzählen. Nach dem Doppelpunkt stehen nun die eben beschriebenen Daten. Es scheint
sich hier um eine Anforderung zu handeln, die allgemein beim Öffnen einer Seite vom
Browser geschickt wird.

Diese Verbindung bleibt nun eine Weile stehen, ohne dass etwas passiert. Nach in der
Regel 180s = 3 Minuten, wird die Verdingung automatisch getrennt. Dies wird durch die
Meldung Unlinked verdeutlicht. Sie können die Zeit bis zur automatischen Trennung
übrigens auch Einstellen und zwar mit dem Befehl AT+CIPSTO=X wobei X maximal 28800
für 28800s = 8h entsprechen darf.

Falls Sie sich gerade nicht sicher sind, ob und wenn ja wie viele Verbindungen zum
Modul bestehen, können Sie dies mit dem Befehl

AT+CIPSTATUS

Status:
    2 - Got IP
    3 - Connected
    4 - Disconnected


