from PIL import Image
import sys
import math as m
import webbrowser

def cos(inputVar):
	return m.cos(m.radians(inputVar))

def sin(inputVar):
	return m.sin(m.radians(inputVar))


stepOutput = open("inCoords.txt", "w")

pointCount = int(sys.argv[1])
radius = int(sys.argv[2])

for i in range(pointCount):
	angle = 360 * i / pointCount
	y = cos(angle) * radius
	x = sin(angle) * radius
	stepOutput.write(str(x) + ' ' + str(y)  + ' 1 \n')

stepOutput.close()
