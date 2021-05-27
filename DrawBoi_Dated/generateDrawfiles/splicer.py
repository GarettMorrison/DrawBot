import math as m

#definitions
width = 19.75

LInitPos = 18
RInitPos = 22.5

LStepSize = 0.006734
RStepSize = LStepSize

penDown = False;

def convertXY(L, R): #Basic math conversion for coordinate system
	W = width
	X = (W*W + L*L - R*R)/(2*W)
	# print(str(L) + ' ' + str(X))
	try: 
		Y = m.sqrt(L*L -X*X)
	except:
		Y = 0
	return X, Y

def convertLR(X, Y): #Basic math conversion for coordinate system
	W = width
	L = m.sqrt(X*X + Y*Y)
	R = m.sqrt((W-X)*(W-X) + Y*Y)
	return L, R

def getDist(x1,y1,x2,y2): #LIterally just pythagoras lol
	out = m.sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
	return out

def heronsFormula(hyp, A, B): #Heron's formula
	h = (1/(hyp*2))*m.sqrt((hyp+A+B)*(-hyp+A+B)*(hyp-A+B)*(hyp+A-B))
	return h

def findTriangleAltitude(X, Y, x1, y1, x2, y2): #Using heron's for triangle altitude, used to stay on line
	hyp = getDist(x1,y1,x2,y2)
	A = getDist(X,Y,x1,y1)
	B = getDist(X,Y,x2,y2)
	out = heronsFormula(hyp,A,B)
	return out





	
# cases
# ^^ 7
# vv 6
# ^v 5
# v^ 4
# ^- 3
# v- 2
# -^ 1
# -v 0

possibleMoves = [[-1,-1], [1,1], [-1,1], [1,-1], [-1,0], [1,0], [0,-1],[0, 1]] #How each possible move corresponds to left and right moves

def getClosest(lPos, rPos, xInit, yInit, xGoal, yGoal): #Calculate next step for moving on line
	minDir = 0;
	minScore = 0;
	minL = 0;
	minR = 0;

	for i in range(8): #Check each option, attempt to minimize distance
		L = lPos + possibleMoves[i][0] * LStepSize
		R = rPos + possibleMoves[i][1] * RStepSize
		X, Y = convertXY(L,R)



		dist = getDist(X, Y, xGoal, yGoal) #get dist for each option

		alti = findTriangleAltitude(X, Y, xInit, yInit, xGoal, yGoal) #add distance from perfectly straight line to prevent drift

		score = dist + alti*2

		if i == 0: #Autosave first val for comparison
			minScore = score
			minL = L
			minR = R

		elif score < minScore: #Set new best score
			minScore = score
			minDir = i;
			minL = L
			minR = R

	return minDir, minL, minR 


def navigateTo(lPos, rPos, xGoal, yGoal, pen): #Do a bunch of steps moving between each point pair
	xInit, yInit = convertXY(lPos, rPos)

	lGoal, rGoal = convertLR(xGoal, yGoal)

	outA = -1
	while True: #Do steps until we hit endpoint
		print(convertXY(lPos, rPos))
		# xPos, yPos = convertXY(lPos, rPos)
		# gap = m.sqrt(m.pow(xPos-xGoal, 2) + m.pow(yPos-yGoal, 2))
		minDir, lPos, rPos =  getClosest(lPos, rPos, xInit, yInit, xGoal, yGoal)
		minDir += pen * 8

		if outA == -1:
			outA = minDir
		else:
			stepOutput.write(str(minDir + outA*16) + '\n')
			# print(minDir + outA*16)
			if abs(lPos - lGoal) + abs(rPos - rGoal) < LStepSize * 5 : 
				break
			
			outA = -1
	return lPos, rPos



def moveTo(lPos, rPos, xGoal, yGoal): #Move to posistion without pen on, ignores making line straight in exchange for speed
	lGoal, rGoal = convertLR(xGoal, yGoal)

	outA = -1
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
		
		print(str(lMove) + ' ' + str(rMove))

		if outA == -1:
			outA = minDir
		else:
			stepOutput.write(str(minDir + outA*16) + ' ' + str(lMove) + ' ' + str(rMove) + '\n')

			if abs(lPos - lGoal) + abs(rPos - rGoal) < LStepSize * 5 : 
				break
			
			outA = -1


	return lPos, rPos


#Start execution code
stepOutput = open("steps.txt", "w") #open output file, saves steps as bytes seperated by endlines

lPos = LInitPos
rPos = RInitPos
xInitPos, yInitPos = convertXY(LInitPos, RInitPos)

pen = 1

for coordIn in open("inCoords.txt", "r"): #read coordinates to move through
	x,y,penStr = coordIn.split()
	pen = int(penStr)
	xPos = float(x) + xInitPos
	yPos = float(y) + yInitPos

	print(str(convertXY(lPos, rPos)) + " " + str(xPos) + " " + str(yPos) + " " )
	lPos, rPos = navigateTo(lPos, rPos, xPos, yPos, pen)

stepOutput.close()
