int this_read = 0;      // variable to store the read value
int last_read = 0;
int inputPin = 48;
int last_second = millis()/1000;
int revs = 0;

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
  Serial.begin(9600);
}

void loop() {
  int this_read = digitalRead(inputPin);   // read the input pin
  if (last_second != (millis()/1000)){
    Serial.print("revs this second: ");
    Serial.println(revs);
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
      revs += 1;
    }
  }
  delay(1);//without this 1ms delay or the presence of any of the print statements above, a revolution can sometimes be counted twice
}
