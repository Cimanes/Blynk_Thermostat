#include "2_Wifi_Connect.h"

/*
 * ***** Configure Wifi connections
 * This tab contains wifi configuration (users and passwords)
 * It also configures the connection type (STA or AP)
 */

//--------------------------------------------
//  Libraries
//--------------------------------------------  
#include "0_Global_Config.h"    // Options and globals
#ifdef NTP_clock
  #ifdef ARDUINO_ARCH_ESP32
    #include <WiFi.h>
  #endif
  #ifdef ARDUINO_ARCH_ESP8266
    #include <ESP8266WiFi.h> 
  #endif

  //--------------------------------------------
  //  Constants
  //--------------------------------------------
  static const char hostname[] = "ESP8266_1" ;

  // Used only for static IP address connections:
  static const IPAddress ip(192, 168, 1, 200)    ;
  static const IPAddress gateway(192, 168, 1, 1) ;
  static const IPAddress subnet(255, 255, 255, 0);


   //--------------------------------------------
  //  STA configuration
  //--------------------------------------------
  void ConnectWiFi_STA(bool useStaticIP) {
    Serial.println("")    ;
    WiFi.mode(WIFI_STA)   ;
    WiFi.begin(ssid, pass);
    if(useStaticIP) WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print('.')   ;
    }

    Serial.println("")            ;
    Serial.print("Booting STA:\t");
    Serial.println(ssid)          ;
    Serial.print("IP address:\t") ;
    Serial.println(WiFi.localIP());
  }

  //--------------------------------------------
  //  STA re-connect
  //--------------------------------------------
  // Used when connection is lost
  void reconnect_STA(bool useStaticIP) {
    Serial.print("Re-connecting to SSID: ")         ;
    Serial.println(ssid)                            ;
    WiFi.begin(ssid, pass)                          ;
    if(useStaticIP) WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass)                        ;  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".")                             ;
      delay(1000);
    }
    Serial.println("\nConnected.");
  }

  //--------------------------------------------
  //  AP configuration
  //--------------------------------------------
  void ConnectWiFi_AP(bool useStaticIP) {
    Serial.println("")    ;
    WiFi.mode(WIFI_AP)    ;
    while(!WiFi.softAP(ssid, pass)) {
      Serial.println(".") ;
      delay(500)          ;
    }
    if(useStaticIP) WiFi.softAPConfig(ip, gateway, subnet);
    Serial.println("")             ;
    Serial.print("Booting AP:\t")   ;
    Serial.println(ssid)            ;
    Serial.print("IP address:\t")   ;
    Serial.println(WiFi.softAPIP()) ;
  }

#endif // NTP_clock