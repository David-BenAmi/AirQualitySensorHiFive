/* ----------------------------------------------------------------------------------------
 * Main execution program for the Air Quality Monitor based on a SiFive HiFive RISC-V controller board.
 * Board: HiFive1 low-cost, Arduino-compatible development board featuring the Freedom E310 (RISC-V). 
 * Device: SparkFun GPS-14414
 * ------------------------------------------------------------------------------------- */

#include "SSWire_AirQualityMonitor.h"
#include "RISCVAirQualityMonitor.h"
#include "PM2_5_Laser_Sensor_Grove_HM3301.h"
#include "LCD_GROVE_16x2.h"
#include "GPS_Qwiic_XA1110.h"

enum I2CAddressEnum{
  I2C_PM2_5_SENSOR = 0x40,
  I2C_CCS811_SENSOR = 0x5b,
  I2C_BME280_SENSOR = 0x77,
  I2C_GPS_SENSOR = 0x10,
  I2C_LCD = 0x3e,
  I2C_EEPROM = 0x50,
  I2C_ESP32_WIFI = 0x0 //TBD
};

char I2CDeviceNameStr[7][20] = { "I2C_PM2_5_SENSOR", "I2C_CCS811_SENSOR",
  "I2C_BME280_SENSOR", "I2C_GPS_SENSOR", "I2C_LCD", "I2C_EEPROM", "I2C_ESP32_WIFI"};

void setup() {

  Serial.begin(115200);
  delay(100);
  lcd_init();
  gps_init();
  Init_hm331_PM25_Sensor();
  
} // end of setup

void loop() {
  // put your main code here, to run repeatedly:

  gps_read_location();
  Sample_hm331_PM25_Sensor();
  delay(5000);
} // end of loop
