#!/usr/bin/env python

#Comprendre Mat


#export OPENCV_SAMPLES_DATA_PATH=/initrd/mnt/dev_save/packages/opencv-4.6.0/

import cv2 as cv
import sys

#FILE = 'digits.png'
#img = cv.imread(cv.samples.findFile(FILE))

#https://docs.opencv.org/4.6.0/d4/da8/group__imgcodecs.html#ga288b8b3da0892bd651fce07b3bbd3a56
img = cv.imread(sys.argv[1], cv.IMREAD_COLOR)


if img is None:
    sys.exit("Could not read the image.")
    
#Pour récupérer les infos en Python, il semblerait qu'il soit nécessaire de considérer Mat
#comme un numpy.ndarray, et récupérer les infos à partir de son shape    
 
print('Image Dimension    : ',img.shape)
print('Image Rows       : ',img.shape[0])
print('Image Cols        : ',img.shape[1])
print('Number of Channels : ',img.shape[2])

print("type of data contained = ", img.dtype)

#Accès à des valeurs individuelles
#https://docs.opencv.org/4.6.0/d5/d98/tutorial_mat_operations.html
value1 = img[0,0,0]
value2 = img[0,0,1]
print("une value 1 = ", value1)
print("une value 2 = ", value2)
