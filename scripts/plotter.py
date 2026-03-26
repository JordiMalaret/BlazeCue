import serial
from collections import deque
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time



PORT = "/dev/cu.usbserial-0001"
BAUD = 9600
SAMPLE_RATE = 50


ser = serial.Serial(PORT, BAUD, timeout=1)
# time.sleep(2)                 
ser.reset_input_buffer()   
start_time = time.time()
count = 0
while(time.time()-start_time < 1):
    if ser.readline():
        count += 1
        print(count)
SAMPLE_RATE = count

# ser.readline()
max_points = 200
dt = 1/SAMPLE_RATE  # ~20 ms per sample

data = deque([0] * max_points, maxlen=max_points)
data2 = deque([0] * max_points, maxlen=max_points)
time = [i * dt for i in range(max_points)]

fig, ax = plt.subplots()
line, = ax.plot(time, list(data))
line2, = ax.plot(time, list(data2), label="Signal 2")

ax.set_ylim(-100, 100)
ax.set_xlim(0, max_points * dt)
ax.set_xlabel("Time (s)")
ax.set_ylabel("Value")
ax.set_title(f"PPG Sensor Data: T0 = {SAMPLE_RATE}/s")
ax.grid()

def update(frame):
    try:
        raw = ser.readline().decode("utf-8", errors="ignore").strip()
        if raw:
            v1_str, v2_str = raw.split(",")
            v1 = float(v1_str)
            v2 = float(v2_str)
            data.append(v1)
            data2.append(v2)
            all_vals = list(data) + list(data2)
            line.set_ydata(list(data))
            line2.set_ydata(list(data2))
            ax.set_ylim(min(all_vals) - 10, max(all_vals) + 10)
    
    except:
        pass

    return line, line2

ani = animation.FuncAnimation(fig, update, interval=20, blit=False)
plt.show()
ser.close()