#Johnny Dive

My first attempt at building a semi-decent robot using Arduino!


The robot has the following spec/features:

2x DC motors power the robot using a tank-style chassis
1x Ultrasonic distance sensor mounted to a servo
2x On-board status LED


The robot uses the following guidelines to navigate:

1. Where possible, always move forward
2. If an object is in the way, look left, then right to find which direction has the most free space
3. Reverse/pivot to avoid object
4. GOTO 1 :-)

