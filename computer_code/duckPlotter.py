#!/usr/bin/python3
import serial
import json
import sys
import time
from gCodeParser import parse

serialDev = '/dev/tty20'
baudRate = 9600

ser = serial.Serial(serialDev, baudRate, timeout=1)


def sendData(obj):
    data = json.dumps(obj) + "\n"
    for d in data:
        ser.write(str(d))

    print("Sent:" + data)

def readData():
    return ser.readline()

def main():
    if len(sys.argv) > 1:
        gcode_file = sys.argv[1]
    else:
        gcode_file = input("Name of the gcode file: ")

    gcode_file = open(gcode_file,'r')
    gcode = gcode_file.read()
    data = parse(gcode)

    for instr in data:
        sendData(instr)
        time.sleep(0.5)
        print("Read: " + readData())
        

if __name__ == "__main__":
    main()
                            
    ser.close()