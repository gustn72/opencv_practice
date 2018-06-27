# -*- coding: utf-8 -*-
"""
Created on Mon Jun 25 07:57:02 2018

@author: khs94
"""

import numpy as np
import tensorflow as tf
import cv2

cap = cv2.VideoCapture(0)
cap2 = cv2.VideoCapture(1)
# Define the codec and create VideoWriter object
fourcc = cv2.VideoWriter_fourcc(*'XVID')
#out = cv2.VideoWriter('output.avi',fourcc, 20.0, (640,480))

while(cap.isOpened()):
    ret, frame = cap.read()
    ret2, frame2 =cap2.read()
    if ret==True:
        #frame = cv2.flip(frame,0)

        # write the flipped frame
        #out.write(frame)
        
        cv2.imshow('frame',frame)
        cv2.imshow('frame2',frame2)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break

# Release everything if job is finished
cap.release()
#out.release()
cv2.destroyAllWindows()