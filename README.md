# ito-sensor-node
Examples of how to read out data from sensors connected with I2C, 1-wire and analogue.

## Specific sensors demonstrated

### BME280 Hum.-Temp.-Pressure, 

### GYML8511 UV-Index, 

### DS18B20 Temperature Sensor

Most relevant specs of the DS18B20 temperature sensor:

* Communicates over one-wire bus communication
* Power supply range: 3.0V to 5.5V
* Operating temperature range: -55ºC to +125ºC
* Accuracy +/-0.5 ºC (between the range -10ºC to 85ºC)

Other analogue sensors could be:
* MQ135 Gas-CO2-Sensor


## Video-link
* https://youtu.be/mq7yYemgUjc

## Code Format
Arduino sketch .ino file type in mixed C/C++.

## Intended hardware
Heltec WiFi Lora 32, TTGO etc.

## License
The libraries used to compose this code might have different licenses.

## Voltage divider - Analogue sensors
Link for calculating possible resistor values: 
* http://www.ohmslawcalculator.com/voltage-divider-calculator

For each analogue sensors can you in the code easily set the voltage range with the parameter VOLT_DIVIDER_x_VOLT_MAX, wherein x can be 1,2,3 or 4.

## 1-wire
Link to program to read out addresses/IDs:
 * https://www.pjrc.com/teensy/td_libs_OneWire.html

Link for further information on 1-wire:
 * https://playground.arduino.cc/Learning/OneWire/
 
## Extension board and prototyping board
Available at
* https://itoffice.eu/product/itoffice-eu-extension-board-for-the-prototyping-board-m_v3-m_v4-and-raspberry-pi-gpios/

* https://itoffice.eu/product/itoffice-eu-prototyping-board-m_v4/
