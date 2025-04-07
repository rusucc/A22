from matplotlib import pyplot as plt
import matplotlib.animation as animation
import numpy as np
import csv
import serial

com_no = int(input("COM NR:"))
ser = serial.Serial(port = 'COM'+str(com_no), baudrate=38400)

csvfile=open('data.csv','w', newline='')
wr=csv.writer(csvfile)

colors = ['red','green','blue','orange','violet','pink','cyan','brown']
distances = []
angles = []
sensors = [(-45/2,64/2), (-45/2,0), (-45/2,-64/2), 
           (0,64/2), (0,-64/2), 
           (45/2,64/2), (45/2,0), (45/2,-64/2)]

sensors = [(45/2,64/2),(0,64/2),(-45/2,64/2), #dreapta sus, dreapta centru sus, stranga centru sus
           (-45/2,0),
           (-45/2,-64/2),(0,-64/2),(45/2,-64/2),
           (45/2,0)
           ]

fig, (ax,ax2) = plt.subplots(1,2)
ax.set_xlim(-6000,6000)
ax.set_ylim(-6000,6000)
plt.grid()
ax.axis('square')

def draw_arc(x,y,angle1,angle2,radius,color):
    angles = np.linspace(angle1, angle2, 100)
    angles = np.radians(angles)
    xs = radius*np.cos(angles)
    xs = [x+xsi for xsi in xs]
    
    ys = radius*np.sin(angles)
    ys = [y+ysi for ysi in ys]

    line1_x = [x,xs[0]]
    line1_y = [y,ys[0]]
    line2_x = [x,xs[-1]]
    line2_y = [y,ys[-1]]
    ax.plot(line1_x,line1_y,color = color)
    ax.plot(line2_x,line2_y, color = color)
    ax.plot(xs,ys, color = color)

def draw_robot():
    global sensors
    x_coords, y_coords = zip(*sensors)
    ax.scatter(x_coords,y_coords)

def draw_sensors(distances,angles):
    angles_offset = [45,90,135,180,225,270,315,0]
    global colors
    global sensors
        #30,40,50,60 unghiuri
    if len(angles) == 8:
        for i in range(len(sensors)):
            draw_arc(sensors[i][0],sensors[i][1],angles_offset[i]-angles[i]/2,angles_offset[i]+angles[i]/2,radius=distances[i], color = colors[i])

def read_serial():
    data_ser = ser.readline().decode('latin-1').rstrip()
    data_ser_split = data_ser.split(',')
    global wr
    wr.writerow(data_ser_split)

    global distances
    distances = [data_ser_split[i] for i in range(0,len(data_ser_split),2)]
    distances = [int(distance) for distance in distances]
    for i in range(len(distances)):
        if distances[i] >= 5000:
            distances[i] = 0
    print(f"distante:{distances}")

    global angles
    angles =[data_ser_split[i] for i in range(1,len(data_ser_split),2)]
    angles =[int(angle) for angle in angles]
    print(f"unghiuri:{angles}")
    for i in range(len(angles)):
        if angles[i]==1:
            angles[i] = 30
        elif angles[i]==2:
            angles[i] = 40
        elif angles[i] == 3:
            angles[i] = 50
        elif angles[i] == 4:
            angles[i] = 60


def update_plot(i):
    global distances
    global angles
    read_serial()
    ax.cla()
    ax.set_xlim(-3000,3000)
    ax.set_ylim(-3000,3000)
    draw_robot()
    draw_sensors(distances=distances,angles=angles)
    ax2.cla()
    ax2.bar(range(8),distances, color = colors)
    ax2.set_ylim(0,3000)


if __name__ == "__main__":
    
    ani = animation.FuncAnimation(fig, update_plot, interval=50,cache_frame_data=False)
    plt.show()