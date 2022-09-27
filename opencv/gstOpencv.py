#!/usr/bin/python

#Objectif: améliorer lutilisation de gstreamer par opencv.
#Je lis que videocapture peut prendre des pipelines, peut être que ça permettra à modules/videoio/src/cap_gstreamer.cpp
 # de fetcher des infos
 
#actuellement la pipeline ne marche pas

#ma meilleure piste est qu'il faut convertir en video/x-raw,format=RGB avant l'appsink et cette pipeline ne marche pas
#gst-launch-1.0 filesrc location=/root/v2_8m.mp4 ! qtdemux ! vaapih264dec ! videoconvert ! 'video/x-raw,format=RGB' ! vaapisink

#Peut être helpful: démarrage avec GST_DEBUG=2 ./gstopencv.py

import sys
import cv2 as cv



#cap = cv.VideoCapture('filesrc location=/root/v2_8m.mp4 ! decodebin ! appsink', cv.CAP_GSTREAMER)
cap = cv.VideoCapture("filesrc location=/root/v2_8m.mp4 ! qtdemux ! queue ! h264parse ! vaapih264dec ! videoconvert ! video/x-raw,format=RGB ! appsink", cv.CAP_GSTREAMER)
#cap = cv.VideoCapture("videotestsrc ! video/x-raw,format=BGR ! autovideoconvert ! appsink") #fonctionne

while cap.isOpened():
    ret, frame = cap.read()
    cv.namedWindow('ma_window', cv.WINDOW_AUTOSIZE)
    cv.imshow('ma_window', frame)
    cv.moveWindow("ma_window", 100, 100)
    cv.setWindowProperty("ma_window", cv.WND_PROP_TOPMOST, 1)
    	
    if cv.waitKey(33) == ord('q'):
        break
        
        
cap.release()
cv.destroyAllWindows()
