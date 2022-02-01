/*
  Exercise 1: Working with the LED

  Task: Using the blink sketch as an example,
  blink your external LED in any non-symmetric
  pattern via delay(amount), where amount is measured in milliseconds.
  
*/

// setup function taken from Blink example
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin D2 as an output.
  pinMode(2, OUTPUT); // LED connected to pin D2
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for 500 ms
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(100);              // wait for 100 ms
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for 100 ms
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for 500 ms
}
