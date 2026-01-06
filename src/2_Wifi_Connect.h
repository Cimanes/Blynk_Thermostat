#pragma once


// Private credentials and connection details (uncomment the one needed)  --> Moved to Globals
/*
#if defined(Cespedal)
  const char ssid[] = "Pepe_Cimanes";
  const char pass[] = "Cimanes7581" ;
#elif defined(Toledo)
  const char ssid[] = "MIWIFI_HtR7" ;
  const char pass[] = "TdQTDf3H"    ;
#elif defined(Adagio)
  const char ssid[] = "Acapella-Suite-Adagio" ;
  const char pass[] = "38352609352426231808"  ;
#elif defined(Barcarolle)
  const char ssid[] = "Acapella-Suite-Barcarolle" ;
  const char pass[] = "85658792719454704286"      ;
#endif
*/

//--------------------------------------------
//  AP configuration
//--------------------------------------------
void ConnectWiFi_AP(bool useStaticIP = false) ;

//--------------------------------------------
//  STA configuration
//--------------------------------------------
void ConnectWiFi_STA(bool useStaticIP = false) ;

//--------------------------------------------
//  STA re-connect
//--------------------------------------------
// Used when connection is lost
void reconnect_STA(bool useStaticIP = false) ;