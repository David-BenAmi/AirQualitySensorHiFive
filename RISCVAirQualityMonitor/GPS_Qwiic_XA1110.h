#ifndef __GPS_QWICC_XA1110__
#define __GPS_QWICC_XA1110__

#define ISRAEL_OFFSET 2

#include "SSWire_AirQualityMonitor.h"
#include "SparkFun_I2C_GPS_Sifive_Library.h"
#include "TinyGPS++.h"


void gps_init();
void gps_read_location();
void gps_display_info();

#endif
