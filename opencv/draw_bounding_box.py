#!/usr/bin/env python


import cv2 as cv
import sys

#FILE = 'digits.png'
#img = cv.imread(cv.samples.findFile(FILE))

img = cv.imread(sys.argv[1])

if img is None:
    sys.exit("Could not read the image.")
    
frameHeight = img.shape[0]
frameWidth = img.shape[1]
print('Image Rows (frameHeight): ',frameHeight)
print('Image Cols (frameWidth): ',frameWidth)




#class=0 CI=0.722897 raw box: [0.586982,0.342083,0.230834,0.657476]
#class=0 CI=0.777667 raw box: [0.633663,0.329043,0.188877,0.608022]
#class=0 CI=0.929977 raw box: [0.881314,0.408202,0.0262301,0.0732234]

output=[0.881314,0.408202,0.0262301,0.0732234]

#print('une valeur: ',output[0])

center_x = int(output[0] * frameWidth)
center_y = int(output[1] * frameHeight)
width = int(output[2] * frameWidth)
height = int(output[3] * frameHeight)
left = int(center_x - (width / 2))
top = int(center_y - (height / 2))
right = int(center_x + (width / 2))
bottom = int(center_y + (height / 2))
cv.rectangle(img, (left,top), (right,bottom), (0, 0, 255), 2) 
   
#cv.rectangle(img, (13,16), (77,144), (0, 0, 255), 2) 
   
cv.imshow("Display window", img)

#Sauvegarder l'image
#cv.imwrite('image.jpg', img)

k = cv.waitKey(0)

