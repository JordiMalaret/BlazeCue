import serial
import time



PORT = "/dev/cu.usbserial-0001"
BAUD = 9600
SAMPLE_RATE = 0
ser = serial.Serial(PORT, BAUD, timeout=1)

def split_data(ser):
    data = [None]*1024
    count = 0
    while(count < 1024):
        line = ser.readline().decode("utf-8", errors="ignore").strip()
        if line:
            data[count] = line
            count += 1
    return data

while(True): #loop() in cpp
    print(split_data(ser))