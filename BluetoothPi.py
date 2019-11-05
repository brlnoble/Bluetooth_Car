#import evdev
from evdev import InputDevice, categorize, ecodes
from time import sleep
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup([14,15,18,23], GPIO.OUT)

b1 = 14 #servo control
b2 = 15
b3 = 18 #drive control
b4 = 23

#creates object 'gamepad' to store the data
#you can call it whatever you like
gamepad = InputDevice('/dev/input/event1')

#button code variables (change to suit your device)
aBtn = 305
bBtn = 304
xBtn = 307
yBtn = 306

upDown = 17
rightLeft = 16

plus = 313
minus = 312

home = 316
screenshot = 317

lTrig = 310
lBum = 308
rTrig = 311
rBum = 309


#prints out device info at start
print(gamepad)

#loop and filter by event code and print the mapped label
for event in gamepad.read_loop():
    if event.type == ecodes.EV_KEY:
        if event.value == 1:
            if event.code == bBtn:
                GPIO.output(b4, 0)
                GPIO.output(b3, 0)
                print("Motors Off")
            elif event.code == aBtn:
                GPIO.output(b4, 1)
                GPIO.output(b3, 1)
                print("Motors On")
            elif event.code == xBtn:
                GPIO.output([b1,b2,b3,b4], 0)
                print("STOP")

            elif event.code == lTrig:
                GPIO.output(b2, 1)
                GPIO.output(b1, 0)
                print("Left Turn")
            elif event.code == rTrig:
                GPIO.output(b2, 0)
                GPIO.output(b1, 1)
                print("Right Turn")
            elif event.code == plus:
                GPIO.output(b4, 1)
                GPIO.output(b3, 0)
                print("Speed +")
                sleep(0.01)
                GPIO.output(b4, 1)
                GPIO.output(b3, 1)
            elif event.code == minus:
                GPIO.output(b4, 0)
                GPIO.output(b3, 1)
                print("Speed -")
                sleep(0.01)
                GPIO.output(b4, 1)
                GPIO.output(b3, 1)                
        elif event.value == 2:
            if event.code == lTrig:
                GPIO.output(b2, 1)
                GPIO.output(b1, 0)
            elif event.code == rTrig:
                GPIO.output(b2, 0)
                GPIO.output(b1, 1)
        elif event.value == 0:
            if event.code == lTrig:
                GPIO.output([b2,b1], 0)
            elif event.code == rTrig:
                GPIO.output([b2,b1], 0)
