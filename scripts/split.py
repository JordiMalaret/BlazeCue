import serial
from collections import deque
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time



PORT = "/dev/cu.usbserial-0001"
BAUD = 9600
SAMPLE_RATE = 0
ser = serial.Serial(PORT, BAUD, timeout=1)

# time.sleep(2)                 
# ser.reset_input_buffer()   
# start_time = time.time()
# count = 0
# while(time.time()-start_time < 1):
#     if ser.readline():
#         count += 1
#         print(count)
# SAMPLE_RATE = count


def split_data(ser):
    data = [None]*1024
    count = 0
    while(count < 1024):
        line = ser.readline().decode("utf-8", errors="ignore").strip()
        if line:
            data[count] = line
            count += 1
    return data

while(True):
    print(split_data(ser))