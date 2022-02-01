
/*
  Exercise 4: Recording displacement data from an accelerometer

  Shake the sensor around in each axis. Do you see the values changing? What do they mean?
  
  Yes, the values change. They each indicate acceleration in one of the axis.

  Task: Try to determine the resolution for each full-scale
  setting of the accelerometer through empirical measurements.
  Can you determine whether your measurements accord with the specifications
  for the accelerometer from the tables on pages 10 and 16 of the datasheet?
  Use the functions setHighSolution() and setPowerMode() to adjust the operating mode.
  You can find the values they can take on from the source header file,
  but note what bit combinations are required for each mode.

  Low_Power + HighResolution disabled    | 10 --> 0.20 resolution
  Normal_Power + HighResolution disabled | 00 --> 0.05 resolution
  Normal_Power + HighResolution enabled  | 01 --> 0.01 resolution
  Low_Power + HighResolution enabled     | 11 --> 0.20 resolution --> Low_power takes precedence

  Full Scale --> +-2g  --> HighResolution: Smallest change 0.01?
             --> +-4g  --> HighResolution: Smallest change 0.01?
             --> +-8g  --> HighResolution: Smallest change 0.01?
             --> +-16g --> HighResolution: Smallest change 0.01?


  Task: with the serial monitor window active, record and save your accelerometer movements, ideally,
  involving some repetitive patterns of motion, to a .csv file (e.g., by copy/pasting the serial monitor data).
  Make sure you have at least a few seconds of data.

  (See sample_acc.txt)

  Task: Plot these data using any tool of your choice (Excel, Python, MATLAB, etc.).
  How would you obtain the displacement using the data? Plot these data on a separate graph.

  (See ex_4_lab1_plots.ipynb)
  To obtain the displacement, one must first integrate the acceleration to find the velocity.
  By integrating the velocity again, we can find the displacement. However, there is an error
  added with every integration, the unknown C. Therefore, at the end, the graph we see does not
  correspond to the actual displacement.

  Task: Choose a suitable value of N and write a moving average filter. Run your data through it.
  Plot this against the raw (unfiltered) signal. What are the benefits/drawbacks of having a high N compared to a low N?

  (See ex_4_lab1_plots.ipynb)
  The benefits of having a smooth signal is that you eliminate some of the noise and outliers.
  The drawback is that you some data is lost and this can increase the overall error accumulated.
  
*/

// starter code taken from lab 1 description
#include "LIS3DHTR.h"
#ifdef SOFTWAREWIRE
#include <SoftwareWire.h>
SoftwareWire myWire(3, 2);
LIS3DHTR<SoftwareWire> LIS;       //Software I2C
#define WIRE myWire
#else
#include <Wire.h>
LIS3DHTR<TwoWire> LIS;           //Hardware I2C
#define WIRE Wire
#endif

// based off https://www.arduino.cc/en/Reference/digitalRead
int ledPin = 2;   // LED connected to D2
int inPin = A3;   // pushbutton connected to A3
int val = 0;      // variable to store the read value

// taken from https://learn.adafruit.com/adxl345-digital-accelerometer/programming
float AccelMinX = 0;
float AccelMaxX = 0;
float AccelMinY = 0;
float AccelMaxY = 0;
float AccelMinZ = 0;
float AccelMaxZ = 0;

/* From Calibration loop:
   Accel Minimums: -1.01 -1.03 -0.98 <-- "RawLow"
   Accel Maximums:  1.04  1.00  1.00 <-- "RawHigh" ==> "RawRange => X: 1.03+1.01=2.04, Y: 0.99+1.03=2.02, Z: 1.01+0.99=2.00"
*/

float RawLowX = -1.01;
float RawLowY = -1.03;
float RawLowZ = -0.98;
float RawRangeX = 1.01 + 1.04;
float RawRangeY = 1.03 + 1.00;
float RawRangeZ = 0.98 + 1.00;
float ReferenceRange = 2.00; // +/- 1.0
float ReferenceLow = -1.00;

// calibrated values
float CalX = 0.00;
float CalY = 0.00;
float CalZ = 0.00;

// raw values
float X = 0.00;
float Y = 0.00;
float Z = 0.00;

// example taken from https://github.com/Seeed-Studio/Seeed_Arduino_LIS3DHTR
void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
  };
  LIS.begin(WIRE, LIS3DHTR_ADDRESS_UPDATED); //IIC init
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_10HZ);
  LIS.setHighSolution(true);           // HR bit:   1
  LIS.setPowerMode(POWER_MODE_NORMAL); // Lpen bit: 0
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G); // +/- 2g
}

// display calibrated values loop
void loop()
{
  if (!LIS)
  {
    Serial.println("LIS3DHTR didn't connect.");
    while (1)
      ;
    return;
  }

  X = LIS.getAccelerationX();
  Y = LIS.getAccelerationY();
  Z = LIS.getAccelerationZ();

  // two-point calibration formula to calibrate each axis:
  // CorrectedValue = (((RawValueInput – RawLow) * ReferenceRange) / RawRange) + ReferenceLow
  CalX = ((((X - RawLowX) * ReferenceRange) / RawRangeX) + ReferenceLow);
  CalY = ((((Y - RawLowY) * ReferenceRange) / RawRangeY) + ReferenceLow);
  CalZ = ((((Z - RawLowZ) * ReferenceRange) / RawRangeZ) + ReferenceLow);

  Serial.print("X: "); Serial.print(CalX); Serial.print(" Y: "); Serial.print(CalY); Serial.print(" Z: "); Serial.print(CalZ); Serial.println();
  delay(100);
}


// calibration loop
//void loop()
//{
//  if (!LIS)
//  {
//    Serial.println("LIS3DHTR didn't connect.");
//    while (1)
//      ;
//    return;
//  }
//
//  // taken from https://learn.adafruit.com/adxl345-digital-accelerometer/programming
//  Serial.println("Type key when ready...");
//  while (!Serial.available()) {} // wait for a character
//
//  if (LIS.getAccelerationX() < AccelMinX) AccelMinX = LIS.getAccelerationX();
//  if (LIS.getAccelerationX() > AccelMaxX) AccelMaxX = LIS.getAccelerationX();
//
//  if (LIS.getAccelerationY() < AccelMinY) AccelMinY = LIS.getAccelerationY();
//  if (LIS.getAccelerationY() > AccelMaxY) AccelMaxY = LIS.getAccelerationY();
//
//  if (LIS.getAccelerationZ() < AccelMinZ) AccelMinZ = LIS.getAccelerationZ();
//  if (LIS.getAccelerationZ() > AccelMaxZ) AccelMaxZ = LIS.getAccelerationZ();
//
//  Serial.print("Accel Minimums: "); Serial.print(AccelMinX); Serial.print("  "); Serial.print(AccelMinY); Serial.print("  "); Serial.print(AccelMinZ); Serial.println();
//  Serial.print("Accel Maximums: "); Serial.print(AccelMaxX); Serial.print("  "); Serial.print(AccelMaxY); Serial.print("  "); Serial.print(AccelMaxZ); Serial.println();
//
//  while (Serial.available())
//  {
//    Serial.read();  // clear the input buffer
//  }
//}
