#pragma once

//  STA configuration
void ConnectWiFi_STA(bool useStaticIP = false) ;

//  STA re-connect - Used when connection is lost
void reconnect_STA(bool useStaticIP = false) ;