from PIL import Image
import sys
import math as m
import webbrowser

import random as r

def cos(inputVar):
	return m.cos(m.radians(inputVar))

def sin(inputVar):
	return m.sin(m.radians(inputVar))


stepOutput = open("inCoords.txt", "w")

xSize = float(sys.argv[1])
ySize = float(sys.argv[2])
count = int(sys.argv[3])



for i in range(count):
	x = xSize * r.randint(0,1000)/1000
	y = ySize * r.randint(0,1000)/1000


	lefDist = x
	rigDist = xSize - x

	topDist = y
	botDist = ySize - y

	rad = lefDist
	if(rigDist < rad):
		rad = rigDist
	if(topDist < rad):
		rad = topDist
	if(botDist < rad):
		rad = botDist

	# angle = 360 * i / pointCount
	# y = cos(angle) * radius
	# x = sin(angle) * radius
	stepOutput.write(str(x + rad*cos(0)) + ' ' + str(y + rad*sin(0))  + ' 0 \n')
	stepOutput.write(str(x + rad*cos(144)) + ' ' + str(y + rad*sin(144))  + ' 1 \n')
	stepOutput.write(str(x + rad*cos(-72)) + ' ' + str(y + rad*sin(-72))  + ' 1 \n')
	stepOutput.write(str(x + rad*cos(72)) + ' ' + str(y + rad*sin(72))  + ' 1 \n')
	stepOutput.write(str(x + rad*cos(-144)) + ' ' + str(y + rad*sin(-144))  + ' 1 \n')
	stepOutput.write(str(x + rad*cos(0)) + ' ' + str(y + rad*sin(0))  + ' 1 \n')

stepOutput.close()
