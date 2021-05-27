import serial
import sys


def bigLoop(port, inStr, nestCount):
	indt = "" #Set indent
	for i in range(nestCount): indt += "   "

	if inStr != "":
		try:
			fileIn = open(inStr, "r")
		except:
			print(indt + "File Not Found")
			return(0)

	while True:
		

		if inStr != "":
			command = fileIn.readline()
			if command == "":
				print(indt + "File Finished!")
				break
		else:
			command = input("Input command: ") # "m -99 500" #

		command = command.split()

		sendBytes = (b'\x00') #Default val is 0



		#Skip comments
		if command[0] in ("#"):
			continue


		#moveRad Command
		elif command[0] in ("moveRad", "mr"):	
			if len(command) < 3:
				print(indt + "Too few inputs")
				continue
			comm = (1).to_bytes(1, "big") # val is 1
			L = (int(command[1]) +32767).to_bytes(2, "big") #Get l position target
			R = (int(command[2]) +32767).to_bytes(2, "big") #Get l position target


			print(indt + "Moving to L:" + command[1] + " R:" + command[2])

			sendBytes = comm + L + R

		#Adjust Command
		elif command[0] in ("adjustRad", "ar"):
			if len(command) < 3:
				print(indt + "Too few inputs")
				continue
			comm = (2).to_bytes(1, "big") #Val is 2
			L = (int(command[1]) +32767).to_bytes(2, "big") #Get l position target
			R = (int(command[2]) +32767).to_bytes(2, "big") #Get l position target


			print(indt + "Adjusting by L:" + command[1] + " R:" + command[2])

			sendBytes = comm + R + L

		#Flash command
		elif command[0] in ("flash", "f"):
			if len(command) < 2:
				print(indt + "Too few inputs")
				continue
			comm = (3).to_bytes(1, "big") #Val is 3
			count = int(command[1]).to_bytes(1, "big")
			print(indt + "Flashing LED " + command[1] + " times")

			sendBytes = comm + count

		#Pen command 5
		elif command[0] in ("pen", "p"):
			if len(command) < 1:
				print(indt + "Too few inputs")
				continue
			comm = (5).to_bytes(1, "big") #Val is 3
			var = int(command[1]).to_bytes(1, "big")

			outStr = "Setting Pen to " + str(var)

			print(indt + outStr)

			sendBytes = comm + var

		#Wait command 6
		elif command[0] in ("wait", "w"):
			outStr = ""
			for i in command[1:]:
				outStr += i + " "

			print(indt + outStr)
			input(indt + "Continue")
			continue

		#move 7
		elif command[0] in ("move", "m"):	
			if len(command) < 3:
				print(indt + "Too few inputs")
				continue
			comm = (7).to_bytes(1, "big") # val is 1
			X = (int(command[1]) +32767).to_bytes(2, "big") #Get position target
			Y = (int(command[2]) +32767).to_bytes(2, "big") #Get position target

			print(indt + "Moving to X:" + command[1] + " Y:" + command[2])

			sendBytes = comm + X + Y


		#adjust 8
		elif command[0] in ("adjust", "a"):
			if len(command) < 3:
				print(indt + "Too few inputs")
				continue
			comm = (8).to_bytes(1, "big") #Val is 2
			X = (int(command[1]) +32767).to_bytes(2, "big") #Get position target
			Y = (int(command[2]) +32767).to_bytes(2, "big") #Get position target

			print(indt + "Adjusting by X:" + command[1] + " Y:" + command[2])

			sendBytes = comm + X + Y


		#setVal 9
		elif command[0] in ("setVal", "s"):
			if len(command) < 3:
				print(indt + "Too few inputs")
				continue
			comm = (9).to_bytes(1, "big") #Val is 2
			var = (int(command[1])).to_bytes(1, "big") #Get var to change
			val = (int(command[2]) +32767).to_bytes(2, "big") #Get value to set var to

			print(indt + "Setting " + command[1] + " to " + command[2])

			sendBytes = comm + var + val


		#line 10
		elif command[0] in ("line", "l"):	
			if len(command) < 3:
				print(indt + "Too few inputs")
				continue
			comm = (10).to_bytes(1, "big") # val is 1
			X = (int(command[1]) +32767).to_bytes(2, "big") #Get position target
			Y = (int(command[2]) +32767).to_bytes(2, "big") #Get position target

			print(indt + "Drawing line to to X:" + command[1] + " Y:" + command[2])

			sendBytes = comm + X + Y


		#execute command 11
		elif command[0] in ("execute", "x"):
			if len(command) < 1:
				print(indt + "Too few inputs")
				continue
			fileStr = command[1]
			bigLoop(port, fileStr, nestCount+1)
			continue

		#Break command 12
		elif command[0] in ("break", "b"):
			print(indt + "Exiting!")
			fileIn.close()
			sys.exit()

		#Display command 13
		elif command[0] in ("display", "d"):
			outStr = ""
			for i in command[1:]:
				outStr += i + " "
			print(indt + outStr)
			continue

		#Ignore comment
		elif command[0] == ("#"):
			continue

		else:
			print(indt + "Unknown Command")
			continue

		port.write(sendBytes)

		returnCode = int.from_bytes(port.read(1), "big")

		print(indt + "Returned: " + str(returnCode))



#Basically just call loop

initPort = serial.Serial('/dev/ttyS16', baudrate=9600, timeout= None)

initStr = ""
if len(sys.argv) > 1:
	initStr = sys.argv[1]

bigLoop(initPort, initStr, 0)