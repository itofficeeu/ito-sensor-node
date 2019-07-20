#include <Wire.h>
#include <SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/* Set only your height above see level unless you know how everything works.
 * Further readings about sea level and height above sea level:
 * https://en.wikipedia.org/wiki/Sea_level */
float local_pressure_hpa = 0;
float height_above_sea_in_meters = 0;
Adafruit_BME280 bme_sensor;

/* Weather to output information in the terminal/monitor */
#define DEBUG_MONITOR 1

/* Regards ML8511 and other analogue sensors in context to microcontrollers:
 * ANALOG_REF_V is determined be me by measuring voltage on an analogue input
 * and adjust this number to reflect the measured voltage.
 * Or, provisional, by adjusting it until the sensors give an expected value.
 * This value is individual to each ESP32.
 * This may fluctuate and should be resolved by a real life reference value,
 * based on electronic components. */
static float ANALOG_REF_V = 4.9;

/* Maximum analogue input to be able to get 3.3 V out in the middle the two resistors.
 * Link for calculating possible resistor values: 
 * http://www.ohmslawcalculator.com/voltage-divider-calculator */
float VOLT_DIVIDER_1_VOLT_MAX = 6.6;
//float VOLT_DIVIDER_2_VOLT_MAX = 6.6;
//float VOLT_DIVIDER_3_VOLT_MAX = 6.6;
//float VOLT_DIVIDER_4_VOLT_MAX = 6.6;

/* ML8511 - UV hardware pin definitions
 * Be aware that ADC2 can only be used for the sensor when no WiFi or Bluetooth is used.
 * This is due to WiFi and Bluetooth is occupying ADC2 to operate (ESP32).
 * Pin 12 belongs to ADC2
 * Pin 33 belongs to ADC1 but is used for something by the TTGO.
 * Pin 36 can be used (tested by example) */
int UV_SENSOR_PIN = 36; 
/* Competes with other analogue sensors for a pin number */

/* Gas sensor voltage
 * Measured with a voltage divider, as a pin only can handle up to 3.3V. */
int MQ135_VOLTAGE_PIN = 36;
/* Competes with other analogue sensors for a pin number */
float MQ135_VOLTAGE_CALIBRATION = 0.30;

/* 1-wire */
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_PIN 17
/* Setup oneWire bus to communicate with OneWire devices */
OneWire oneWire(ONE_WIRE_BUS_PIN);
/* Pass our oneWire reference to Dallas Temperature sensor libraries */
DallasTemperature sensors(&oneWire);

/* Create the individual variables for the 1-wire sensors.
 * Link to program to read out addresses/IDs:
 * https://www.pjrc.com/teensy/td_libs_OneWire.html
 * Link for further information on 1-wire:
 * https://playground.arduino.cc/Learning/OneWire/ */
// Example device address/ID: 28 70 F5 E0 16 13 1 C7
//DeviceAddress sensor1 = { 0x28, 0x70, 0xF5, 0xE0, 0x16, 0x13, 0x1, 0xC7 };
// Example device address/ID: 28 1F 6C B 17 13 1 12
// Example device address/ID: 28 EA 2F F7 16 13 1 E4
DeviceAddress sensor1 = { 0x28, 0x1F, 0x6C, 0xB, 0x17, 0x13, 0x1, 0x12 };
//DeviceAddress sensor1 = { 0x28, 0xEA, 0x2F, 0xF7, 0x16, 0x13, 0x1, 0xE4 };

// To use with the onboard OLED display
#define PIN_SDA 4
#define PIN_SCL 15

#include <AsyncDelay.h>
#include <SoftWire.h>
SoftWire sw(PIN_SDA, PIN_SCL);

/* I2C sensors can per default not use pins 21 and 22 when the display is used with SSD1306.h.
 * One solution can be to let the physical wiring of the I2C sensors use pin 4 and 15. */
/* Use DISPLAY_USE to turn off the display. */
#define DISPLAY_USE 1
#define DISPLAY_ON 1
SSD1306 display(0x3C, 4, 15);

