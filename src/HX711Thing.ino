#include <Thing.h>
#include <BlinkPattern.h>
#include <HX711.h>
#include <Arduino.h>

#define SCALE_DOUT_PIN D2
#define SCALE_SCK_PIN D3

using namespace g3rb3n;

Thing thing;
BlinkPattern led(BUILTIN_LED);
HX711 scale(SCALE_DOUT_PIN, SCALE_SCK_PIN);

float divider = 108100.;

BlinkPattern::Pattern<2> panic{{1,1},25};
BlinkPattern::Pattern<2> start{{1,9},25};
BlinkPattern::Pattern<2> normal{{1,39},25};
BlinkPattern::Pattern<0> disable{{},1000};

void setup() 
{
//  ESP.wdtDisable();

  Serial.begin(230400);
  Serial.println();
  Serial.println("Client:" + thing.clientId());

  led.setPattern(start);
  
  thing.onStateChange([](const String& msg){
    Serial.println(msg);
  });

  thing.addSensor(String("sensor/hx711/") + thing.clientId(), 1000, [](Value& value){
    digitalWrite(BUILTIN_LED, 1);
    float grams = scale.get_units(4);
    Serial.println(grams);
    digitalWrite(BUILTIN_LED, 0);
    value = grams;
  });

  thing.addActuator(String("reset/hx711/") + thing.clientId(), [](Value& value){
    scale.set_scale(108100.);
    scale.tare();
  });

  scale.set_scale(108100.);
  scale.tare();
  
  thing.begin();
  led.setPattern(disable);
}

void loop()
{
  thing.handle();
  led.handle();
}
