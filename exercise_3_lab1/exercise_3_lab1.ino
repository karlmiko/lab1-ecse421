/*
  Exercise 3: Using the potentiometer

  Task: Choose any value in between this range, and write a program that turns on
  the external LED only when the value of the potentiometer is higher than this threshold.

  Task: Write a program that causes the LED to blink at a frequency
  proportional to the potentiometer value, blinking at 1 Hz when the potentiometer is at its lowest value,
  10 Hz when the potentiometer is at its highest value,
  and transitioning continuously as the potentiometer value changes between these extremes.
  
*/


int analogValue = 0;    // variable to hold the analog value
int inPin = A0;         // potentiometer connected to A0
int ledPin = 2;         // LED connected to D2
int THRESHOLD = 612;    // threshold value for turning on LED
float halfDelay = 100.0;

void setup() {
  // open the serial port at 9600 bps:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  // sets the D2 pin as output
}

void loop() {
  // read the analog input on pin A0:
  analogValue = analogRead(inPin);

  // first task
//  if (analogValue >= THRESHOLD) {
//    digitalWrite(ledPin, HIGH);
//  }
//  else {
//    digitalWrite(ledPin, LOW);
//  }
//  delay(10); // delay 10 ms before the next reading

  // second task
  /* 
     For f = 1/T:
     At f = 1 Hz --> 1 blink per second --> 1/1000ms --> 500 ms on, 500 ms off.
     At f = 10 Hz --> 10 blinks per second --> 1/100ms --> 50 ms on, 50 ms off.
     We have that (0, 1000 ms) and (1023, 100 ms) are two points,
     where 0 and 1023 are analogValue, and 1000 and 100 are delays of blinks in ms.
     
     Linear equation: m = 1000-100/0-1023 = -0.879765
                      y = -0.879765 x + b <-- plug in (0, 1000)
                      b = 1000
             So, TotalDelay = -0.879765*analogValue + 1000
                        
  */
  halfDelay = (-0.879765*(float)analogValue + 1000.0)/2.0;
  Serial.println(halfDelay*2.0);   // print TotalDelay as an ASCII-encoded decimal

  digitalWrite(ledPin, HIGH);
  delay(halfDelay);              // for half of total delay LED is on
  digitalWrite(ledPin, LOW);
  delay(halfDelay);              // for half of total delay LED is off
  
}
