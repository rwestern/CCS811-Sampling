#include <HardwareSerial.h>
#include <SparkFunCCS811.h>

// Purloined from:
// https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library
//printDriverError decodes the CCS811Core::status type and prints the
//type of error to the serial terminal.
//
//Save the return value of any function of type CCS811Core::status, then pass
//to this function to see what the output was.
void printDriverError(CCS811Core::status errorCode)
{
  switch (errorCode)
  {
  case CCS811Core::SENSOR_SUCCESS:
    Serial.print("SUCCESS");
    break;
  case CCS811Core::SENSOR_ID_ERROR:
    Serial.print("ID_ERROR");
    break;
  case CCS811Core::SENSOR_I2C_ERROR:
    Serial.print("I2C_ERROR");
    break;
  case CCS811Core::SENSOR_INTERNAL_ERROR:
    Serial.print("INTERNAL_ERROR");
    break;
  case CCS811Core::SENSOR_GENERIC_ERROR:
    Serial.print("GENERIC_ERROR");
    break;
  default:
    Serial.print("Unspecified error.");
  }
}

//printSensorError gets, clears, then prints the errors
//saved within the error register.
void printSensorError(CCS811 &myCCS811)
{
  uint8_t error = myCCS811.getErrorRegister();

  if (error == 0xFF) //comm error
  {
    Serial.println("Failed to get ERROR_ID register.");
  }
  else
  {
    Serial.print("Error: ");
    if (error & 1 << 5)
      Serial.print("HeaterSupply");
    if (error & 1 << 4)
      Serial.print("HeaterFault");
    if (error & 1 << 3)
      Serial.print("MaxResistance");
    if (error & 1 << 2)
      Serial.print("MeasModeInvalid");
    if (error & 1 << 1)
      Serial.print("ReadRegInvalid");
    if (error & 1 << 0)
      Serial.print("MsgInvalid");
    Serial.println();
  }
}