/* A counter for different purposes
 * 1) To check visually that new data gets added.
 * 2) To let different things happen on modulus (%) loops. */
uint16_t count_loop = 0;

/*
 * The setup routine runs once when you press reset:
 */
void setup() {
  /* initialize serial communication at 115200 bits per second: */
  Serial.begin(115200);

  /* Set the pin mode for the gas sensor to be able to read the value */
  pinMode(MQ135_VOLTAGE_PIN, INPUT);

  /* Start 1-wire */
  sensors.begin();

#if DISPLAY_USE
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW); // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH);
  delay(50);
  display.init();
  display.setFont(ArialMT_Plain_10);
  delay(50);
  display.drawString( 0, 0, "Starting up ...");
  display.drawString( 0,20, "- and initializing...");
  display.display();
#endif
  
  if (!bme_sensor.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  /* The pressure is set to the level where the sensor is turned on.
   * Moving up and down in height will be reflected in the measuring followed. */
  local_pressure_hpa = bme_sensor.readPressure() / 100.0F;

  HeaderPrint();
}


/*
 * the loop routine runs over and over again forever:
 */
void loop() {

  /* Print voltage returned by the gas sensor */
  // Right now disabled due to the competing pin number and pin headers.
  //float voltage = Voltage(MQ135_VOLTAGE_PIN, MQ135_VOLTAGE_CALIBRATION); // sensorValue * (6.6 / 1023.0);
  //ColumnAdd(5, voltage, 1);

  /* Print UV radiation */
  float uv_mW_cm2 = UV_Read_mW_cm2(UV_SENSOR_PIN, VOLT_DIVIDER_1_VOLT_MAX);
  ColumnAdd(4, uv_mW_cm2, 1);

  /* Print data from 1-wire sensors */
  sensors.requestTemperatures(); // Send the command to get temperatures
  delay(100);
  float temp_liquid = sensors.getTempC(sensor1);
  ColumnAdd(5, temp_liquid, 1);

  /* Print data from the sensor BME280 */
  float temp_air = bme_sensor.readTemperature();
  float humi     = bme_sensor.readHumidity();
  float pres     = bme_sensor.readPressure() / 100.0F;
  float alt      = bme_sensor.readAltitude(local_pressure_hpa) +height_above_sea_in_meters;
  ColumnAdd(5, temp_air, 1);
  ColumnAdd(5, humi,  0);
  ColumnAdd(5, pres, 2);
  ColumnAdd(5, alt, 1);

  Serial.println("");

#if DISPLAY_USE
    digitalWrite(16, LOW); // set GPIO16 low to reset OLED
    // Rest commented out so that the display wouldn't use power from the battery.

#if DISPLAY_ON
    delay(50);
    digitalWrite(16, HIGH);
    display.init();
    display.setFont(ArialMT_Plain_10);

    /* Line 1 */
    display.drawString( 0,0, "Count:            : itoffice.eu");
    /* DisplayPrint(int indent_base, int pixel_y, float/int value_number, int decimals) */
    DisplayPrint(47, 0, count_loop, 0);

    /* Line 2 */
    display.drawString( 0,10, "Humi. ");
    /* DisplayPrint(int indent_base, int pixel_y, float value_number, int decimals, String unit) */
    DisplayPrint(25, 10, humi, 0, "%");

    /* Line 3 */
    display.drawString(0,20, "Pres.        ");
    DisplayPrint(24, 20, pres, 2);
    display.drawString(66, 20, "Alt.              ");
    if(alt<1)
    {
      /* Add decimal if altitude is lower than 1 meter */
      DisplayPrint(80, 20, alt, 1, "m");
    }
    else
    {
      /* Reduce decimal if altitude is higher than 1 meter */
      DisplayPrint(80, 20, alt, 0, "m");
    }

    /* Line 4 */
    display.drawString( 0,30, "Air Temp.              ");
    DisplayPrint(60, 30, temp_air, 1, "C");

    /* Line 5 */
    display.drawString( 0,40, "Liquid Temp.               ");
    DisplayPrint(60, 40, temp_liquid, 1, "C");

    /* Let the display show the data above sat. */
    display.display();
#endif
#endif

  count_loop++;
  /* Print each 10 line the header line. */
  if( (count_loop%10)==0 )
  {
    HeaderPrint();
  }

  delay(2000);
}

