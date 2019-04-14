#!/usr/bin/python3
import serial
import json

serialDev = '/dev/ttyUSB0'
baudRate = 9600

def sendData(obj):
    data = json.dumps(obj)
    ser = serial.Serial(serialDev, baudRate, timeout=1)
    for d in data:
        print (d)
        ser.write(str(d))                            
    ser.close()

def main():
    data = {
        "type": "move"
    }

    sendData(data)

if __name__ == "__main__":
    main()