### Code for large drawing robot

Work in progress, I mostly wanted to back up the code for the servos while I work on an improved mechanism. 

The robot consists of two DIY servos (called bigServos) and a small hobby servo. Each bigServo is a DC motor driven by an L298N driver. It is connected via a gear (which gives a torque advantage) to a large spool, whos position is measured via potentiometer. These two spools connect to joints on a large (6ft tall) 2 axis arm, allowing control of the position of its head. The hobby servo control the pen on the head of the robot, by driving a spool which runs over pulleys, up through the arm, and around another spool which rotates the pen's angle. 

I used cheap DC motors, which are comically underpowered for the scale of the mechanism. I intend to add a pulley system to give ridiculously high mechanical advantage, which will be slow but able to draw without buying additional motors. 