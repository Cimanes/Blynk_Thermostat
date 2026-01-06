#pragma once

//--------------------------------------------
//  NTP Configuration
//--------------------------------------------
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"    // Europe/Madrid	CET-1CEST,M3.5.0,M10.5.0/3
#define MY_NTP_SERVER "at.pool.ntp.org"

//--------------------------------------------
//Variables
//--------------------------------------------
void get_Date();  //  Receive data from NTP and process
void setup_NTP();