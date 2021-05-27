import math as m

file = open("data/spiro.txt", "w")

#Unit = 1/10"

lInit = 710
base = 620
stepPerUnit = 52

rad = 20

w0 = base/2
h0 = m.sqrt(pow(lInit,2) - pow(w0,2))

# print(str(w0) + " " + str(h0))
# print("---")
def getDist(inVals):
	netSqr = 0
	for i in inVals:
		netSqr += pow(i, 2)
	net = m.sqrt(netSqr)
	return net


def saveG(X, Y):
	w = X + w0
	h = Y + h0
	Lf0 = getDist((w,h)) -lInit
	Rf0 = getDist(((base-w,h))) -lInit
	string = "mr " + str(int(Lf0 * stepPerUnit)) + " " + str(int(Rf0 * stepPerUnit)) + "\n"
	file.write(string)
	return

#Move init
saveG(rad, 0)
file.write("w Please Remove Cap\n")


for i in range(41):
	ang = 0.95*i*m.pi
	saveG(rad*m.cos(ang), rad*m.sin(ang))

file.write("w Please Replace Cap\n")
saveG(0, 0)
file.write("d Done with Spiro!")

file.close()