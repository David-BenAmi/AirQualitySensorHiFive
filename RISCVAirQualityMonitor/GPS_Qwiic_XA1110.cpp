/* ----------------------------------------------------------------------------------------
 * GPS readout
 * Board: SparkFun GPS Breakout - XA1110 (Qwiic) / with HiFive1
 * Device: SparkFun GPS-14414
 * ------------------------------------------------------------------------------------- */

#include "GPS_Qwiic_XA1110.h"

#define TMP_GPS_I2C_ADDRESS 0x10   //TODO to be removed once PUBLIC manager ready

I2CGPS myI2CGPS; //Hook object to the library
TinyGPSPlus gps; //Declare gps object


/* ----------------------------------------------------------------------------------------
 *  gps_init: function to initalize the GPS module
 *  returns: void
 * ------------------------------------------------------------------------------------- */
void gps_init(){
  if (myI2CGPS.begin(SSWire, TMP_GPS_I2C_ADDRESS)) {  //TODO get this from external source
    // "GPS sensor responded"
    Serial.println("GPS sensor responded");
  } else {
    // "GPS sensor does not respond"
    Serial.println("GPS sensor does not respond");
  }
} // end of gps_init


/* ----------------------------------------------------------------------------------------
 *  gps_read_location: function to read the location data from module
 *  returns void
 *  //TODO  should return the GPS coordinates as values
 * ------------------------------------------------------------------------------------- */
void gps_read_location() {
    //available() returns the number of new bytes available from the GPS module
    while (myI2CGPS.available())
    {
      gps.encode(myI2CGPS.read()); //Feed the GPS parser
    }
    //Check to see if new GPS info is available
    if (gps.time.isUpdated())
    {
      gps_display_info();
    }
} // end of gps_read_location


/* ----------------------------------------------------------------------------------------
 *  gps_display_info: function to print location and date/time to serial consule
 *  returns void
 *  //TODO  should return the GPS coordinates as values
 * ------------------------------------------------------------------------------------- */
void gps_display_info()
{
  //We have new GPS data to deal with!
  Serial.println();

  if (gps.time.isValid())
  {
    Serial.print(F("Date: "));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());

    Serial.print((" Time: "));
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print((gps.time.hour()+ ISRAEL_OFFSET) % 24);
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());

    Serial.println(); //Done printing time
  }
  else
  {
    Serial.println(F("Time not valid"));
  }

  if (gps.location.isValid())
  {
    Serial.print("Location: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(", "));
    Serial.print(gps.location.lng(), 6);
    Serial.println();
  }
  else
  {
    Serial.println(F("Location not valid"));
  }
} // end of gps_display_info
