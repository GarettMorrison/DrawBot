import math as m
import ezdxf

#Make output DXF
outDXF = ezdxf.new(setup=True)
omsp = outDXF.modelspace()
outDXF.layers.new(name='Skip', dxfattribs={'linetype': 'CONTINUOUS', 'color': 1})
outDXF.layers.new(name='Draw', dxfattribs={'linetype': 'CONTINUOUS', 'color': 2})



#Unit = 1/10"




# w0 = base/2
# h0 = m.sqrt(pow(lInit,2) - pow(w0,2))

# # print(str(w0) + " " + str(h0))
# # print("---")
# def getDist(inVals):
# 	netSqr = 0
# 	for i in inVals:
# 		netSqr += pow(i, 2)
# 	net = m.sqrt(netSqr)
# 	return net


# def saveG(X, Y):
# 	w = X + w0
# 	h = Y + h0
# 	Lf0 = getDist((w,h)) -lInit
# 	Rf0 = getDist(((base-w,h))) -lInit
# 	string = "mr " + str(int(Lf0 * stepPerUnit)) + " " + str(int(Rf0 * stepPerUnit)) + "\n"
# 	file.write(string)
# 	return

def saveP(pos, lastPos):
	omsp.add_line(lastPos, pos, dxfattribs = {'layer': 'draw'})

#Move init
rad = 20
pos = (rad, 0)
lastPos = pos
startPos = pos

i = 1
while True:
	ang = 0.82*i*m.pi
	pos = (rad*m.cos(ang), rad*m.sin(ang))
	saveP(pos, lastPos)
	lastPos = pos

	if abs(pos[0] -startPos[0]) < 0.0001 and abs(pos[1] -startPos[1]) < 0.0001:
		break
	i += 1

outDXF.saveas("data/spirog.dxf")