#include <HX711ADC.h>
#include <OneWire.h>
#include <spark-dallas-temperature.h>

/*
 * Project particle
 * Description:
 * Author:
 * Date:
 */

#define HX711DOUT D3
#define HX711SCK D2
#define ONE_WIRE_BUS D4
#define REQUIRESALARMS false
#define REQUIRESNEW false

/**
 * helper function to publish integers e.g. for debugging
 */
void publishNumber(const char *name, int integer)
{
  publishNumber(name, (float)integer);
}

void publishNumber(const char *name, double value)
{
  char *buf;
  size_t bufz = snprintf(NULL, 0, "%f", value) * sizeof(char);
  buf = (char *)malloc(bufz);
  if (buf == NULL)
  {
    publishChar("Out of memory", "publishNumber");
    return;
  }
  snprintf(buf, bufz, "%f", value);
  publishChar(name, buf);
  free(buf);
}
void publishNumber(const char *name, long int value)
{
  char *buf;
  size_t bufz = snprintf(NULL, 0, "%ld", value) * sizeof(char);
  buf = (char *)malloc(bufz);
  if (buf == NULL)
  {
    publishChar("Out of memory", "publishNumber");
    return;
  }
  snprintf(buf, bufz, "%lf", value);
  publishChar(name, buf);
  free(buf);
}
/**
 * helper function to publish chars e.g. for debugging
 */
void publishChar(const char *name, const char *message)
{
  Particle.publish(name, message, 0, PRIVATE);
  delay(1000); // ensure not throttled
}

// establish HX711
HX711ADC scale(HX711DOUT, HX711SCK);

// set up onewire and dallas sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// setup() runs once, when the device is first turned on.
void setup()
{
  pinMode(D7, OUTPUT);
  scale.begin();
  sensors.begin();
  publishChar("Rebooted", "true");
}
void loop()
{
  digitalWrite(D7, !digitalRead(D7));
  sensors.requestTemperatures();
  publishNumber("measurement/weight/value", scale.get_value(10));
  publishNumber("measurement/weight/units", scale.get_units(10));
  publishNumber("measurement/temperature/c", sensors.getTempCByIndex(0));
  delay(100);
}