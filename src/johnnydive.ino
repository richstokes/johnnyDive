/*Automatically drive a tracked robot
//Power research:
//4V in, 2.5V out with 3x AAA powering motor shield
//7.2v battery @ speed 110 (PWM) = 4.5v


The following pins are always in use by the Motor sheild:

Function	        Channel A	Channel B
Direction	        Digital 12	Digital 13
Speed (PWM)     	Digital 3	Digital 11
Brake           	Digital 9	Digital 8
Current Sensing 	Analog 0	Analog 1

//Notes on R3 Motor sheild
The power pins are as follows:

Vin on the screw terminal block, is the input voltage to the motor connected to the shield.
An external power supply connected to this pin also provide power to the Arduino board on which is mounted. By cutting the "Vin Connect" jumper you make this a dedicated power line for the motor.

Motor wires:
Blue and yellow = motor A
Blue and yellow = motor B
Black = aux power ground
Red = aux power +7.2v battery pack

Servo wires: (Ultrasonic sensor is mounted to servo so that robot can "look around")
Yellow (signal wire) = Digital PIN 10
Power and ground



Ultrasonic sensor wires:
Black = ground
Red = 5v
Yellow = Echo
Green = Trigger

Chassis is grounded to make it easy to connect optional componants by using this as a shared GND.

Optional LEDs are connected to show what the robot is trying to do.
Status updates are printed to serial if Arduino is connected via USB.



*/
#include <Servo.h>

//Hardware set up
#define echoPin 5 // Ultrasonic Echo Pin
#define trigPin 4 // Ultrasonic Trigger Pin
#define greenLEDPin 6 // Green LED (Optional)
#define redLEDPin 7 // Red LED (Optional )
//Servo set up
Servo eyeServo;  // create servo object to control a servo, twelve servo objects can be created on most boards
int pos = 90;    // variable to store the servo position, 90 means we initallly center it across the range of movement (0-180 degrees)
int servoPIN = 10;
int ledPin1 = 13;      // select the pin for the first LED
int sensorValue = 0;  // variable to store the value coming from the ultrasonic sensor
int nextDirection; //stores result of lookAround function

//Variables
int maximumRange = 30; // Maximum range of object. i.e. how close should we get (max 255)
int minimumRange = 0; // Minimum range
long duration, distance, leftDistance, rightDistance; // Duration used to calculate distance.
boolean dirchange = false; // Tracks if the robot has to make a direction change. Can be used to stop/brake motors before turning

void setup() {
  Serial.begin (9600);
  Serial.println("Setting up the robot...");
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel B pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel B pin
  Serial.println("Motors initalized...");

  //Bring up servo
  eyeServo.attach(servoPIN);  // attaches the servo on pin 9 to the servo object

  //Setup distance sensor and LEDs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLEDPin, OUTPUT); // Use Green LED indicator (optional)
  pinMode(redLEDPin, OUTPUT); //Use green LED indicator (optional)

}


void loop(){

  /* The following trigPin/echoPin cycle is used to determine the
   distance of the nearest object by bouncing soundwaves off of it. */
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);

   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);

   digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH);

   distance = duration/58.2;    //Calculate the distance (in cm) based on the speed of sound.

   if (distance >= maximumRange || distance <= minimumRange){
   Serial.println("-1");
   Serial.println("Looking clear!");
   digitalWrite(greenLEDPin, HIGH);

   //default movement
   //turnRight(255);
   forwards(255);   //move forward
   dirchange = true;
   }

   else {
     stop(1000); //Pause

         nextDirection = lookAround(); //run the lookAround function to find out direction with most free space

                 if (nextDirection == 1) {
                   Serial.println("We should go left next");
                   reverseTurnLeft(255);
                   //delay(3000); //increase for debugging
                 }
                 else {
                   Serial.println("We should go right next");
                   reverseTurnRight(255);
                   //delay(3000); //increase for debugging
                 }
                 Serial.println("");
                 stop(500);


}

     /* Original avoidance code. Commented out for testing the servo lookAround feature
   Serial.println(distance);    //send the distance
   Serial.println("Something in the way, turning around!");
   digitalWrite(greenLEDPin, LOW); // turn off green LED

      if (dirchange == true) {
        Serial.println("Change of direction detected, pausing..");
        stop(4000);
        dirchange = false; //switch
        reverseTurn(255);
        }
        else {
          Serial.println("No direction change, OK!");
          reverseTurn(255);
          dirchange = false;
          }
 }
   //Delay 10ms before next reading.
   delay(10);*/

}

// End of main loop !!! //


// Functions to make the main loop easier to follow and stop having to repeat code
void tprint(String text) {
  Serial.println(text);
}

