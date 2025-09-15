/**
 **************************************************
 *
 * @file        CCS811-SOLDERED.h
 * @brief       Header file for CCS811 Environmental sensor
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     @ soldered.com
 ***************************************************/

#ifndef __SENSOR__
#define __SENSOR__

#include "Arduino.h"
#include "libs/Generic-easyC/easyC.h"
#include "libs/SparkFun_CCS811_Arduino_Library/src/SparkFunCCS811.h"

class CCS_811 : public CCS811
{
  public:
    CCS_811() : CCS811(0x5A)
    {
    }

  protected:
  private:
};

#endif
