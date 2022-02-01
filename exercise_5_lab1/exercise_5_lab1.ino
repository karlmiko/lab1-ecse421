
/*
  Exercise 5: Displaying displacement data to an OLED Screen

  Task: Create a real-time distance calculation algorithm and test it by moving your accelerometer module a
  few centimeters across a surface uniaxially, to the best of your ability.
  Place a ruler parallel to this motion to estimate the “real” distance.
  Is the calculated measurement reliable? Provide an explanation for the results you obtained.

  No. My calculation was not reliable. As previusly mentioned, I could not account for the errors
  introduced to the values after each of the two integrations, therefore my final result does not
  provide a good estimation of the real displacement. However, I managed to add some logic to prevent
  some strange behaviours. For example, adding an acceleration threshold before updating the displacement
  helped having unwanted displacement changes when the sensor is not moving at all. Another factor that
  influenced my results was the poor handling of the "g" factor in the z-axis. I did not undestand how to properly
  account for gravity accelerating the object in the z-axis, so my z accelerations all include gravity.
  Plus, I was not able to convert my displayed values to meters or centimeters. Still, I was able to
  observe some relevant characteristics of the accelerometer, such as the acceleration in 3 axis, the
  importance of calibration, and the error factor introduced by integrating.

*/

#include <Arduino.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

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

// values taken from exercise 4
// from calibration
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

// previous values
float PrevAX = 0.00;
float PrevAY = 0.00;
float PrevAZ = 0.00;
float PrevVX = 0.00;
float PrevVY = 0.00;
float PrevVZ = 0.00;
float VelX = 0.00;
float DisX = 0.00;
float TotalX = 0.00;


void setup(void) {
  //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
  u8x8.begin(); // to start using the screen
  u8x8.setFlipMode(1);
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

void loop(void) {
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0); //defining the starting point for the cursor
  u8x8.print("--Position--");

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

  if (CalX > 0.05 || CalX < -0.05)
  {
    VelX = (PrevAX + CalX)*10 / 2;
    DisX = (PrevVX + VelX)*10 / 2;
    TotalX = TotalX + DisX;
    PrevVX = VelX;
    PrevAX = CalX;
  }

  Serial.print("X: "); Serial.print(CalX); Serial.print(" Y: "); Serial.print(CalY); Serial.print(" Z: "); Serial.print(CalZ); Serial.println();
  u8x8.setCursor(0, 1); //defining the starting point for the cursor
  u8x8.print("Acc X: "); u8x8.print(CalX);
  u8x8.setCursor(0, 2); //defining the starting point for the cursor
  u8x8.print("X: "); u8x8.print(TotalX);
  delay(10);

}
