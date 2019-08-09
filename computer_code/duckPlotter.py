#!/usr/bin/python3
import serial
import json
import sys
import time
from gCodeParser import parse
from tqdm import tqdm

baudRate = 9600


def sendData(ser, obj):
    data = json.dumps(obj) + "\n"
    for d in data:
        ser.write(str(d).encode())

    tqdm.write("Sent: " + data)


def readData(ser):
    return str(ser.readline())


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

    ser = serial.Serial(serialDev, baudRate, timeout=None)

    time.sleep(2) # wait for Arduino

    reset = [{
        'type': 'reset'
    }]

    data = reset + data

    for instr in tqdm(data):
        sendData(ser, instr)
        time.sleep(1)
        result = readData(ser)
        tqdm.write("Read: " + result)

        #print debug code from the Arduino
        while "Done" not in result:
            tqdm.write("\t" + str(result))
            result = readData(ser)

    ser.close()


if __name__ == "__main__":
    main()
