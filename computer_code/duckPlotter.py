#!/usr/bin/python3
import serial
import json
import sys
import time
from gCodeParser import parse, findLastPosition
from tqdm import tqdm
import argparse

baudRate = 9600


def sendData(ser, obj):
    data = json.dumps(obj) + "\n"
    for d in data:
        ser.write(str(d).encode())

    tqdm.write("Sent: " + data)


def readData(ser):
    return str(ser.readline())


def main():
    parser = argparse.ArgumentParser(description='DuckPlotter pc program. Send g-code instructions to Arduino.')
    
    parser.add_argument('file_name', help='g-code file to plot')
    parser.add_argument('serial_port', help='serial port of Arduino')
    parser.add_argument('--continue', action='store', dest='prev_file_name', help='previous g-code file')
    
    args = parser.parse_args()

    gcode_file = open(args.file_name, 'r')
    gcode = gcode_file.read()
    data = parse(gcode)

    ser = serial.Serial(args.serial_port, baudRate, timeout=None)

    time.sleep(2) # wait for Arduino

    reset = [{
        'type': 'reset'
    }]

    sync = [{
        'type': 'sync',
        'x': 0,
        'y': 0
    }]

    if args.prev_file_name is None:
        data = reset + data
    else:
        prev_file = open(args.prev_file_name, 'r')
        lines = prev_file.readlines()
        x, y = findLastPosition(lines)
        sync[0]['x'], sync[0]['y'] = x, y
        data = sync + data
        print("Resuming from: x = " + str(x) + ", y = " + str(y))

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
