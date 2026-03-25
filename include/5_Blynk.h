#pragma once


//--------------------------------------------
//  BLYNK configuration
//--------------------------------------------
#define BLYNK_FIRMWARE_VERSION "1.0.2"
#define BLYNK_PRINT Serial
// #define BLYNK_DEBUG

#define BLYNK_TEMPLATE_ID "TMPLc-2xzsN8"
#define BLYNK_TEMPLATE_NAME "Thermostat Wemos mini"
#define BLYNK_AUTH_TOKEN "0_F5qU0JwwjDi45OwxpJ3fVtkIdfiOdB"

//--------------------------------------------
//  Functions
//--------------------------------------------
void setup_Blynk();
void loop_Blynk();
bool get_boiler();
bool get_sum_win();