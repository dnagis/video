#!/usr/bin/env python

#Comprendre Mat

#export OPENCV_SAMPLES_DATA_PATH=/initrd/mnt/dev_save/packages/opencv-4.6.0/

import cv2 as cv
import sys

#FILE = 'digits.png'
#img = cv.imread(cv.samples.findFile(FILE))

img = cv.imread(sys.argv[1])

if img is None:
    sys.exit("Could not read the image.")
    



