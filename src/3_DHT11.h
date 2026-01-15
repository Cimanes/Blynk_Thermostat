#pragma once

//--------------------------------------------
//  DHT11 Sensor configuration
//--------------------------------------------
// Define DHT sensor type: "auto-detect" caused errors.
// 11 = DHT11 DHT12, KY015, 
// 22 = DHT22, DHT33, DHT44 a.o
// 23 = DHT23
// 70 = Sonoff Si7021 (Experimental, may not work properly)
#define DHT_TYPE 11  

//--------------------------------------------
//  Function prototypes
//--------------------------------------------
void setup_sensor() ;