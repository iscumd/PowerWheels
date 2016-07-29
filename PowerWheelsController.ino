/*
 * ISC Power Wheels controller code Summer 2016
 * throttle: https://www.amazon.com/dp/B00H4GN5O6
 * motor driver: https://www.pololu.com/product/2995
 * 
 * circuit layout:
 * gnd -> throttle black
 * gnd -> motor driver gnd  // can be skipped if arduino and motors share common ground
 * 5v -> throttle red
 * a0 -> throttle green
 * a1 -> motor driver 1 CS
 * a2 -> motor driver 2 CS
 * 9 -> motor driver pwm (both)
 * wire motor driver inputs in parallel to control 2 motors
 */

#define THROTTLE_INPUT_PIN A0
#define MOTOR_OUTPUT_PIN 9
#define MOTOR_CURRENT_SENSE_OUTPUT_1 A1
#define MOTOR_CURRENT_SENSE_OUTPUT_2 A2

#define THROTTLE_DISCONNECT 150
#define THROTTLE_LOW 200
#define THROTTLE_HIGH 860
#define THROTTLE_SHORT 900
////#define DEBUG (d) Serial.println(#d + ": " + d); 
#define MAX_CURRENT 35 // amps


float pinvoltage_to_amps(int pinvoltage)
{
  float volts = pinvoltage * 5.0f / 1024.0f; // based on analogread function
  float millivolts = volts * 1000.0f; // 1000 millivolts = 1 volt
  float amps = (millivolts-50.0f)/20.0f; // 50 millivolt offset, 20 millivolts per amp
  return amps;
}

float PID (float Actual, float targetPoint)
{   //
     // PID START
   static float Last = 0;
   const float kP = 2.0f; //DEBUG(kP)
   const float kI = 0.0f;
   const float kD = 0.0f;  
   
   float Error = targetPoint - Actual;
  
   static float Integral;
   const float IntThresh = 1000.0f; 
   //const float ScaleFactor = 1.0f;
   // THIS CHECK DOESN'T DO ANYTHING RIGHT NOW
   if (abs(Error) < IntThresh){ // prevent integral 'windup'
   Integral = Integral + Error; // accumulate the error integral
   }
   else {
   Integral=0; // zero it if out of bounds
   }
   float P = Error*kP; // calc proportional term
   float I = 0;//Integral*kI; // integral term
   float D = (Last-Actual)*kD; // derivative term
   float Drive = P + I + D; // Total drive = P+I+D
   
   Last = Actual; 
   return Drive;
}

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
  float targetPoint = map(throttleAdjusted, THROTTLE_LOW, THROTTLE_HIGH, 0, MAX_CURRENT);
  
  int motor_currentsense_pin_1 = analogRead(MOTOR_CURRENT_SENSE_OUTPUT_1);
  int motor_currentsense_pin_2 = analogRead(MOTOR_CURRENT_SENSE_OUTPUT_2);
  
  float motor_amps_1 = pinvoltage_to_amps(motor_currentsense_pin_1);
  float motor_amps_2 = pinvoltage_to_amps(motor_currentsense_pin_2);
  
  float total_motor_amps = motor_amps_1 + motor_amps_2;

  float Drive = PID(total_motor_amps, targetPoint);

  if (Drive < 0)
  {
    Drive = 0;
  }
  
  float pwm = map(Drive, 0, MAX_CURRENT, 0, 255);//(Drive*1.0f)/ScaleFactor; // scale Drive to be in the range 0-255

  if (pwm < 0)
  {
    digitalWrite(MOTOR_OUTPUT_PIN, LOW);
  }
  else if (pwm > 254)
  {
    if (pwm < 0)
  {
    digitalWrite(MOTOR_OUTPUT_PIN, HIGH);
  }
  else
  {
    analogWrite(MOTOR_OUTPUT_PIN, pwm); // send PWM command to motor board
  }
  


  
  if (debug)
  {  
    
    Serial.print("  throttleRaw: ");
    Serial.print(throttleRaw);

    Serial.print("  total_motor_amps: ");
    Serial.print(total_motor_amps);

    Serial.print("  targetPoint: ");
    Serial.print(targetPoint);

    Serial.print("  Drive: ");
    Serial.print(Drive);
    
    Serial.print("  pwm: ");
    Serial.println(pwm);
  
  }
  
  delay(1000);        // delay in between reads for stability
}
