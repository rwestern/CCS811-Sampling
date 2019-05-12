/******************************************************************************
  Logging humidity-compensated CCS811 samples to Google Sheets

  Based heavily on code from Marshall Taylor @ SparkFun Electronics:
  https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library

  Hardware Connections (Breakoutboard to ESP8266):
  3.3V to 3.3V pin
  GND to GND pin
  SDA to D2
  SCL to D1


  Connects to Wifi network, then sends samples via GET request
  To Pushingbox, which forwards it to Google Sheets.
  Requires setup of a Pushingbox Service and Scenario, and Google Sheet and published app:
  https://www.hackster.io/detox/transmit-esp8266-data-to-google-sheets-8fc617
  
  Writes data to Pushingbox as:
    CO2_ppm
    TVOC_ppb
    Temp_K
    Press_Pa
    RH
  Define a PB GET Service as:
    ?CO2=$CO2_ppm$&TVOC=$TVOC_ppb$&Temp=$Temp_K$&Press=$Press_Pa$&RH=$RH$
  
******************************************************************************/

#include <Wire.h>
#include <SparkFunBME280.h> //Click here to get the library: http://librarymanager/All#SparkFun_BME280
#include <SparkFunCCS811.h> //Click here to get the library: http://librarymanager/All#SparkFun_CCS811
#include <ESP8266WiFi.h>
#include "wifi_pw.h" // stores wifi password and SSID
#include "CCS811_debug.h"
#include "pushingbox_devid.h" // stores Pushingbox DevID

//const char* const devid = ""; // Pushingbox DevID
//const char* const ssid = ""; //Wifi SSID
//const char* const pw = ""; //Wifi PW
const char* const site = "api.pushingbox.com";

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address


//Global sensor objects
CCS811 myCCS811(CCS811_ADDR);
BME280 myBME280;

bool attemptWifiLogin();
void writeToGoogleDocs(CCS811 &myCCS811, BME280 &myBME280);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Apply BME280 data to CCS811 for compensation.");

  Wire.begin();

  //This begins the CCS811 sensor and prints error status of .begin()
  CCS811Core::status returnCode = myCCS811.begin();
  Serial.print("CCS811 begin exited with: ");
  //Pass the error code to a function to print the results
  printDriverError(returnCode);
  Serial.println();

  //For I2C, enable the following and disable the SPI section
  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x77;

  //Initialize BME280
  //For I2C, enable the following and disable the SPI section
  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x77;
  myBME280.settings.runMode = 3; //Normal mode
  myBME280.settings.tStandby = 0;
  myBME280.settings.filter = 4;
  myBME280.settings.tempOverSample = 5;
  myBME280.settings.pressOverSample = 5;
  myBME280.settings.humidOverSample = 5;

  //Calling .begin() causes the settings to be loaded
  delay(100); //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  myBME280.begin();
}
//---------------------------------------------------------------
void loop()
{
  if (attemptWifiLogin())
  {

    //Check to see if data is available
    if (myCCS811.dataAvailable())
    {
      //Calling this function updates the global tVOC and eCO2 variables
      myCCS811.readAlgorithmResults();

      float BMEtempC = myBME280.readTempC();
      float BMEhumid = myBME280.readFloatHumidity();

      writeToGoogleDocs(myCCS811, myBME280);
      //This sends the temperature data to the CCS811
      myCCS811.setEnvironmentalData(BMEhumid, BMEtempC);
    }
    else if (myCCS811.checkForStatusError())
    {
      //If the CCS811 found an internal error, print it.
      printSensorError(myCCS811);
    }
    delay(10 * 60 * 1000); //Wait for next reading
  }
  else
  {
    delay(2000);
  }
}


bool attemptWifiLogin()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return true;
  }

  Serial.println("Attempting Wifi login");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pw);

  delay(10000);
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected");
    return true;
  }

  Serial.println("Failed");
  return false;
}

void writeToGoogleDocs(CCS811 &myCCS811, BME280 &myBME280)
{
  WiFiClient client;

  if (client.connect(site, 80))
  {
    const String url =
        "/pushingbox?devid=" + String(devid) +
        "&CO2_ppm=" + (String)myCCS811.getCO2() +
        "&TVOC_ppb=" + (String)myCCS811.getTVOC() +
        "&Temp_K=" + String(myBME280.readTempC() + 273.15) +
        "&Press_Pa=" + (String)myBME280.readFloatPressure() +
        "&RH=" + (String)myBME280.readFloatHumidity();

    Serial.println(String("http://") + site + url);

    client.print("GET " + url);

    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(site);
    client.println("User-Agent: ESP8266/1.0");
    client.println("Connection: close");
    client.println();

    while (client.connected() || client.available())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        //Serial.println(line);
      }
    }
  }
}
