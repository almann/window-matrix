## Overview
A simple tiled WS2811 tiled matrix (using NeoPixel libraries) using Arduino.
This code targets the ESP8266 (specifically Adafruit Huzzah).  The intention
is to leverage the MCU's native WiFi, but currently displayed images and patterns
are compiled into code.

## Image Conversion
Included in this package is the `Image2RGBArray.py` script.  This uses the
Python Image Library (PIL) to convert bitmap images into a C/C++ code that
represents the images, the generated file is typically `Images.h`.

Ex.

```
./Image2RGBArray.py christmas-bmp/*.bmp > Images.h

```

## TODO
* Use SPIFFS to store image/pattern data instead of embed in code.
* WiFi control and image/pattern upload.
