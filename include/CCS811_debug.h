#ifndef _CCS811_DEBUG_H_
#define _CCS811_DEBUG_H_

#include <SparkFunCCS811.h>

void printSensorError(CCS811 &myCCS811);
void printDriverError(CCS811Core::status);

#endif