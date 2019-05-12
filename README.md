<h1>Logging humidity-compensated CCS811 samples to Google Sheets</h1>

  Based heavily on code from Marshall Taylor @ SparkFun Electronics:
  https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library

  Hardware Connections (CCS811 to ESP8266):
  - 3.3V to 3.3V pin
  - GND to GND pin
  - SDA to D2
  - SCL to D1


  <p>Connects to Wifi network, then sends samples via GET request to Pushingbox, which forwards it to Google Sheets.</p>
  <p>Requires setup of a Pushingbox Service and Scenario, and Google Sheet and published app:
  <div>https://www.hackster.io/detox/transmit-esp8266-data-to-google-sheets-8fc617</p>
  
  Writes data to Pushingbox as:
    <ul><li>CO2_ppm</li>
    <li>TVOC_ppb</li>
    <li>Temp_K</li>
    <li>Press_Pa</li>
    <li>RH</li></ul>
    
  1. Define a PB GET Service as:
    <code>?CO2=$CO2_ppm$&TVOC=$TVOC_ppb$&Temp=$Temp_K$&Press=$Press_Pa$&RH=$RH$</code>
  2. Create a Google sheet
  3. Create a google app using Code.gs
  4. Publish app.
  
