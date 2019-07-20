# ito-sensor-node
Examples of how to read out data from sensors connected with I2C, 1-wire and analogue.

## Specific sensors demonstrated

### BME280 Hum.-Temp.-Pressure, I2C

Most relevant specs:

Temperature	-40°C to 85°C (±1.0°C accuracy)
Humidity	0 to 100% RH (±3% accuracy)
Pressure	300hPa to 1100hPa (±1hPa accuraccy)
Altitude	0 to 30,000ft (±1 meter accuracy)

### DS18B20 Temperature Sensor, 1-wire

Most relevant specs:

* Communicates over one-wire bus communication
* Power supply range: 3.0V to 5.5V
* Operating temperature range: -55ºC to +125ºC
* Accuracy +/-0.5 ºC (between the range -10ºC to 85ºC)

### GYML8511 UV-Index, analogue

This sensor detects 280-390nm light most effectively. This is categorized as part of the UVB (burning rays) spectrum and most of the UVA (tanning rays) spectrum. It outputs a analog voltage that is linearly related to the measured UV intensity (mW/cm2).

### Other analogue sensors could be:
* MQ-135 Air Quality Sensor

  https://www.aliexpress.com/item/32776781932.html?spm=a2g0x.search0104.3.2.59d04024NeVcOV&ws_ab_test=searchweb0_0%2Csearchweb201602_5_10065_10068_10547_319_317_10548_10696_10084_453_10083_454_10618_10304_10307_10820_10821_10301_537_536_10843_10059_10884_10887_321_322_10103%2Csearchweb201603_52%2CppcSwitch_0&algo_expid=fc3978c3-5882-4f7c-ace0-9d51b9830a4c-0&algo_pvid=fc3978c3-5882-4f7c-ace0-9d51b9830a4c&transAbTest=ae803_4

* Soil Moisture Sensor

  https://de.aliexpress.com/item/32951556560.html?spm=a2g0s.9042311.0.0.27424c4dyGtgmo

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
