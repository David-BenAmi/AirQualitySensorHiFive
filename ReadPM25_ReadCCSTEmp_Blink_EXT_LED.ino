/*
  Blink External Led (PIN2) + Read from 2 sensors (Temperateure/Humidity and PM2.5)

  Read Measurements from 2 Sensors (Seeed HM331 PM2.5 and Adafruit's CCS811)
  and output to SERIAL port
  Turns an LED on for preconfigured time, then off for one preconfigured, repeatedly.
*/
#include "Seeed_HM330X.h"
#include "Adafruit_CCS811.h"

#ifdef  ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif

int             EXTERNAL_LED_PIN = 2;
int             DELAY_AFTER_HM331_READ = 5000;
int             DELAY_AFTER_CCS811_READ = 500;
int             DELAY_BLINK_ON  = 3000;
int             DELAY_BLINK_OFF = 1000;

HM330X          hm331Pm25Sensor;
u8              hm331Buf[30];
Adafruit_CCS811 ccsTempSensor;

const char *pm25Str[]={"sensor num: ","PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
                    };

err_t print_hm331_result(const char* str,u16 value)
{
    if(NULL==str)
        return ERROR_PARAM;
    SERIAL.print(str);
    SERIAL.println(value);
    return NO_ERROR;
}

/*parse buf with 29 u8-data*/
err_t parse_hm331_result(u8 *data)
{
    u16 value=0;
    err_t NO_ERROR;
    if(NULL==data)
        return ERROR_PARAM;
    for(int i=1;i<8;i++)
    {
         value = (u16)data[i*2]<<8|data[i*2+1];
         print_hm331_result(pm25Str[i-1],value);
    }
}

err_t parse_hm331_result_value(u8 *data)
{
    if(NULL==data)
        return ERROR_PARAM;
    for(int i=0;i<28;i++)
    {
        SERIAL.print(data[i],HEX);
        SERIAL.print("  ");
        if((0==(i)%5)||(0==i))
        {
            SERIAL.println(" ");
        }
    }
    u8 sum=0;
    for(int i=0;i<28;i++)
    {
        sum+=data[i];
    }
    if(sum!=data[28])
    {
        SERIAL.println("wrong checkSum!!!!");
    }
    SERIAL.println(" ");
    SERIAL.println(" ");
    return NO_ERROR;
}

void Sample_hm331_PM25_Sensor()
{
  if(hm331Pm25Sensor.read_sensor_value(hm331Buf,29))
    {
      SERIAL.println("HM330X read result failed!!!");
    }
    parse_hm331_result_value(hm331Buf);
    parse_hm331_result(hm331Buf);
    SERIAL.println(" ");
    SERIAL.println(" ");
    SERIAL.println(" ");
}

void Sample_Ccs_Temperature_Sensor()
{
  if(ccsTempSensor.available())
    {
      float temp = ccsTempSensor.calculateTemperature();
      if(!ccsTempSensor.readData())
      {
        SERIAL.print("CO2: ");
        SERIAL.print(ccsTempSensor.geteCO2());
        SERIAL.print("ppm, TVOC: ");
        SERIAL.print(ccsTempSensor.getTVOC());
        SERIAL.print("ppb Temp:");
        SERIAL.println(temp);
      }
      else
      {
        SERIAL.println("ccsTempSensor ERROR!");
        while(1);
      }
    }
}

void Blink_External_LED(int offDelay, int onDelay)
{
  digitalWrite(EXTERNAL_LED_PIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(offDelay);                          // wait for a second
  digitalWrite(EXTERNAL_LED_PIN, LOW);  // turn the LED off by making the voltage LOW
  delay(onDelay);                          // wait for a second
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(EXTERNAL_LED_PIN, OUTPUT);
  SERIAL.begin(115200);
  delay(100);
  SERIAL.println("Serial start");
  if(hm331Pm25Sensor.init())
  {
    SERIAL.println("HM330X init failed!!!");
    while(1);
  }
  
  SERIAL.begin(9600);
  SERIAL.println("ccsTempSensor test");
  if(!ccsTempSensor.begin())
  {
    SERIAL.println("Failed to start ccsTempSensor! Please check your wiring.");
    while(1);
  }
  //calibrate temperature sensor
  while(!ccsTempSensor.available());
  float temp = ccsTempSensor.calculateTemperature();
  ccsTempSensor.setTempOffset(temp - 25.0);
}

// the loop function runs over and over again forever
void loop() {
  
  // Read data from hm331Pm25 sensor
  Sample_hm331_PM25_Sensor();
  delay(DELAY_AFTER_HM331_READ);
  // Read data from ccsTemp sensor
  Sample_Ccs_Temperature_Sensor();
  delay(DELAY_AFTER_CCS811_READ);
  // Blink External led on PIN 2
  Blink_External_LED(DELAY_BLINK_ON, DELAY_BLINK_OFF);
}
