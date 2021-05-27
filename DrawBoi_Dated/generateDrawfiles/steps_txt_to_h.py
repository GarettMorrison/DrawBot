# import string as s

stepTotal = sum(1 for line in open('steps.txt'))

step_h = open('steps.h', 'w')

step_h.write("#ifndef _steps_H \n")
step_h.write("#define _steps_H \n")

step_h.write("#include <Arduino.h> \n")
step_h.write("#include <avr/pgmspace.h> \n")

step_h.write("#define number_of_steps " + str(stepTotal) + ' \n')

step_h.write("static int stepCount = number_of_steps; \n")

step_h.write("static const PROGMEM byte steps[number_of_steps] = { \n")

for line in open('steps.txt'):
	step_h.write('\t' + line.strip() + ',\n')

step_h.write("}; \n")


step_h.write("#endif //_steps_H \n")