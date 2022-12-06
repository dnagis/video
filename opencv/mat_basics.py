#!/usr/bin/env python

#Comprendre Mat


#export OPENCV_SAMPLES_DATA_PATH=/initrd/mnt/dev_save/packages/opencv-4.6.0/

import cv2 as cv
import sys

#FILE = 'digits.png'
#img = cv.imread(cv.samples.findFile(FILE))

img = cv.imread(sys.argv[1], IMREAD_COLOR)

if img is None:
    sys.exit("Could not read the image.")

#Accès aux valeurs individuelles
#https://docs.opencv.org/4.6.0/d5/d98/tutorial_mat_operations.html
value1 = img[0,0,0]
value2 = img[0,0,1]
print("value1 = ", value1)
print("value2 = ", value2)
