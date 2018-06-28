import cv2
import numpy as np
from matplotlib import pyplot as plt

videopath="/home/yolo/Videos/test.m4v"
def nothing(x):
  pass

if __name__ == "__main__":
    cv2.namedWindow('Colorbars')
    cv2.createTrackbar("lower", "Colorbars",0,255,nothing)
    cv2.createTrackbar("upper", "Colorbars",0,255,nothing)
    cap = cv2.VideoCapture(videopath)
    _, frame = cap.read()
    frame = cv2.resize(frame, None, fx=0.5, fy=0.5, interpolation=cv2.INTER_AREA)
    while(1):





        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        lower_red = np.array([30,150,50])
        upper_red = np.array([255,255,180])

        mask = cv2.inRange(hsv, lower_red, upper_red)
        res = cv2.bitwise_and(frame,frame, mask= mask)

        cv2.imshow('Original',frame)

        lower = cv2.getTrackbarPos("lower", "Colorbars")
        upper = cv2.getTrackbarPos("upper", "Colorbars")

        edges = cv2.Canny(frame,lower,upper)

        #edges = cv2.Canny(frame,0,100)
        cv2.imshow('Edges',edges)

        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break

    cv2.destroyAllWindows()
    cap.release()
