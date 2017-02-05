#Johnny Dive

My first attempt at building a semi-decent robot using Arduino!


The robot has the following spec/features:

- 2x DC motors power the robot using a tank-style chassis
- 1x Ultrasonic distance sensor mounted to a servo
- 2x On-board status LED

9
The robot uses the following guidelines to navigate:

1. Where possible, always move forward
2. If an object is in the way, look left, then right to find which direction has the most free space
3. Reverse/pivot to avoid object
4. GOTO 1 :-)


#Parts List

If you'd like to follow along at home, you'll want something similar to:

- Arduino Uno R3
- Arduino Motorshield R3
- 9V battery (for powering Arduino)
- 7.2V battery (for powering motorsheild)
- Tracked robot chassis (I am using the Sinoning SN2400)
- 2x DC motors (I am using 33GB-520 12V 350RPM DC gear motor)
- Ultrasonic sensor (HC-SR04)
- 2x LEDs (optional)


#Set up

##Power notes
One important thing I learned, while the Motorshield can draw power direct from the Arduino, this will not provide enough current to successfully drive motors.

The way around this is to cut the "VIN connect" jumper on the motorshield. Doing this lets you connect up a seperate power source to specifically drive the motors.


