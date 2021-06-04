import math as m
import sys
import os
import ezdxf

def getDist(inVals):
	netSqr = 0
	for i in inVals:
		netSqr += pow(i, 2)
	net = m.sqrt(netSqr)
	return net

def ptDist(pt1, pt2):
	return m.sqrt(pow(pt1[0] -pt2[0], 2) + pow(pt1[1] -pt2[1], 2))

def savePt(pt, move = False):
	global file
	global scaleFactor

	x = int((pt[0] -xMid)*scaleFactor)
	y = int((pt[1] -yMid)*scaleFactor)

	#Catch failures
	if x > 32000:
		x = 32000
		print("x over " + str(x) + " | " + str(pt[0]))
	elif x < -32000:
		x = -32000
		print("x under " + str(x) + " | " + str(pt[0]))

	if y > 32000:
		y = 32000
		print("y over " + str(y) + " | " + str(pt[1]))
	elif y < -32000:
		y = -32000
		print("y under " + str(y) + " | " + str(pt[1]))



	if move:
		string = "m " + str(x) + " " + str(y) + "\n"
	else:
		string = "l " + str(x) + " " + str(y) + "\n"
	file.write(string)
	return

#Find any dxf files in directory
selPath = "None"
filePaths = ()
for fileName in os.listdir(os.getcwd()) + ["data/" + strVal for strVal in (os.listdir(os.getcwd() + "/data"))]:
	# print(file)
	if fileName.endswith(".dxf"):
		filePaths += (fileName,)

if len(filePaths) == 0:
	print("No DXFs Found")
	sys.exit()
elif len(filePaths) == 1:
	selPath = filePaths[0]
else:
	print("Multiple DXFs Found. Please select one.")
	for i in range(1, 1+len(filePaths)):
		print(str(i).ljust(3, " ") + filePaths[i -1])
	index = int(input("Select Index:"))
	selPath = filePaths[index -1]

if selPath == "None":
	sys.exit()

print("Selected:" + selPath)

#Get title
titleBreakdown = []
for i in selPath.split("."): 
	for j in i.split("/"):
		titleBreakdown += [j]
		
title = titleBreakdown[-2]
file = open("out/" + title + ".txt", "w")

file.write("x settings.txt\n")

#Make demo output DXF
demoDxf = ezdxf.new(setup=True)
omsp = demoDxf.modelspace()
demoDxf.layers.new(name='Skip', dxfattribs={'linetype': 'CONTINUOUS', 'color': 1})
demoDxf.layers.new(name='Draw', dxfattribs={'linetype': 'CONTINUOUS', 'color': 2})

#Open input dxf
inDxf = ezdxf.readfile(selPath)
msp = inDxf.modelspace()

#Open output file
lines = []
for l in msp.query('LINE'):
	pt1 = (l.dxf.start[0], l.dxf.start[1])
	pt2 = (l.dxf.end[0]  , l.dxf.end[1])
	lines.append((pt1, pt2))
	# print((pt1,pt2))

# print(lines)


#Get starting vals
# bestLn = 0
# bestPt = 0
# bestDist = ptDist(lines[bestLn][bestPt], (0,0))

xDims = [lines[0][0][0]]*2
yDims = [lines[0][0][1]]*2

for i in range(len(lines)):
	for j in range(len(lines[0])):
		x = lines[i][j][0]
		y = lines[i][j][1]

		if x < xDims[0]:
			xDims[0] = x
		if x > xDims[1]:
			xDims[1] = x

		if y < yDims[0]:
			yDims[0] = y
		if y > yDims[1]:
			yDims[1] = y

scaleFactor = 0

xDiff = (xDims[1] -xDims[0]) * 1.25
yDiff = (yDims[1] -yDims[0]) * 1.25
if xDiff > yDiff:
	scaleFactor = 64000/xDiff
else:
	scaleFactor = 64000/yDiff

xMid = xDims[0] +xDiff/2
yMid = yDims[0] +yDiff/2

print("xDiff: " + str(xDiff))
print("yDiff: " + str(yDiff))
print("xMid: " + str(xMid))
print("yMid: " + str(yMid))
print("xMin: " + str(xDims[0]) + " xMax: " + str(xDims[1]))
print("yMin: " + str(yDims[0]) + " yMax: " + str(yDims[1]))
print("scaleFactor: " + str(scaleFactor))

pos = (0,0)
posLast = (xMid,yMid)
penCapOn = True
savePt(pos, move = True) #Move to 0,0 which is start
file.write("p 1" + '\n') #pen down

#Loop through lines
while len(lines) > 0:
	if(len(lines)%100 == 0):
		print(str(len(lines)) + " lines remaining")

	bailLoop = False
	for i in range(len(lines)):
		for j in range(2):
			if lines[i][j] == pos:
				bailLoop = True

				#Calc Line
				posLast = pos
				pos = lines[i][abs(j-1)]
				lines.pop(i)
				#Do line
				savePt(pos)
				omsp.add_line(posLast, pos, dxfattribs = {'layer': 'draw'})

			if bailLoop: break
		if bailLoop: break
	if bailLoop: continue


	#If cant draw immediate line, move to closest segment
	#Find closest point
	bestDist = ptDist(lines[0][0], pos)
	bestPosNext = lines[0][0]
	bestPosIndex = 0,0
	for i in range(len(lines)):
		for j in range(len(lines[0])):
			dist = ptDist(lines[i][j], pos)
			if dist < bestDist:
				bestDist = dist
				bestPosNext = lines[i][j]
				bestPosIndex = i,j

	#Move to point without drawing
	file.write("p 0" + '\n') #pen up
	posLast = pos
	pos = bestPosNext
	savePt(pos, move = True)
	omsp.add_line(posLast, pos, dxfattribs = {'layer': 'skip'})
	file.write("p 1" + '\n') #pen down

	#Draw Line
	posLast = pos
	pos = lines[bestPosIndex[0]][abs(bestPosIndex[1]-1)]
	lines.pop(bestPosIndex[0])
	#Do line
	savePt(pos)
	omsp.add_line(posLast, pos, dxfattribs = {'layer': 'draw'})


#Go to Mid
file.write("p 0" + '\n') #pen up
savePt((xMid, yMid), move = True) #Middle Point


#All place
file.write("d Done with drawfile")

file.close()
demoDxf.saveas("out/" + title + "_draw.dxf")