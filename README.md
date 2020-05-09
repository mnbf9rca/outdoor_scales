# outdoor_scales

a simple outdoor scales using 4 x load cells combined with an HX711 and a Particle Photon to route data to the cloud.

## Particle / Arduino code

This code is designed to run on the Particle Photon device. It makes use of the great HX711ADC library from <https://github.com/eliteio/HX711ADC/>

It assumes:

- HX711 DOUT / DT is connected to pin D3
- HX711 SCK is connected to pin D2

The LED blinks every time a reading is taken.
