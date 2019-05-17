See comments within file for added notes.

This was a project designed to track my cat near my house, via collar with gps transmitter.

SIRF III Satellite chip with 1200 BAUD serial output was connected to a Tiny Arduino board,
which took commands and sent position information over a low-cost, long-range (1km) transmitter. 
A coil antenna was used; about an inch long. 

An 800mah lithium battery was used to power the device,
which could power it for days or longer if I kept the GPS into sleep mode most the time.
The battery was able to unclip (attached via a simple 3-prong plug) and it could be charged
nightly as needed.


On the other end, a computer with a Serial to USB adapter 
listened to a receiver that was wired up on a breadboard with a large antenna.
This was fed into a Python script, that calculated range (distance) of the cat,
marked "last known position" if the cat went out of range, logged the data,
and kept a Google maps webpage available to view the cat's movement logs.


Added logic was meant to be added to ardunio, but I ran out of motivation.

- If the batter on the device got low, the GPS could be turned off and the 
device could just repeat the last known location every 5 minutes.

- Adding a motion sensor to the collar could allow me to trigger the
GPS to turn on only when there is movement; if not moving the last
known position could be kept actively being transmitted.

- if the cat is near the house, a low power transmission power ccould be
used, with a less frequency GPS tracking.  If the cat gets farther from the house,
perhaps more frequent tracknig and greater power output.

Final note, at about 500m, the quality of the signal frmo the transmitter
was quite fragile. Buildings, trees, walls caused problems. Despite that,
at range I could still detect "garbled" signals, which could be
used to find a direction up to 1km away, bringing me in range of 500m eventually.

As well, I was thinking of putting up 'solar' repeaters in my area,
to amply and log any passing transmitted data, so I can increase the range
up to a few kilometers with under $100 spent.

Cellular collars might work better, considering the size fo them these days,
but this tracker was well suited for the task at hand.

I bought a serial-out-based 9600 baud camera also with the hope of capturing JPG snapshots and transmitting them back to the computer, but I somehow managed to break the camera's firmware. THe $80 cost to buy a new one wasn't worth it.
