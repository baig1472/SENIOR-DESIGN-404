from dronekit import connect, VehicleMode, LocationGlobalRelative
import time
from drone_demo import*
#-- Connect to the vehicle
import argparse
# Distance CODE
import RPi.GPIO as GPIO
import time
import signal
import sys

# use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BCM)

# set GPIO Pins
pinTrigger = 18
pinEcho = 24

def close(signal, frame):
    print("\nTurning off ultrasonic distance detection...\n")
    GPIO.cleanup() 
    sys.exit(0)

signal.signal(signal.SIGINT, close)

# set GPIO input and output channels
GPIO.setup(pinTrigger, GPIO.OUT)
GPIO.setup(pinEcho, GPIO.IN)

while True:
    drone()
    # set Trigger to HIGH
    GPIO.output(pinTrigger, True)
    # set Trigger after 0.01ms to LOW
    time.sleep(0.00001)
    GPIO.output(pinTrigger, False)

    startTime = time.time()
    stopTime = time.time()

    # save start time
    while 0 == GPIO.input(pinEcho):
        startTime = time.time()

    # save time of arrival
    while 1 == GPIO.input(pinEcho):
        stopTime = time.time()

    # time difference between start and arrival
    TimeElapsed = stopTime - startTime
    # multiply with the sonic speed (34300 cm/s)
    # and divide by 2, because there and back
    distance = (TimeElapsed * 34300) / 2

    print ("Distance: %.1f cm" % distance)
    #time.sleep(1)
#//////////////////////////////////
    
    if distance >50:
 
        parser = argparse.ArgumentParser(description='commands')
        parser.add_argument('--connect')
        args = parser.parse_args()

        connection_string = "/dev/ttyAMA0"
        baud_rate=57600


        print("Connection to the vehicle on %s"%connection_string)
        vehicle = connect(connection_string,baud=baud_rate, wait_ready=True)

        #-- Define the function for takeoff
        def arm_and_takeoff(tgt_altitude):
            print("Arming motors")
            
            while not vehicle.is_armable:
                time.sleep(1)
                
            vehicle.mode = VehicleMode("GUIDED")
            vehicle.armed = True
            
            while not vehicle.armed: time.sleep(1)
            
            print("Takeoff")
            vehicle.simple_takeoff(tgt_altitude)
            
            #-- wait to reach the target altitude
            while True:
                altitude = vehicle.location.global_relative_frame.alt
                
                if altitude >= tgt_altitude -1:
                    print("Altitude reached")
                    break
                    
                time.sleep(1)
                
                
        #------ MAIN PROGRAM ----
        arm_and_takeoff(10)

        #-- set the default speed
        vehicle.airspeed = 7

        #-- Go to wp1
        print ("go to wp1")



        inhello=open("gps_wp.txt","r")
        list=[]

        read=inhello.readline()
        i=1
        condition=3
        while i !=0:
            while condition > 0:
                list.append(read)
                read=inhello.readline()
                if read == '\n':
                    i=1
                else:
                    i=0
                condition-=1
        inhello.close()
        latitude=float(list[0])
        longitude=float(list[1])
        alt=float(list[2])
        print (latitude)
        print (longitude)
        print (alt)



          

        #wp1 = LocationGlobalRelative(latitude,longitude, alt)

        #vehicle.simple_goto(wp1)

        #--- Here you can do all your magic....
        #time.sleep(30)
#        approach_distance
        #drone()
        
        #--- Coming back
        print("Coming back")
        vehicle.mode = VehicleMode("RTL")

        time.sleep(20)

        #-- Close connection
        vehicle.close()


    #time.sleep(1)


                                 