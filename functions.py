import numpy as np
from PIL import Image, ImageSequence
import cv2
import time


def get_limits(colour):
    c = np.uint8([[colour]])
    hsvC = cv2.cvtColor(c, cv2.COLOR_BGR2HSV)
    lower_limit = hsvC[0][0][0] - 10, 100, 100 #set the lower bound of the hue shift and saturation
    upper_limit = hsvC[0][0][0] + 10, 255, 255 #set the upper bound of the hue shift and saturation
    lower_limit = np.array(lower_limit, dtype=np.uint8)
    upper_limit = np.array(upper_limit, dtype=np.uint8)
    return lower_limit, upper_limit


def colour_map2num(input_colour):
    input_string = ""
    colour2num = {
        0:'w', #white
        1:'g', #green
        2:'r', #red
        3:'b', #blue
        4:'o', #orange
        5:'y'  #yellow
    }
    #map colour number in 2d array to strings
    for i in range(len(input_colour)):
        for j in range(len(input_colour[0])):
            input_string += colour2num[input_colour[i][j]]+' '
        input_string += '\n'
    return input_string


#define colour of the cube
white = [255, 255, 255]
green = [0, 255, 0]
red = [0, 0, 255]
blue = [255, 100, 0]
orange = [0, 128, 255]
yellow = [0, 255, 255]
#get the range of colour to be recognised as a certain colour
lower_white, upper_white = np.array([0, 0, 150], dtype=np.uint8), np.array([179, 55, 255], dtype=np.uint8) #for white, it is meaningless to allow hue shift, so I hard coded this part
lower_green, upper_green = get_limits(green)
lower_red, upper_red = get_limits([110, 90, 255]) #red colour of my cube is not red enough I guess
lower_blue, upper_blue = get_limits(blue)
lower_orange, upper_orange = get_limits(orange)
lower_yellow, upper_yellow = get_limits(yellow)
colour_range = [[white, lower_white, upper_white], [green, lower_green, upper_green], [red, lower_red, upper_red], [blue, lower_blue, upper_blue], [orange, lower_orange, upper_orange], [yellow, lower_yellow, upper_yellow]] #put the colours in an array to be imported to main script


#define coordinate of skewb skeleton in preview window
top_coord = 50
mid_coord = 240
bottom_coord = 430
#define coordinates of different points of the skewb skeleton
skewb_points = [(top_coord, top_coord),
                (mid_coord, top_coord),
                (bottom_coord, top_coord),
                (bottom_coord, mid_coord),
                (bottom_coord, bottom_coord),
                (mid_coord, bottom_coord),
                (top_coord, bottom_coord),
                (top_coord, mid_coord),
                (int((mid_coord+top_coord)/2), int(bottom_coord-(mid_coord-top_coord)/2))
                ]
triangle_topL = np.array([skewb_points[0], skewb_points[1], skewb_points[7]])
triangle_topR = np.array([skewb_points[1], skewb_points[2], skewb_points[3]])
triangle_bottomL = np.array([skewb_points[5], skewb_points[6], skewb_points[7]])
triangle_bottomR = np.array([skewb_points[3], skewb_points[4], skewb_points[5]])
sqare_centre = np.array([skewb_points[1], skewb_points[3], skewb_points[5], skewb_points[7]])
fill_area = [triangle_topL, triangle_topR, triangle_bottomR, triangle_bottomL, sqare_centre] #define area in the skewb skeleton to be filled with colours


def load_gif_frames(gif_path): #load gif and store frames in an array
    gif = Image.open(gif_path)
    frames = []
    # Process all frames
    for frame in ImageSequence.Iterator(gif):
        frame_rgb = frame.convert('RGB') #convert frame to rgb
        frame_array = np.array(frame_rgb)
        frame_bgr = cv2.cvtColor(frame_array, cv2.COLOR_RGB2BGR) #convert from rgb to bgr for opencv
        frames.append(frame_bgr)
    return frames


def get_current_frame(frames, start_time): #calculate which frame to be displayed by the elapsed time
    current_time = time.time() * 1000  #convert to milliseconds
    elapsed = current_time - start_time
    elapsed_reduced = elapsed % 1980 #the gifs are 1980 milliseconds long

    # Find the correct frame for the current time
    for i in range(60):
        if elapsed_reduced < i*33 + 33: #the gifs are 30 fps, 1000/30=33
            return frames[i]
    return frames[0]


def resize_and_crop(frame, target_width=500, target_height=480): #reduce the pixel of the webcam frame, crop the frame from centre to match the target ratio
    height, width = frame.shape[:2] #get original height and width

    width_scaler = target_width/width #calculate scaler
    heigh_scaler = target_height/height

    scaler = max(width_scaler, heigh_scaler) #find the larger scaler

    width = int(width*scaler) #resize the image
    height = int(height*scaler)
    frame = cv2.resize(frame, (width, height))

    start_x = (width-target_width)//2 #calculate coordinates to crop out
    start_y = (height-target_height)//2
    frame = frame[start_y:start_y+target_height, start_x:start_x+target_width] #crop the frame to target size
    return frame
