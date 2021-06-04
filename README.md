### Code for large drawing robot

This is the WIP codebase for the DrawBot portion of my AI Art experiment. The target is to create an impressionist drawing of a target image on a canvas using procedural generation and robotics. 

Work in progress, SmallBoi.ino and controlSmallBoi.py have fully functional communication and can execute commands and scripts. DrawDXF.py converts dxf files to a list of commands readable by the controller script. 

SmallBoi is a simple stepper driven pulley system which, by controlling the length of two strings to a center mechanism, can move the head to any point on the surface. It also has a pair of servos on the head that can select one of four markers for printing. 

BigBoi consists of two DIY servos (referred to as bigServos) and a small hobby servo. Each bigServo is a DC motor driven by an L298N driver. It is connected via a gear (which gives a torque advantage) to a large spool, whos position is measured via potentiometer. These two spools connect to joints on a large (6ft tall) 2 axis arm, allowing control of the position of its head. The hobby servo control the pen on the head of the robot, by driving a spool which runs over pulleys, up through the arm, and around another spool which rotates the pen's angle. I used cheap DC motors, which are comically underpowered for the scale of the mechanism. I intend to add a pulley system to give ridiculously high mechanical advantage, which will be slow but able to draw without buying additional motors. 

Blog post in progress [here](https://www.garettmorrison.net/posts/in-progress/#drawbot)
