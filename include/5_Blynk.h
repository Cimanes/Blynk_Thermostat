#pragma once
/*
 * The device can send data to the App using Blynk.virtualWrite(pin, value)
 * and receive data from the App using BLYNK_WRITE(Vpin) { ... }
 */

//--------------------------------------------
//  BLYNK configuration
//--------------------------------------------
#define BLYNK_TEMPLATE_ID "TMPLc-2xzsN8"
#define BLYNK_TEMPLATE_NAME "Thermostat Wemos mini"
#define BLYNK_AUTH_TOKEN "0_F5qU0JwwjDi45OwxpJ3fVtkIdfiOdB"

#define BLYNK_USE_OTA
#define BLYNK_FIRMWARE_VERSION "1.0"

// #define BLYNK_DEBUG
// #define BLYNK_PRINT Serial

//--------------------------------------------
//  Functions
//--------------------------------------------
void setup_Blynk();
void loop_Blynk();
bool get_boiler();
bool get_sum_win();