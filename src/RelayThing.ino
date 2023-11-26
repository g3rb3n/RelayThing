#include <Thing.h>
#include <BlinkPattern.h>

#define PIN D1

using namespace g3rb3n;

Thing thing;
BlinkPattern led(BUILTIN_LED);

BlinkPattern::Pattern<2> start{{1,9},25};
BlinkPattern::Pattern<2> normal{{1,39},25};
BlinkPattern::Pattern<0> disable{{},1000};

bool state = false;

void setup()
{
  Serial.begin(230400);
  Serial.println();
  Serial.println("ClientID:" + thing.clientId());

  led.setPattern(start);

  pinMode(PIN, OUTPUT);

  thing.onStateChange([](const String& msg){
    Serial.println(msg);
  });

  thing.begin();

  thing.addActuator("things/" + thing.clientId() + "/relay/switch", [](Value& value){
    state = (bool)value;
    digitalWrite(PIN, state);
    Serial.println("Got:" + String(state));
    thing.publish("things/" + thing.clientId() + "/relay/state", value);
  });

  //thing.addSensor(String("sensor/relay/") + thing.clientId(), 1000, [](Value& value){
  //  value = state;
  //});

  led.setPattern(normal);
}

void loop()
{
  thing.handle();
  led.handle();
}
