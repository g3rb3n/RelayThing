#include <Arduino.h>
#include <unity.h>

#include "HX711.h"

#ifdef UNIT_TEST

using namespace g3rb3n;

#define SCALE_DOUT_PIN D2
#define SCALE_SCK_PIN D3

HX711 scale(SCALE_DOUT_PIN, SCALE_SCK_PIN);

void readsProperValue() 
{
  float value = scale.get_units(4);
  TEST_ASSERT_FLOAT_WITHIN(10, 0, value);
}

void setup() 
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);
  UNITY_BEGIN();

  scale.set_scale(108100.);
  scale.tare();
}

void loop() 
{
  uint8_t count = 100;
  while(--count)
  {
    RUN_TEST(readsProperValue);
  }
  UNITY_END();
}

#endif
