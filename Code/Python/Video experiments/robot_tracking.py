# Modified ball_tracking example from:
# https://www.pyimagesearch.com/2015/09/21/opencv-track-object-movement/

# import the necessary packages
from __future__ import division
from collections import deque
import numpy as np
import argparse
import imutils
import cv2
import matplotlib.pyplot as plt

# define the lower and upper boundaries of the "green"
# ball in the HSV color space, then initialize the
# list of tracked points
greenLower = (29, 86, 50)
greenUpper = (64, 255, 255)

dir = 'Video Circle/Video_r1_right_r30_battery/'
file = dir + 'DSC_4781.MOV'
line_color = (0, 0, 255)

rect_p1 = (440, 40)
rect_p2 = (1460, 1040)

camera = cv2.VideoCapture(file)

# Get total number of frames for OpenCV 3+
num_frames = int(camera.get(cv2.CAP_PROP_FRAME_COUNT))

pts = deque(maxlen=num_frames)

cnt = 0
# keep looping
while cnt < num_frames:
    # grab the current frame
    (grabbed, frame) = camera.read()

    # resize the frame, blur it, and convert it to the HSV
    # color space
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # construct a mask for the color "green", then perform
    # a series of dilations and erosions to remove any small
    # blobs left in the mask
    mask = cv2.inRange(hsv, greenLower, greenUpper)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    # find contours in the mask and initialize the current
    # (x, y) center of the ball
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                            cv2.CHAIN_APPROX_SIMPLE)[-2]
    center = None

    # only proceed if at least one contour was found
    if len(cnts) > 0:
        # find the largest contour in the mask, then use
        # it to compute the minimum enclosing circle and
        # centroid
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

        # only proceed if the radius meets a minimum size
        if radius > 10:
            # draw the circle and centroid on the frame,
            # then update the list of tracked points
            cv2.circle(frame, (int(x), int(y)), int(radius),
                       (0, 255, 255), 2)
            cv2.circle(frame, center, 5, (0, 0, 255), -1)

    # update the points queue
    pts.appendleft(center)

    # loop over the set of tracked points
    for i in xrange(1, len(pts)):
        # if either of the tracked points are None, ignore
        # them
        if pts[i - 1] is None or pts[i] is None:
            continue

        # otherwise, compute the thickness of the line and
        # draw the connecting lines
        #thickness = int(np.sqrt(buffer / float(i + 1)) * 2.5)
        cv2.line(frame, pts[i - 1], pts[i], line_color, 2)

    """
    # show the frame to our screen
    cv2.imshow("Frame", frame)
    #cv2.imshow('mask', mask)
    key = cv2.waitKey(1) & 0xFF

    # if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        break
    """

    cnt += 1

print "Finished"
cv2.rectangle(frame, rect_p1, rect_p2, (0, 0, 255), 2)
cv2.imshow("Frame", frame)
# wait for any keypress
cv2.waitKey(0)

# cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()

list = list(pts)
clist = [i for i in list if i is not None]

x = rect_p2[0] - rect_p1[0]
y = rect_p2[1] - rect_p1[1]

x2 = (1920 - x)/2
y2 = (1080 - y)/2

# scale circle acording to square dimensions in cm
slist = []
for i in clist:
    x3 = ((i[0] - x2) * (80 / x))
    y3 = ((i[1] - y2) * (80 / y))
    # inverse y while images start in the top left corner instead of bottom left
    y3 = 80 - y3
    slist.append((x3, y3))

print clist
print slist
plt.figure(1)
plt.plot(*zip(*slist))
plt.axes().set_aspect('equal', 'datalim')
plt.show()
