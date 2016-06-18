/*
 * ISC Power Wheels controller code Summer 2016
 * throttle: https://www.amazon.com/dp/B00H4GN5O6
 * motor driver: https://www.pololu.com/product/2995
 * 
 * circuit layout:
 * gnd -> throttle black
 * gnd -> motor driver gnd
 * 5v -> throttle red
 * a0 -> throttle green
 * 9 -> motor driver pwm
 * wire motor driver inputs in parallel to control 2 motors
 */

int debug = 0;
// for detecting if connected to a computer or not

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // for debugging purposes only
  while(Serial.available()==0 && millis()<4000);
  // wait 4 seconds for serial input to enable debug mode
  {
    int test = Serial.read(); //We then clear the input buffer
    Serial.println("DEBUG"); //Give feedback indicating mode
    debug = 1; //Enable debug
  }
  if (0 == debug)
  {
    Serial.end();
  }
  
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  //Serial.println(sensorValue);
  // range: 200 - 850
  if (sensorValue < 150) or (sensorValue > 900)
  {
    // error: out-of-range - throttle may be disconnected
    sensorValue = 200;
  }
  else if (sensorValue < 200)
  {
    // avoid jitter near minimum throttle
    sensorValue = 200;
  }
  else if (sensorValue > 850)
  {
    // avoid jitter near maximum throttle
    sensorValue = 850;
  }
   int pwm = map(sensorValue, 200, 850, 0, 255);
   analogWrite(9, pwm);
   if (debug)
   {  
    Serial.println("pwm " + pwm + " raw: " + sensorValue);
   }
  
  delay(1);        // delay in between reads for stability
  
}
