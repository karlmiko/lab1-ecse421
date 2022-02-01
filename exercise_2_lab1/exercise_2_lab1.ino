/*
  Exercise 2: Using the pushbutton

  Task: Using your knowledge from the current and previous tasks,
  write a program that turns on the LED when the pushbutton is pressed,
  and blinks the LED when the pushbutton is not pressed.

  Quesiton: What is the smallest value of change in voltage
  that these analog inputs can report?  Explain your reasoning.

  The voltage reading range from 0V to 5V, and they are expressed by 10-bits of precision.
  Assuming linear quantization, then the smallest value of change in voltage that
  can be reported is 5V/2^10 = 5/1024 = 4.88 mV
  
*/

// based off https://www.arduino.cc/en/Reference/digitalRead
int ledPin = 2;   // LED connected to D2
int inPin = A3;   // pushbutton connected to A3
int val = 0;      // variable to store the read value

// setup function taken from Blink example
void setup() {
  pinMode(ledPin, OUTPUT);  // sets the D2 pin as output
  pinMode(inPin, INPUT);    // sets the A3 pin as input
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(ledPin, HIGH);  // turn the LED on
  delay(50);                   // wait 50 ms
  
  val = digitalRead(inPin);    // read the input pin
  if (val <= 0) {              // if button is not pressed
    digitalWrite(ledPin, LOW); // Turn the LED off
    delay(50);                 // wait 50 ms for blinking effect
  } 
}
