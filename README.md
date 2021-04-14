Lightning Detector and Shutter Shutter V1.0

This project is an auto-shutter for any camera that can use a shutter cable. Its set up to detect lightning and fire your camera much quicker than a human can.

It uses an ESP32 as a controller and a standard clear 5mm LED as a lightning detector.  Please see the schematic for more information.

Note about the LED used as a lightning detector: Standard LEDs are designed to light up when a voltage is applied across their terminals.  All standard LEDs also work in reverse, where a voltage is produced when a light is applied.  This voltage is measured by the ESP32's ADC and fed into the lightning detection unit.

The result is a microprocessor firing your camera's shutter quicker than we can even detect a flash with our own eyes.

You can watch a video demonstration here : https://www.youtube.com/watch?v=opdjW-LTlm4



