/* ----------------------------------------------------------------------------------------
 * Access PM2.5 Particle Sensor with grove cable (I2C)
 * Board: Grove - Laser PM2.5 Sensor (HM3301) / with HiFive1
 * Device: Seeed Studio	PN: 101020613
 * ------------------------------------------------------------------------------------- */

#include "Seeed_HM330X.h"

int             DELAY_AFTER_HM331_READ = 5000;  //TODO REMOVE

HM330X          hm331Pm25Sensor;
u8              hm331Buf[30];


const char *pm25Str[]={"sensor num: ","PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
                    };

/* ----------------------------------------------------------------------------------------
 *  print_hm331_result: function to print results read by the Laser PM2.5 Sensor (HM3301)
 *  Input: str - the string to print, value - the value to print next to string
 *  returns: HM330XErrorCode  (NO_ERROR / ERROR_PARAM / ERROR_COMM / ERROR_OTHERS)
 * ------------------------------------------------------------------------------------- */
HM330XErrorCode print_hm331_result(const char* str,u16 value)
{
    if(NULL==str)
        return ERROR_PARAM;
    Serial.print(str);
    Serial.println(value);
    return NO_ERROR;
} // end of print_hm331_result


/* ----------------------------------------------------------------------------------------
 *  print_hm331_result: function to parse buffer with 29 char in u8-data format from sensor
 *  Input: data - the input buffer
 *  returns: HM330XErrorCode  (NO_ERROR / ERROR_PARAM / ERROR_COMM / ERROR_OTHERS)
 * ------------------------------------------------------------------------------------- */
HM330XErrorCode parse_hm331_result(u8 *data)
{
    u16 value=0;
    //err_t NO_ERROR;    //TODO remove
    if(NULL==data)
        return ERROR_PARAM;
    for(int i=1;i<8;i++)
    {
         value = (u16)data[i*2]<<8|data[i*2+1];
         print_hm331_result(pm25Str[i-1],value);
    }
    return NO_ERROR;
} // end of parse_hm331_result

/* ----------------------------------------------------------------------------------------
 *  parse_hm331_result_value: function to parse out the value data from 29 char buffer in
 *                            u8-data format from sensor
 *  Input: data - the input buffer
 *  returns: HM330XErrorCode  (NO_ERROR / ERROR_PARAM / ERROR_COMM / ERROR_OTHERS)
 * ------------------------------------------------------------------------------------- */
HM330XErrorCode parse_hm331_result_value(u8 *data)
{
    if(NULL==data)
        return ERROR_PARAM;
    for(int i=0;i<28;i++)
    {
        Serial.print(data[i],HEX);
        Serial.print("  ");
        if((0==(i)%5)||(0==i))
        {
            Serial.println(" ");
        }
    }
    u8 sum=0;
    for(int i=0;i<28;i++)
    {
        sum+=data[i];
    }
    if(sum!=data[28])
    {
        Serial.println();
        Serial.println("wrong checksum !");
    }
    Serial.println(" ");
    Serial.println(" ");
    return NO_ERROR;
} // end of parse_hm331_result_value

/* ----------------------------------------------------------------------------------------
 *  Sample_hm331_PM25_Sensor: print to serial consule the data as parsed from sensor read buffer
 *  Input: none
 *  returns: void
 * ------------------------------------------------------------------------------------- */
void Sample_hm331_PM25_Sensor()
{
  if(hm331Pm25Sensor.read_sensor_value(hm331Buf,29))
    {
      Serial.println("HM330X read result failed !");
    }
    parse_hm331_result_value(hm331Buf);
    parse_hm331_result(hm331Buf);
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
} // end of Sample_hm331_PM25_Sensor


/* ----------------------------------------------------------------------------------------
 *  Init_hm331_PM25_Sensor: initalize the Laser PM2.5 Sensor (HM3301) - runs once on setup
 *  Input: none
 *  returns: void
 * ------------------------------------------------------------------------------------- */
void Init_hm331_PM25_Sensor() {

  Serial.println("HM331 PM2.5 Sensor start");
  if(hm331Pm25Sensor.init())
  {
    Serial.println("HM330X init failed !");
  }
} // end of Init_hm331_PM25_Sensor
