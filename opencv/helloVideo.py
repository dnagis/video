#!/usr/bin/python

#openCV: Hello world ouverture de video, affichage frames 
#./helloVideo.py /root/out.mp4
#https://docs.opencv.org/4.x/dd/d43/tutorial_py_video_display.html

import sys
import cv2 as cv



cap = cv.VideoCapture(sys.argv[1])

#calcul du temps (ms) à attendre entre chaque affichage de frame (cv.waitKey)
#Reference de tous les CAP_PROP_*: https://docs.opencv.org/4.x/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d
#visiblement marche pas à chaque coup
#fps=cap.get(cv.CAP_PROP_FPS) 
#wait_time=round(1000/fps)

#problème de compatibilité gstreamer opencv:
#[ WARN:0@0.047] global /initrd/mnt/dev_save/packages/opencv-4.6.0/modules/videoio/src/cap_gstreamer.cpp (1374) open OpenCV | GStreamer warning: unable to query duration of stream
#[ WARN:0@0.047] global /initrd/mnt/dev_save/packages/opencv-4.6.0/modules/videoio/src/cap_gstreamer.cpp (1405) open OpenCV | GStreamer warning: Cannot query video position: status=1, value=16, duration=-1

print("fps = " + str(cap.get(cv.CAP_PROP_FPS)) )
print("total frames = " + str(cap.get(cv.CAP_PROP_FRAME_COUNT)) ) #-1, pb compatibilité gstreamer je suppose

cap.set(cv.CAP_PROP_POS_FRAMES, 3000) #ne marche pas

while cap.isOpened():
    ret, frame = cap.read()
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
        


    
    ## [display_frame_number (trouvé dans tutorial_code/.../bg_sub.py)]
    #get the frame number and write it on the current frame
    pos_frames=str(int(cap.get(cv.CAP_PROP_POS_FRAMES)))
    #print("CAP_PROP_POS_MSEC=", int(cap.get(cv.CAP_PROP_POS_MSEC)))       
    cv.rectangle(frame, (10, 2), (100,20), (255,255,255), -1)
    cv.putText(frame, pos_frames, (15, 15), cv.FONT_HERSHEY_SIMPLEX, 0.5 , (0,0,0))
    ## [display_frame_number]
    
    cv.namedWindow('ma_window', cv.WINDOW_AUTOSIZE)
    cv.imshow('ma_window', frame)
    cv.moveWindow("ma_window", 100, 100)
    cv.setWindowProperty("ma_window", cv.WND_PROP_TOPMOST, 1)
    	
    if cv.waitKey(33) == ord('q'):
        break
        
        
cap.release()
cv.destroyAllWindows()
