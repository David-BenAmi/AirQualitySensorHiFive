/* ----------------------------------------------------------------------------------------
 * EEPROM I2C Write/Read Test Program
 * Board: Arduino Uno / HiFive1
 * Device: SparkFun SPX-14764 Qwiic EEPROM 512Kbit 64KB persistent memory (I2C)
 * ------------------------------------------------------------------------------------- */
#include "E2PROM_Qwiic_64K.h"

#define I2C_EEPROM_ADDRESS    0x50   // EEPROM I2C default address //TODO remove
#define I2C_CLOCK_RATE        400000
#define MAX_EEPROM_PAGE_WRITE 0X10   // Limit the page write to 16 bytes (Max is 64 bytes)
#define MAX_EEPROM_ADDRESS (64*1024-1) // 512Kbits is 64KBytes


char testString[] = "Hello World!";
byte tempStore[MAX_EEPROM_PAGE_WRITE];



/* ----------------------------------------------------------------------------------------
 *  string2ByteArray: function to convert a string into byte array
 *  returns the number of bytes converetd
 * ------------------------------------------------------------------------------------- */
int string2ByteArray(char* input, byte* output)
{
    int inputIdx = 0 ,outputIdx = 0;
    // Scan the input string to its end but limit it by allowed page size
    while((input[inputIdx] != '\0') && (inputIdx < MAX_EEPROM_PAGE_WRITE))
    {
        output[outputIdx++] = (byte)input[inputIdx++];
    }
    return inputIdx;
} // end of string2ByteArray


/* ----------------------------------------------------------------------------------------
 * writeEEPROMPage: Three step memory page writing
 * 1. Send the MSB of the address.
 * 2. Send the LSB of the address.
 * 3. Send the data to be stored.
 * returns the number of bytes written to EEPROM or zero on error
 * ------------------------------------------------------------------------------------- */
int writeEEPROMPage(long eepromAddress, byte* pageBuffer, int bytes)
{
  int dataByteIdx = 0;
  /*if ((eepromAddress + bytes) < MAX_EEPROM_ADDRESS) //&& (bytes<=MAX_EEPROM_PAGE_WRITE)) // validate input
  {*/
    SSWire.beginTransmission(I2C_EEPROM_ADDRESS);
    // Write the address (MSB and LSB)
    SSWire.write((int)(eepromAddress >> 8));    // MSB (shift left 8 bits)
    SSWire.write((int)(eepromAddress & 0xFF));  // LSB (keep lower 8 bits)

    // Write bytes to EEPROM
    for (dataByteIdx = 0 ; dataByteIdx < bytes ; dataByteIdx++)
      SSWire.write(pageBuffer[dataByteIdx]); //Write sequencial data

    SSWire.endTransmission(); //Send stop condition
    return dataByteIdx;
  /*}
  else
  {
    return 0;
  }*/
} // end of writeEEPROMPage

/* ----------------------------------------------------------------------------------------
 * readEEPROM: read X bytes from a given EEPROM address into a given byte buffer
 * returns the number of bytes read or zero on error
 * ------------------------------------------------------------------------------------- */
int readEEPROM(long eepromAddress, byte* readBuffer, int bytes)
{
  int readBytes = 0;
 /* if (((eepromAddress + bytes) < MAX_EEPROM_ADDRESS) && (bytes<MAX_EEPROM_PAGE_WRITE)) // validate input
  {*/
    SSWire.beginTransmission(I2C_EEPROM_ADDRESS);
    SSWire.write((int)(eepromAddress >> 8));    // MSB (shift left 8 bits)
    SSWire.write((int)(eepromAddress & 0xFF));  // LSB (keep lower 8 bits)

    SSWire.endTransmission();

    // read from I2C bus a known number of bytes and release the bus
    SSWire.requestFrom(I2C_EEPROM_ADDRESS, bytes, true);
    // start reading byte after byte from the I2C slave
    while (SSWire.available())
    {
      readBuffer[readBytes] = SSWire.read();
      readBytes++;
    }
    return readBytes-1;
  /*}
  else
  {
    return 0;
  }*/
} // end of readEEPROM


/* ----------------------------------------------------------------------------------------
 * e2prom_test: verify R/W to/from the E2PROM
 *  printout to consule the progress and result.
 * ------------------------------------------------------------------------------------- */
void e2prom_test()
{
  byte dataRead[MAX_EEPROM_PAGE_WRITE];
  char message[64] = "";
  int numOfBytes = 0, bytesWritten = 0, bytesRead = 0;
  long startAddress = 0x0;    // start writing data from this EEPROM address

//Start the I2C Library
  SSWire.begin();
  SSWire.setClock(I2C_CLOCK_RATE);

//Start the serial port
  Serial.begin(115200);



// convert a string into a limited byte array
  numOfBytes = string2ByteArray(testString , tempStore);

  sprintf(message, "DEBUG: 'testString' is: '%s'", testString);
  Serial.println(message);

  sprintf(message, "DEBUG: string2ByteArray returned %d bytes", numOfBytes);
  Serial.println(message);

// (1) Write once the pre-defined test string (as bytes)
  bytesWritten = writeEEPROMPage(startAddress, tempStore, numOfBytes);

  sprintf(message, "Written to EEPROM %d bytes: '%s'", bytesWritten, testString);
  Serial.println(message);

// (2) Read once the EEPROM for the test string (as bytes)
  bytesRead = readEEPROM(startAddress, dataRead, numOfBytes);

  dataRead[bytesRead+1] = '\0';  // add NULL as end of string indicator
  sprintf(message, "Read from EEPROM %d bytes: '%s'", bytesRead, dataRead);
  Serial.println(message);

// (3) memory data compare
  if ((memcmp ( dataRead, tempStore, (size_t)bytesWritten)!=0) || (bytesWritten<=0))
  {
    Serial.println("Fail: The data read is not the same as the data written!");
  }
  else
  {
    Serial.println("Pass: The data read is the same as the data written.");
  }
} // end of e2prom_test
