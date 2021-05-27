import math as m
import sys
import os
import ezdxf


#Find any dxf files in directory
selPath = "None"
filePaths = ()
for fileName in os.listdir(os.getcwd()) + ["out/" + strVal for strVal in (os.listdir(os.getcwd() + "/out"))]:
	# print(file)
	if fileName.endswith(".txt"):
		filePaths += (fileName,)

if len(filePaths) == 0:
	print("No Files Found")
	sys.exit()
elif len(filePaths) == 1:
	selPath = filePaths[0]
else:
	print("Multiple Files Found. Please select one.")
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






inFile =  open(selPath)
outFile = open("out/" + title + "_mr.txt", "w")

#Unit = 1/10"
lInit = 710
base = 620
stepPerUnit = 52

imgScale = 60

w0 = base/2
h0 = m.sqrt(pow(lInit,2) - pow(w0,2))

def getDist(inVals):
	netSqr = 0
	for i in inVals:
		netSqr += pow(i, 2)
	net = m.sqrt(netSqr)
	return net

def saveG(inVals):
	w = (inVals[0]/32767)*imgScale + w0
	h = (inVals[1]/32767)*imgScale + h0
	Lf0 = getDist((w,h)) -lInit
	Rf0 = getDist(((base-w,h))) -lInit
	string = "mr " + str(int(Lf0 * stepPerUnit)) + " " + str(int(Rf0 * stepPerUnit)) + "\n"
	outFile.write(string)
	return


#Make demo output DXF
demoDxf = ezdxf.new(setup=True)
omsp = demoDxf.modelspace()
demoDxf.layers.new(name='Skip', dxfattribs={'linetype': 'CONTINUOUS', 'color': 1})
demoDxf.layers.new(name='Draw', dxfattribs={'linetype': 'CONTINUOUS', 'color': 2})

pos = (0,0)
lastPos = pos

lastLine = ""
while True:
	line = inFile.readline()

	if line == "":
		break
	if line == lastLine: 
		continue
	else: 
		lastLine = line

	if line[0] == 'l':
		lineParts = line.split(" ")
		lastPos = pos
		pos = (int(lineParts[1]), int(lineParts[2]))
		saveG(pos)
		omsp.add_line(lastPos, pos, dxfattribs = {'layer': 'draw'})
	elif line[0] in ['w', 'd']:
		outFile.write(line)

demoDxf.saveas("out/mr.dxf")

