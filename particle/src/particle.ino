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
  bool success = Particle.function("tare", tare);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  scale.begin();
  publishChar("Before setting up the scale:", "");
  publishNumber("read:", scale.read()); // print a raw reading from the ADC

  publishNumber("read average:", scale.read_average(20)); // print the average of 20 readings from the ADC

  publishNumber("get value:", scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  publishNumber("get units:", scale.get_units(5)); // print the average of 5 readings from the ADC minus tare weight (not set) divided
                                                   // by the SCALE parameter (not set yet)

  scale.set_scale(); // this value is obtained by calibrating the scale with known weights; see the README for details

  publishChar("After setting up the scale:", "");

  publishNumber("read", scale.read()); // print a raw reading from the ADC

  publishNumber("read average", scale.read_average(20)); // print the average of 20 readings from the ADC

  publishNumber("get value", scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  publishNumber("get units", scale.get_units(5)); // print the average of 5 readings from the ADC minus tare weight, divided
                                                   // by the SCALE parameter set with set_scale

  publishChar("Readings:", "");
}
void loop()
{
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH); // put the ADC in sleep mode
  publishNumber("single/value", scale.get_value());
  publishNumber("average/value", scale.get_value(10));
  publishNumber("single/units", scale.get_units());
  publishNumber("average/unit:", scale.get_units(10));
  delay(100);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}