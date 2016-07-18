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

#define THROTTLE_INPUT_PIN A0
#define MOTOR_OUTPUT_PIN 9

#define THROTTLE_DISCONNECT 150
#define THROTTLE_LOW 200
#define THROTTLE_HIGH 860
#define THROTTLE_SHORT 900

// for detecting if connected to a computer or not
int debug = 0;

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
  int throttleRaw = analogRead(THROTTLE_INPUT_PIN);

  // print out the value you read:
  ////Serial.println(sensorValue);

  int throttleAdjusted;

  if ((throttleRaw < THROTTLE_DISCONNECT) or (throttleRaw > THROTTLE_SHORT))  // safety check
  {
    // error: out-of-range - throttle may be disconnected
    throttleAdjusted = THROTTLE_LOW;
  }
  else if (throttleRaw < THROTTLE_LOW)  // avoid jitter near minimum throttle
  {
    throttleAdjusted = THROTTLE_LOW;
  }
  else if (throttleRaw > THROTTLE_HIGH)  // avoid jitter near maximum throttle
  {
    throttleAdjusted = THROTTLE_HIGH;
  }
  else  // Use raw throttle value
  {
    throttleAdjusted = throttleRaw;
  }
  
  int pwm = map(throttleAdjusted, THROTTLE_LOW, THROTTLE_HIGH, 0, 255);
  analogWrite(MOTOR_OUTPUT_PIN, pwm);
  
  if (debug)
  {  
    Serial.print("raw: ");
    Serial.print(throttleRaw);
    Serial.print(" pwm: ");
    Serial.println(pwm);
  }
  
  delay(1);        // delay in between reads for stability
}
