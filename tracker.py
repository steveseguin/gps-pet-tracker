import MySQLdb
import sys
import os
import serial
from datetime import datetime

try:
	con = MySQLdb.connect('rdsnext72.c6npskow0rt3.us-east-1.rds.amazonaws.com', 'next72', 'dfdg25125g4ghs', 'tracker')
	cur = con.cursor()
except _mysql.Error, e:
	print "Error %d: %s" % (e.args[0], e.args[1])
	sys.exit(1)

try:
	print "Trying... serial port..."
	tracker = serial.Serial('COM3', 1200, timeout = 2) 
except: 
	print "Failed to connect" 
	sys.exit(1)

line = ""
lat = ""
lng = ""
age = 0
timelast = datetime.now()

print "Listening:"	
while True:
	try:
		for b in tracker.read():
			timelast = datetime.now()
			if (b == ':'):
				if len(line)>0: ## clear old saved data for new data
					print("Fragmented Data: " + line)
					pieces = line.split(",")
					if len(pieces)==2:
						lat = pieces[0]
						lng = pieces[1]
						pieces = lng.split("#")
						if len(pieces)==2:
							lng = pieces[0]
							age = pieces[1]
				
					cur.execute("INSERT INTO evarate (raw,lat,lng,age) VALUES (%s,%s,%s,%s)", (line,lat,lng,age))
					con.commit() #commit the insert
					
					line = ""
					lat = ""
					lng = ""
					age = 0
			else:
				line+=b

		tdelta = datetime.now() - timelast
		tdelta = tdelta.total_seconds()
		if ((tdelta>3) & (len(line)>0)):
			timelast = datetime.now()
			print("")
			print("Recieved Data: " + line)
			pieces = line.split(",")
			if len(pieces)==2:
				lat = pieces[0]
				lng = pieces[1]
				pieces = lng.split("#")
				if len(pieces)==2:
					lng = pieces[0]
					age = pieces[1]
		
			cur.execute("INSERT INTO evarate (raw,lat,lng,age) VALUES (%s,%s,%s,%s)", (line,lat,lng,age))
			con.commit() #commit the insert
			
			line = ""
			lat = ""
			lng = ""
			age = 0
		else:
			print ".",
	except:
		print "Failed"
