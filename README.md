# ECO Solar Boost MPPT-3000 PRO ESPHome External Component

This repository contains a component for [ESPHome](https://esphome.io/)
to support the AZO Digital ECO Solar Boost MPPT-3000 PRO inverter.

ESP32 acts as I2C bus sniffer and decodes messages displayed on inverter LCD. Tested with inverter version 5.49.

## Usage
Connect ESP32 to LCD module side of the inverter.
 - GND to GND
 - SDA data pin to configured GPIO, default 21
 - SCL data pin to configured GPIO, default 22
 - use 1K series resistor with diode or high speed opto coupler to connect data pins (e.g. 2ch VO0631T, 2x 1ch 6N137-L)
 - Use external power supply for ESP32, inverter does not provide enough 5V power
 - Additional optocoupler connected to 5V supply may be used to detect if inverter is on, `binary_sensor gpio`
```
  ___ ESP23 GPIOx
   |
  _|_
 _\_/_  1N4148
   |
  _|_
 |1K |
 |   |
 |_ _|
   |
  _|_ LCD SCD/SDA
```

![Diagram](./img/MPPT3000-LCD-wire.jpg)

File `mppt3000.yaml` contains example ESPHome configuration.
 - Default `update_interval` is 30 seconds as inverter reports all values every +/-20 seconds
 - Timeout filter is recommended for non incremental values
 - Data aquisition may get stuck, node may be rebooted using configured restart switch
 - Either use github link or copy `components` files to `esphome` local storage and set `- source: components`
