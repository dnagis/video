#!/usr/bin/env python

#openCV: Hello world ouverture de video, affichage frames 
#https://docs.opencv.org/4.x/dd/d43/tutorial_py_video_display.html


import cv2 as cv



cap = cv.VideoCapture('lunel_1.mp4')

#calcul du temps (ms) à attendre entre chaque affichage de frame (cv.waitKey)
#Reference de tous les CAP_PROP_*: https://docs.opencv.org/4.x/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d
fps=cap.get(cv.CAP_PROP_FPS) 
wait_time=round(1000/fps)


while cap.isOpened():
    ret, frame = cap.read()
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
        


    
    ## [display_frame_number (trouvé dans tutorial_code/.../bg_sub.py)]
    #get the frame number and write it on the current frame
    pos_frames=str(cap.get(cv.CAP_PROP_POS_FRAMES))
    #print("CAP_PROP_POS_MSEC=", str(cap.get(cv.CAP_PROP_POS_MSEC)))       
    cv.rectangle(frame, (10, 2), (100,20), (255,255,255), -1)
    cv.putText(frame, pos_frames, (15, 15), cv.FONT_HERSHEY_SIMPLEX, 0.5 , (0,0,0))
    ## [display_frame_number]
    

    
    cv.imshow('frame', frame)
    if cv.waitKey(wait_time) == ord('q'):
        break
        
        
cap.release()
cv.destroyAllWindows()
