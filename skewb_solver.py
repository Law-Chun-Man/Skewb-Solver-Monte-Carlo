import cv2
import subprocess
import numpy as np
import time
from functions import colour_map2num, colour_range, fill_area, skewb_points, load_gif_frames, get_current_frame, resize_and_crop


cap = cv2.VideoCapture(0, cv2.CAP_V4L2) #change the number to load different webcam


#import frames of different gif for telling user how to rotate the cube for next step
frames0 = load_gif_frames('cube_rotation_gif/0.gif')
frames1 = load_gif_frames('cube_rotation_gif/1.gif')
frames2 = load_gif_frames('cube_rotation_gif/2.gif')
frames3 = load_gif_frames('cube_rotation_gif/3.gif')
frames4 = load_gif_frames('cube_rotation_gif/4.gif')
frames5 = load_gif_frames('cube_rotation_gif/5.gif')
frames = [frames0, frames1, frames2, frames3, frames4, frames5] #group the frames together in one array
start_time = time.time() * 1000 #convert to milliseconds
prev_time = time.time()


cv2.namedWindow("frame", cv2.WINDOW_NORMAL)
cv2.resizeWindow("frame", 1920, 1080)
#set webcam properties
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
cap.set(cv2.CAP_PROP_FPS, 30)
input_colour = [] #initialise array for storing colour of different faces of skewb
face_number = 0 #keep track of the number of faces scanned


while True:
    ret, frame = cap.read()
    frame = resize_and_crop(frame) #resize the webcam to 500px wide, and 480px tall
    #frame = cv2.flip(frame, 1)
    hsvImage = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV) #convert to HSV for recognising colour
    #crop out a region of the frame for colour recognisation
    roi = [
        hsvImage[skewb_points[0][0]:skewb_points[-1][0], skewb_points[0][0]:skewb_points[-1][0]],
        hsvImage[skewb_points[0][0]:skewb_points[-1][0], skewb_points[-1][1]:skewb_points[2][0]],
        hsvImage[skewb_points[-1][1]:skewb_points[2][0], skewb_points[-1][1]:skewb_points[2][0]],
        hsvImage[skewb_points[-1][1]:skewb_points[2][0], skewb_points[0][0]:skewb_points[-1][0]],
        hsvImage[skewb_points[-1][0]:skewb_points[-1][1], skewb_points[-1][0]:skewb_points[-1][1]]
    ]


    face_colour = [0, 0, 0, 0, 0] #initialise array to store colour of current face
    for i in range(5): #loop through all the region
        for j in range(6): #loop through all the colours
            mask = cv2.inRange(roi[i], colour_range[j][1], colour_range[j][2]) #loop through all the colour to check the colour of the region
            pixels = cv2.countNonZero(mask)
            if pixels>2000: #if the number of pixel for certain colour is bigger than 2000, save the colour
                face_colour[i] = j
                cv2.fillPoly(frame, [fill_area[i]], colour_range[j][0]) #fill colour on the screen skewb skeleton to notify user that the colour had been scanned


    #draws skeleton of the skewb face for user to fit the skewb in
    cv2.rectangle(frame, skewb_points[0], skewb_points[4], (0, 0, 0), 5)
    vertices = np.array([skewb_points[1], skewb_points[7], skewb_points[5], skewb_points[3]], np.int32)
    vertices = vertices.reshape((-1, 1, 2))
    cv2.polylines(frame, [vertices], isClosed=True, color=(0, 0, 0), thickness=5)


    current_time = time.time()
    prev_time = current_time
    gif_frame = get_current_frame(frames[face_number], start_time) #get the frame from the gif
    combined_frame = np.hstack((frame, gif_frame)) #show the gif on the right side of the webcam
    cv2.imshow("frame", combined_frame)


    key = cv2.waitKey(1) & 0xFF
    if key == 13 and face_number<5: #press enter to capture colours (13 is enter key)
        input_colour.append(face_colour) #save the colour of current face
        face_number += 1 #record after scanning one face
    elif key == 13 and face_number==5: #press enter again after finishing capturing all sides
        input_colour.append(face_colour) #save the colour of current face
        break


cap.release()
cv2.destroyAllWindows()


input_string = colour_map2num(input_colour) #change the number to word string to be input in C++ programme
process = subprocess.Popen(['./a'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True) #run the C++ programme and input the colour and capture the output
output, error = process.communicate(input=input_string) #print the output from C++ programme
if error:
    print(error)
else:
    print(output)
