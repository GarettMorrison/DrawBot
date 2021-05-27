file = open("data/spiral.txt", "w")

rMod = 35
radius = 0

def saveM(L, R):
	string = "m " + str(L) + " " + str(R) + "\n"
	file.write(string)
	return

for i in range(250):
	saveM(0, radius)
	radius += rMod
	saveM(radius, 0)
	radius += rMod
	saveM(0, -1*radius)
	radius += rMod
	saveM(-1*radius, 0)
	radius += rMod

file.close()