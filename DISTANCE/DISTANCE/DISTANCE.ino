int this_read = 0;      // variable to store the read value
int last_read = 0;
int inputPin = 48;
int last_second = millis()/1000;
int revs = 0;

/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Pitch & Roll & Yaw Gyroscope Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

MPU6050 mpu;

// Timers
unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;

float pitch_base = 0;
float roll_base = 0;
float yaw_base = 0;

float pitch_delta = 0;
float roll_delta = 0;
float yaw_delta = 0;

float pitch_average_delta = 0;
float roll_average_delta = 0;
float yaw_average_delta = 0;

float pitch_sum = 0;
float roll_sum = 0;
float yaw_sum = 0;

float final_pitch = 0;
float final_roll = 0;
float final_yaw = 0;

int iteration_count = 0;

float totalX = 0.0;
float totalY = 0.0;
/*
 * how to??
 * the accelerometer and this system must be wired to the same arduino
 * 
 * 0 - set everything up, including perceived displacement
 * 1 - at runtime, get the current angle in the xy plane (whatever plane is parallel to the ground)
 * 2 - if a new revolution (magnet) is detected:
 *     - get current xy angle
 *     - get x and y components of that revolution
 *     - add calculated x and y components to to total x displacement and total y displacement
 *     - based on x and y components, determine overall displacement
 *     
 *  if a rider goes two feet forward, turns right 45, and goes two feet forward with a magnet at every 1 feet worth of revolution:
 *  magnet is detected, angle is read to be 0 degrees = 1 foot y, 0 feet x
 *  2 feet y, 0 feet x
 *  angle then read to be 45 -> 2.7 feet y, 0.7 feet x
 *  angle is still 45 for the last foot -> 3.4 feet y, 1.4 feet x
 *  in essence, displacement is calculated with a series of 1-foot long vectors that are added together to produce the final position
 *  
 *  
 */

/*
 * Digital 1 == HIGH, Digital 0 == LOW
 */

void setup() {
  pinMode(inputPin, INPUT);    // sets the digital pin 48 as input
  Serial.begin(115200);

  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3);
}

void loop() {
  gyroCalc();
  int this_read = digitalRead(inputPin);   // read the input pin
  if (last_second != (millis()/1000)){
    //Serial.print("revs this second: ");
    //Serial.println(revs);
    revs = 0;
    last_second = (millis()/1000);
  }
  //Serial.println(this_read);
  //Serial.println(last_read);
  if (this_read == last_read){
    //a
  }else{
    if (this_read == 0){
      last_read = this_read;
      //Serial.println("now off");
    }else{
      last_read = this_read;
      //Serial.println("now on");
      //revs += 1;

      totalX += (cos((final_yaw /180 * PI)));
      
      totalY += (sin((final_yaw /180 * PI)));
     
      Serial.print("Total Y traveled");Serial.println(totalY);
      }
  }
  delay(1);//without this 1ms delay or the presence of any of the print statements above, a revolution can sometimes be counted twice
  Serial.print("Yaw Angle");Serial.println(final_yaw);  
 
}

void gyroCalc(){
   //for (int i = 0; i <= 99; i++){
    timer = millis();
  
    // Read normalized values
    Vector norm = mpu.readNormalizeGyro();
  
    // Calculate Pitch, Roll and Yaw
    pitch = pitch + norm.YAxis * timeStep;
    roll = roll + norm.XAxis * timeStep;
    yaw = yaw + norm.ZAxis * timeStep;
  
    if (iteration_count == 0){
      pitch_base = pitch;
      roll_base = roll;
      yaw_base = yaw;
    }
    pitch_delta = pitch_base - pitch;
    roll_delta = roll_base - roll;
    yaw_delta = yaw_base - yaw;
    
    pitch_sum += pitch_delta;
    roll_sum += roll_delta;
    yaw_sum += yaw_delta;
    
    //if (i == 99){
      final_pitch = pitch;
      final_roll = roll;
      final_yaw = yaw;
    //}
    
    // Wait to full timeStep period
    delay((timeStep*1000) - (millis() - timer));
  //}
  
  pitch_average_delta = pitch_sum/100;
  roll_average_delta = roll_sum/100;
  yaw_average_delta = yaw_sum/100;
  
  pitch_base -= pitch_average_delta;
  roll_base -= roll_average_delta;
  yaw_base -= yaw_average_delta;
  
  pitch_sum = 0;
  roll_sum = 0;
  yaw_sum = 0;
  
  iteration_count += 1;

  if (final_yaw < 0){
    final_yaw += 360;
    }
  final_yaw = (int) final_yaw % 360; 
  //Serial.println(final_yaw);
  /* 
  Serial.print("|");
  Serial.print("GYRO");
  Serial.print("|");
  Serial.print(final_pitch);
  Serial.print("|");
  Serial.print(final_roll);
  Serial.print("|");
  Serial.print(final_yaw);
  Serial.print("|");
  Serial.print(pitch_average_delta);
  Serial.print("|");
  Serial.print(roll_average_delta);
  Serial.print("|");
  Serial.print(yaw_average_delta);
  Serial.println("|");
  */
}
