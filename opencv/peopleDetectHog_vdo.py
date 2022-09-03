#!/usr/bin/python

#Lecture video avec affichage frame par frame (mon_repo: helloVideo.py)
#HOGDescriptor_getDefaultPeopleDetector detectMultiScale sur chaque frame
#draw rectangles sur les frames




import sys
import cv2 as cv


def draw_detections(frame, rects, thickness = 1):
    for x, y, w, h in rects:
        # HOG detector returns slightly larger rectangles than the real objects, so we shrink 
        pad_w, pad_h = int(0.15*w), int(0.05*h)
        cv.rectangle(frame, (x+pad_w, y+pad_h), (x+w-pad_w, y+h-pad_h), (0, 255, 0), thickness)



#./peopleDetectHog_vdo.py capture.mp4
cap = cv.VideoCapture(sys.argv[1])

#initialisation HOG
hog = cv.HOGDescriptor()
hog.setSVMDetector( cv.HOGDescriptor_getDefaultPeopleDetector() )

while cap.isOpened():
    ret, frame = cap.read()

    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break

    found, _w = hog.detectMultiScale(frame, winStride=(8,8), padding=(32,32), scale=1.05)
    draw_detections(frame, found)


    ## [display_frame_number]
    pos_frames=str(int(cap.get(cv.CAP_PROP_POS_FRAMES)))   
    cv.rectangle(frame, (10, 2), (100,20), (255,255,255), -1)
    cv.putText(frame, pos_frames, (15, 15), cv.FONT_HERSHEY_SIMPLEX, 0.5 , (0,0,0))


    ## [affichage frame avec positionnement]    
    cv.namedWindow('ma_window', cv.WINDOW_AUTOSIZE)
    cv.imshow('ma_window', frame)
    cv.moveWindow("ma_window", 100, 100)
    cv.setWindowProperty("ma_window", cv.WND_PROP_TOPMOST, 1)
 
    	
    if cv.waitKey(10) == ord('q'):
        break
        
        
cap.release()
cv.destroyAllWindows()
