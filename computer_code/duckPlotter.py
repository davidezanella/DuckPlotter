#!/usr/bin/python3
import serial
import json
import sys
import time
from gCodeParser import parse

serialDev = '/dev/tty20'
baudRate = 9600


def sendData(ser, obj):
    data = json.dumps(obj) + "\n"
    for d in data:
        ser.write(str(d))

    print("Sent:" + data)


def readData(ser):
    return ser.readline()


def main():
    if len(sys.argv) > 1:
        gcode_file = sys.argv[1]
    else:
        gcode_file = input("Name of the gcode file: ")
    if len(sys.argv) > 2:
        serialDev = sys.argv[2]
    else:
        serialDev = input("Port connected with arduino: ")

    gcode_file = open(gcode_file, 'r')
    gcode = gcode_file.read()
    data = parse(gcode)

    ser = serial.Serial(serialDev, baudRate, timeout=1)

    for instr in data:
        sendData(ser, instr)
        time.sleep(0.5)
        print("Read: " + readData(ser))

    ser.close()


if __name__ == "__main__":
    main()