void forwards(int speed)  //Move robot forwards at specified speed
{
  Serial.println("Moving forward");  //Motors forward @ full speed
  digitalWrite(13, HIGH); //Establishes forward direction  of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at 0 speed

  //Move motor A forwards
  digitalWrite(12, LOW); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed);   //Spins the motor on Channel A at speed
  delay(100);
}

void reverse(int speed) // Reverse robot at specified speed
{
  Serial.println("Reversing straight");
  digitalWrite(13, LOW); //Establishes reverse direction  of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at 0 speed

  //Move motor A forwards
  digitalWrite(12, HIGH); //Establishes reverse direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed);   //Spins the motor on Channel A at speed
  delay(100);
}

void turnRight(int speed) //Turns the robot in a circle to the right, approx 20 degrees
{
  Serial.println("Turning right (forwards)");
  digitalWrite(13, HIGH); //Establishes forward direction  of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at 0 speed

  //Move motor A forwards
  digitalWrite(12, LOW); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed/1.6);   //Spins the motor on Channel A at slow speed to aid turning
  delay(100);
}

void turnLeft(int speed) //WIP
{
  Serial.println("Turning left (forwards)");
}

void reverseTurnLeft(int speed) // Reverse robot at specified speed while rotating to the left (approx 45 degrees)
{
  //Turn while reversing right
  Serial.println("Reversing to the left");
  digitalWrite(redLEDPin, HIGH);
  //Motor B reverse
  digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed

  //Motor A Reverse
  digitalWrite(12, HIGH);  //Establishes reverse direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed/1.5);    //Spins the motor on Channel A at ~half speed

  delay(1000); //do this for x seconds in order to clear obstacle
  digitalWrite(redLEDPin, LOW);
}


void reverseTurnRight(int speed) // Reverse robot at specified speed while rotating to the right (approx 45 degrees)
{
  //Turn while reversing right
  Serial.println("Reversing to the right");
  digitalWrite(redLEDPin, HIGH);
  //Motor B reverse
  digitalWrite(13, LOW); //Establishes reverse direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed

  //Motor A Reverse
  digitalWrite(12, LOW);  //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed/1.5);    //Spins the motor on Channel A at ~half speed

  delay(1000); //do this for x seconds in order to clear obstacle
  digitalWrite(redLEDPin, LOW);
}

void stop(int duration)  //Motor speed the variable
{
  Serial.println("Stopping vehicle..");
  digitalWrite(8, HIGH);   //Engage the Brake for Channel B
  analogWrite(11, 0);   //Spins the motor on Channel B at 0 speed

  digitalWrite(9, HIGH);   //Engage the Brake for Channel A
  analogWrite(3, 0);   //Spins the motor on Channel A at 0 speed
  delay(duration);
}

int lookAround() {
  int rotSpeed = 50; //how fast we want the servo to rotate
  digitalWrite(redLEDPin, HIGH);
  digitalWrite(greenLEDPin, HIGH);

  //Function to look left then right to find clearest path
  //Returns 1 for left, 0 for right
  //Uncomment Serial.println's for debugging info

  //Look Left
  //Serial.println("Moving servo left...");

  for (pos = 90; pos <= 180; pos = pos + 10) { // goes from 0 degrees to 90 degrees
    // in steps of 10 degree
    eyeServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(rotSpeed);                       // waits 15ms for the servo to reach the position
    //Serial.print("left pos: "); Serial.println(pos);
  }
  //Get distance at left position
  delay(250); //delay to make sure servo is stopped at left position
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  leftDistance = duration/58.2;    //Calculate the distance (in cm) based on the speed of sound.
  Serial.print("Left distance: "); Serial.println(leftDistance);




  //Look Right
  //Serial.println("Moving servo right...");
    for (pos = 180; pos >= 0; pos = pos - 10) {
      eyeServo.write(pos);
      delay(rotSpeed);
      //Serial.print("right pos: "); Serial.println(pos);
    }

    //Get distance at right position
    delay(250);//delay to make sure servo is stopped at right position
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    rightDistance = duration/58.2;    //Calculate the distance (in cm) based on the speed of sound.
    Serial.print("Right distance: "); Serial.println(rightDistance);


  //Move back to center position
  Serial.println("Moving servo back to center...");
  eyeServo.write(90);
  delay(500);
  digitalWrite(redLEDPin, LOW);
  digitalWrite(greenLEDPin, LOW);

  //Figure out which direction has the most space
  if (leftDistance > rightDistance) {
    //Serial.println("Left has the most free space!!!!!");
    //Go back to center when done
    return 1;
  }
  else {
    //Serial.println("Right has the most free space!!!");
    //Go back to center when done
    return 0;
  }
}
