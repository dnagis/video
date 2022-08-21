#!/usr/bin/python

#HOG detection sur chaque frame samples/python/peopledetect.py


import sys
import cv2 as cv



cap = cv.VideoCapture(sys.argv[1])


hog = cv.HOGDescriptor()
hog.setSVMDetector( cv.HOGDescriptor_getDefaultPeopleDetector() )


while cap.isOpened():
    ret, frame = cap.read()
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
        
    pos_frames=str(cap.get(cv.CAP_PROP_POS_FRAMES))
    	
    print(pos_frames)
    found, _w = hog.detectMultiScale(frame, winStride=(8,8), padding=(32,32), scale=1.05)
    print('%d found' % (len(found)))
        
        
cap.release()
cv.destroyAllWindows()