float UV_Read_mW_cm2(int pinToRead, float volt_divider_volt_max)
{
  int uv0to4095 = averageAnalogRead(pinToRead);
  float volt_not_calibrated = mapfloat(uv0to4095, 0, 4095, 0, 3.3);
  float volt_calibrated = (ANALOG_REF_V/3.3) * volt_not_calibrated;
  float volt_divider_ratio = volt_divider_volt_max / 3.3;
  float volt_div_volt_in = volt_calibrated * volt_divider_ratio;

  /* The ML8511 intensity graph tells that it always at least outputs 1V, */
  /* which equals 0.00 mW/cm2. */
  /* It tells further that it maximal outputs 3V, that is equal to 15.00 mW/cm2. */
  float mW_cm2 = mapfloat(volt_div_volt_in, 0.99, 2.9, 0.0, 15.0);
  if (mW_cm2<0)
  {
    mW_cm2 = 0;
  }
  return mW_cm2;
}

float Voltage(int PIN, float CALIBRATION)
{
  int voltage0to4095 = averageAnalogRead(PIN);
  float battery_voltage = mapfloat(voltage0to4095, 0, 4095, 0, 6.6);
  return battery_voltage + CALIBRATION;
}

/* Takes an average of readings on a given pin + Return the average */
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8; // This number can be reduced by putting a 0.1nF capacitor on the sensors to ground.
  unsigned int runningValue = 0; 
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
  return(runningValue);  
}

/* The Arduino Map function but for floats */
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* Author: Andreas C. Dyhrberg */
void DisplayPrint(int indent_base, int pixel_y, float value_number, int decimals)
{
  int prefill_to_align_right = DisplayPrefill(value_number);
  display.drawString((indent_base+prefill_to_align_right), pixel_y, String(value_number, decimals));
}

/* Author: Andreas C. Dyhrberg */
void DisplayPrint(int indent_base, int pixel_y, float value_number, int decimals, String unit)
{
  int prefill_to_align_right = DisplayPrefill(value_number);
  if(value_number<0 && prefill_to_align_right>3)
  {
    prefill_to_align_right = prefill_to_align_right-4;
  }
  display.drawString((indent_base+prefill_to_align_right), pixel_y, String(value_number, decimals));
  display.drawString((indent_base+20+(decimals*8)),pixel_y, unit);
}

/* A helper function to indent a value so that right side is aligned vertically
 * Author: Andreas C. Dyhrberg */
int DisplayPrefill(int value) {
  int prefill = 0;
  if(value < 100){
     prefill = 6;
  }
  if(value < 10){
      prefill = prefill+6;
  }
  return prefill;
}

/* Add another value as a column in the serial monitor.
 * param i: stands for the width of the column.
 * param value: value to show.
 * Author: Andreas C. Dyhrberg */
void ColumnAdd(int i, float value, int decimals) {
  if (value>=0) {
    Serial.print(" ");
  }
  while(i) {
    if ( value< (pow(10,i)) ) {
      Serial.print(" ");
    }
    i--;
  }
  Serial.print(value, decimals);
  Serial.print(";");
}

void HeaderPrint()
{
#if DEBUG_MONITOR
  Serial.println("");
  //Serial.print("     volt;");
  Serial.print("UV-Index;");
  Serial.print("Liquid *C;");
  Serial.print("   Air *C;");
  Serial.print("  Hum %;");
  Serial.print(" Pres. hPa;");
  Serial.print(" Altitude;");
  Serial.println("");
#endif
}
