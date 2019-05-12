Logging humidity-compensated CCS811 samples to Google Sheets

  Based heavily on code from Marshall Taylor @ SparkFun Electronics:
  https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library

  Hardware Connections (CCS811 to ESP8266):
  3.3V to 3.3V pin
  GND to GND pin
  SDA to D2
  SCL to D1


  Connects to Wifi network, then sends samples via GET request to Pushingbox, which forwards it to Google Sheets.
  Requires setup of a Pushingbox Service and Scenario, and Google Sheet and published app:
  https://www.hackster.io/detox/transmit-esp8266-data-to-google-sheets-8fc617
  
  Writes data to Pushingbox as:
    CO2_ppm
    TVOC_ppb
    Temp_K
    Press_Pa
    RH
    
  1. Define a PB GET Service as:
    ?CO2=$CO2_ppm$&TVOC=$TVOC_ppb$&Temp=$Temp_K$&Press=$Press_Pa$&RH=$RH$
  2. Create a Google sheet
  3. Create a google app using Code.gs
  4. Publish app.
  
