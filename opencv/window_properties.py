#!/usr/bin/python

#Affichage d'une image avec modification des propriétés de la fenêtre pour
#que la window soit visible!

import cv2 as cv
import sys


#export OPENCV_SAMPLES_DATA_PATH=/initrd/mnt/dev_save/packages/opencv-4.6.0/
img = cv.imread(cv.samples.findFile("starry_night.jpg"))


if img is None:
    sys.exit("Could not read the image.")

#Création manuelle de window pour pouvoir modifier ses properties
cv.namedWindow('ma_window', cv.WINDOW_AUTOSIZE)
cv.imshow("ma_window", img)
cv.moveWindow("ma_window", 100, 100)
cv.setWindowProperty("ma_window", cv.WND_PROP_TOPMOST, 1)


k = cv.waitKey(0) #any key exits





