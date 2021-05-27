import math as m
import sys
from PIL import Image

#definitions
width = 19.75

LInitPos = 26
RInitPos = 26

LStepSize = 0.006734
RStepSize = LStepSize

imageX = 4
imageY = 4
penWidth = 0.1

imPixX = 0
imPixY = 0

#utility globals
global outA
outA = -1

def convertXY(L, R):
	W = width
	X = (W*W + L*L - R*R)/(2*W)
	# print(str(L) + ' ' + str(X))
	try: 
		Y = m.sqrt(L*L -X*X)
	except:
		Y = 0
	return X, Y

def convertLR(X, Y):
	W = width
	L = m.sqrt(X*X + Y*Y)
	R = m.sqrt((W-X)*(W-X) + Y*Y)
	return L, R

def getDist(x1,y1,x2,y2):
	out = m.sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
	return out

def heronsFormula(hyp, A, B):
	h = (1/(hyp*2))*m.sqrt((hyp+A+B)*(-hyp+A+B)*(hyp-A+B)*(hyp+A-B))
	return h

def findTriangleAltitude(X, Y, x1, y1, x2, y2):
	hyp = getDist(x1,y1,x2,y2)
	A = getDist(X,Y,x1,y1)
	B = getDist(X,Y,x2,y2)
	out = heronsFormula(hyp,A,B)
	return out






# cases
# ^^ 0
# vv 1
# ^v 2
# v^ 3
# ^- 4
# v- 5
# -^ 6
# -v 7

possibleMoves = [[-1,-1], [1,1], [-1,1], [1,-1], [-1,0], [1,0], [0,-1],[0, 1]]


def moveTo(lPos, rPos, xGoal, yGoal):
	lGoal, rGoal = convertLR(xGoal, yGoal)
	global outA

	while True:
		# print(convertXY(lPos, rPos))

		lMove = 0
		if(lGoal < lPos):
			lMove = -1
		if(lGoal > lPos):
			lMove = 1

		rMove = 0
		if(rGoal < rPos):
			rMove = -1
		if(rGoal > rPos):
			rMove = 1


		minDir = 0
		for i in range(8):
			if(possibleMoves[i][0] == lMove and possibleMoves[i][1] == rMove):
				minDir = i

		lPos += lMove * LStepSize
		rPos += rMove * RStepSize

		if outA == -1:
			outA = minDir
		else:
			stepOutput.write(str(minDir + outA*16) + '\n')

			if abs(lPos - lGoal) + abs(rPos - rGoal) < LStepSize * 5 : 
				break
			
			outA = -1

	return lPos, rPos

def moveStep(dir, pen, lPos, rPos):
		
	# print(str(dir) + ' ' + str(pen))
	outByte = dir + pen*8
	global outA
	if outA == -1:
		outA = outByte
	else:
		stepOutput.write(str(outByte + outA*16) + '\n') 
		outA = -1

	L = lPos + possibleMoves[dir][0] * LStepSize
	R = rPos + possibleMoves[dir][1] * RStepSize
	return(L,R)


def moveLine(dir, row, lPos, rPos, inPix):
	xPos, yPos = convertXY(lPos, rPos)

	xPixel = int(imPixX*(xPos-xInitPos)/imageX)
	yPixel = int(imPixY*(yPos-yInitPos)/imageY)
	pixVal = getPix(inPix, xPixel, yPixel)

	pen = 0
	# if(pixVal > )
	if pixVal > 255 * ((row%5 /5)*0.8 +1):
		pen = 1

	outByte = dir + 8*pen

	global outA
	if outA == -1:
		outA = outByte
	else:
		stepOutput.write(str(outByte + outA*16) + '\n') 
		outA = -1

	L = lPos + possibleMoves[dir][0] * LStepSize
	R = rPos + possibleMoves[dir][1] * RStepSize
	return(L,R)


def getPix(image, x, y):
	var = 0
	try:
		var = image[x,y]
	except:
		var = 0

	return var





#Start applying functions

#Get file open and load pixels
try:
	inputFile = "Mountains.png"#sys.argv[1]
	inImage = Image.open(inputFile).convert('L')
	inPix = inImage.load()
	imPixY,imPixX = inImage.size

except IOError:
	print("Unable to load image")
	sys.exit(1)


stepOutput = open("steps.txt", "w")

row = 0
lPos = LInitPos
rPos = RInitPos
xInitPos, yInitPos = convertXY(LInitPos, RInitPos)

lPos, rPos = moveTo(lPos, rPos, xInitPos - imageX/2, yInitPos - imageY/2)

LInitPos, RInitPos = lPos, rPos #reset init to new positions at top left
xInitPos, yInitPos = convertXY(LInitPos, RInitPos)

lMax, trash = convertLR(xInitPos + imageX, yInitPos + imageY)

test = [0,0,0,0]

while lPos < lMax:
	#move left to right
	while True:
		row += 1
		test[0] += 1
		xPos, yPos = convertXY(lPos, rPos)
		if xPos > imageX + xInitPos or yPos < yInitPos:
			break
		lPos, rPos = moveLine(6, row, lPos, rPos, inPix)

	#expand radius
	for i in range(int(penWidth/LStepSize)):
		test[1] += 1
		lPos, rPos = moveStep(5, 0, lPos, rPos)


	#move right to left
	while True: 
		row += 1
		test[2] += 1
		xPos, yPos = convertXY(lPos, rPos)
		if xPos < xInitPos or yPos > imageY + yInitPos:
			break
		lPos, rPos = moveLine(7, row, lPos, rPos, inPix)

	#expand radius
	for i in range(int(penWidth/LStepSize)):
		test[3] += 1
		lPos, rPos = moveStep(5, 0, lPos, rPos)

# lPos, rPos = moveTo(lPos, rPos, 20, 20)
print(test)

stepOutput.close()
