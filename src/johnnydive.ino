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

    Vin on the screw terminal block, is the input voltage to the motor connected to the shield. An external power supply connected to this pin also provide power to the Arduino board on which is mounted. By cutting the "Vin Connect" jumper you make this a dedicated power line for the motor.

Motor wires:
Blue and yellow = motor A
Blue and yellow = motor B
Black = aux power ground
Red/Black = aux power +5v

Ultrasonic sensor wires:
Black = ground
Red = 5v
Yellow = Echo
Green = Trigger

*/

#define echoPin 5 // Echo Pin
#define trigPin 4 // Trigger Pin
#define LEDPin 6 // Optional LED

int maximumRange = 30; // Maximum range of object. i.e. how close should we get (max 255)
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
boolean dirchange = false; // Tracks if the robot has to make a direction change. Can be used to stop/brake motors before turning

void setup() {
  Serial.begin (9600);
  Serial.println("Setting up the robot...");
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel A pin
  Serial.println("Motors initalized...");

  //Setup distance sensor and LED
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)

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
   digitalWrite(LEDPin, HIGH);
   turnRight(255);
   //forwards(255);   //move forward
   dirchange = true;
   }

   else {

   Serial.println(distance);    /* Send the distance */
   Serial.println("Something in the way, turning around!");
   digitalWrite(LEDPin, LOW); // turn a LED

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
   delay(10);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Functions to make the main loop easier to follow and stop having to repeat code
void tprint(String text) {
  Serial.println(text);
}

void forwards(int speed)  //Motor speed the variable
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

void reverse(int speed)
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

void turnRight(int speed)
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

void turnLeft(int speed)
{
  Serial.println("Turning left (forwards)");
}

void reverseTurn(int speed)
{
  //Turn while reversing right
  Serial.println("Reversing to the right");
  //Motor B reverse
  digitalWrite(13, LOW); //Establishes reverse direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed

  //Motor A Reverse
  digitalWrite(12, LOW);  //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed/1.5);    //Spins the motor on Channel A at ~half speed

  delay(2000); //do this for x seconds in order to clear obstacle
}

void stop(int duration)  //Motor speed the variable
{
  Serial.println("Stopping vehicle..");  //Motor A forward @ full speed
  //Make sure motor B is not steering
  //digitalWrite(13, HIGH); //Establishes right direction of Channel B
  digitalWrite(8, HIGH);   //Engage the Brake for Channel B
  analogWrite(11, 0);   //Spins the motor on Channel B at 0 speed

  //Move motor A forwards
  //digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, HIGH);   //Engage the Brake for Channel A
  analogWrite(3, 0);   //Spins the motor on Channel A at speed
  delay(duration);
}

//Look around with turret to find clearest path
