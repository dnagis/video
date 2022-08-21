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
        
    pos_frames=int(cap.get(cv.CAP_PROP_POS_FRAMES))
    time=int(cap.get(cv.CAP_PROP_POS_MSEC)) 
    	
    
    
    #https://stackoverflow.com/questions/54400034/what-does-cv2s-detectmultiscale-method-return
    #dit que ce qui est retourné c'est une liste de rectangles
    found, _w = hog.detectMultiScale(frame, winStride=(8,8), padding=(32,32), scale=1.05)
    print('frame: ', pos_frames, ' time: ', time ,' rectangles n=', len(found), ' contenu:', str(found))
        
        
cap.release()
cv.destroyAllWindows()
