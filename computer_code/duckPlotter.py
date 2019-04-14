#!/usr/bin/python3
import serial
import json
import time

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
    data = {
        "type": "move"
    }

    for i in range(5):
        sendData(data)
        time.sleep(0.5)
        print("Read: " + readData())
        

if __name__ == "__main__":
    main()
                            
    ser.close()