#pragma once  
/*
 * Operate a digital output to energize a Relay based on several inputs:
 *  - Automatic operation by comparison of current temperature (T) with setpoint temperature (T_sp).
 *  -  Setpoint (T_sp) is constrained between T_min and T_max.
 *  - If T_sp is set as T_min, the heater will remain OFF, no matter what the actual temperature is
 *  - If T_sp is set as T_max, the heater will remain ON, no matter what the actual temperature is
*/

//--------------------------------------------
//  Libraries
//--------------------------------------------
#include "0_Global_Config.h"    // Options and globals

//--------------------------------------------
//  Pin definitions
//--------------------------------------------
#define pin_relay 12    // Wemos GPIO12 = D6 digital output pin to energize the relay.
#ifdef local_enable
  #define pin_lower  0  // Wemos GPIO2  = D4 pin to lower T_sp in Auto, or heater off in Man (embedded pull up, active low).
  #define pin_raise  2  // Wemos GPIO0  = D3 pin to raise T_sp in Auto, or heater on in Man (embedded pull up, active low).
  #define pin_auto  15  // Wemos GPIO15 = D8 pin to change mode Manual/Auto. (embedded pull down, active high).
#endif

//--------------------------------------------
//  Control Variables
//--------------------------------------------
#ifdef local_enable
  extern bool local_adj;
  extern bool adjusting;
#endif

//--------------------------------------------
// Public API
//--------------------------------------------
void setup_control();
void thermostat();