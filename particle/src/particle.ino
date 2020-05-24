// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>

/*
 * Project particle
 * Description:
 * Author:
 * Date:
 */

#define HX711DOUT D3
#define HX711SCK D2
int led1 = D0; // Instead of writing D0 over and over again, we'll write led1
// You'll need to wire an LED to this one to see it blink.

int led2 = D7; // Instead of writing D7 over and over again, we'll write led2

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

HX711ADC scale(HX711DOUT, HX711SCK);

int tare(String extra)
{
    scale.tare();
    return 1;
}

// setup() runs once, when the device is first turned on.
void setup()
{
  pinMode(led2, OUTPUT);
  scale.begin();
  publishChar("Rebooted", "true");
}
void loop()
{
  digitalWrite(led2, !digitalRead(led2));
  publishNumber("average/value", scale.get_value(10));
  publishNumber("average/units", scale.get_units(10));
  delay(100);

}