#!/usr/bin/env python

#openCV: Hello world ouverture de video 
#https://docs.opencv.org/4.x/dd/d43/tutorial_py_video_display.html


import cv2 as cv



cap = cv.VideoCapture('walkVauvert.mp4')

#calcul du temps (ms) à attendre entre chaque affichage de frame (cv.waitKey)
fps=cap.get(cv.CAP_PROP_FPS) #https://docs.opencv.org/4.x/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d
wait_time=round(1000/fps)


while cap.isOpened():
    ret, frame = cap.read()
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
        
    count=cv.CAP_PROP_FRAME_COUNT
    print("CAP_PROP_=", count)
    

    
    cv.imshow('frame', frame)
    if cv.waitKey(wait_time) == ord('q'):
        break
        
        
cap.release()
cv.destroyAllWindows()
