#!/usr/bin/env python


#export OPENCV_SAMPLES_DATA_PATH=/initrd/mnt/dev_save/packages/opencv-4.6.0/

import cv2 as cv
import sys

FILE = 'stuff.jpg'

img = cv.imread(cv.samples.findFile(FILE))

if img is None:
    sys.exit("Could not read the image.")
    
    
cv.imshow("Display window", img)
k = cv.waitKey(0)


