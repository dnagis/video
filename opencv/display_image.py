#!/usr/bin/env python


#export OPENCV_SAMPLES_DATA_PATH=/initrd/mnt/dev_save/packages/opencv-4.6.0/

import cv2 as cv
import sys

#FILE = 'digits.png'
#img = cv.imread(cv.samples.findFile(FILE))

img = cv.imread(sys.argv[1])

if img is None:
    sys.exit("Could not read the image.")
    
#dessiner un rect de 64x128 (pour HOG)    
#cv.rectangle(img, (40,40), (104,168), (0, 0, 255), 2) 
   
cv.imshow("Display window", img)

k = cv.waitKey(0)


