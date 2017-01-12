import cv2
import numpy as np
import params

def nothing(x):
    pass
# Creating a window for later use
cv2.namedWindow('result')

# Starting with 100's to prevent error while masking

# Creating track bar
cv2.createTrackbar('hl', 'result',0,179,nothing)
cv2.createTrackbar('hh', 'result',0,179,nothing)
cv2.createTrackbar('sl', 'result',0,255,nothing)
cv2.createTrackbar('sh', 'result',0,255,nothing)
cv2.createTrackbar('vl', 'result',0,255,nothing)
cv2.createTrackbar('vh', 'result',0,255,nothing)

# for i in range(1, 5):
cap = cv2.VideoCapture(params.cam_number)
while(cap.isOpened()):
# frame = cv2.imread('images/'+ str(i) + '.png')
    ret, frame = cap.read()
    # cv2.imshow("result", frame)
    # cv2.waitKey(1000)
#    frame = cv2.resize(frame, (300, 400))
 #   frame = frame[14:301, 56:263]
    print(frame.shape)
    #converting to HSV
    hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)

    # get info from track bar and appy to result
    hl = cv2.getTrackbarPos('hl','result')
    hh = cv2.getTrackbarPos('hh','result')
    sl = cv2.getTrackbarPos('sl','result')
    sh = cv2.getTrackbarPos('sh','result')
    vl = cv2.getTrackbarPos('vl','result')
    vh = cv2.getTrackbarPos('vh','result')

    # Normal masking algorithm
    lower_blue = np.array([hl,sl,vl])
    upper_blue = np.array([hh,sh,vh])
    print([(hl, hh), (sl, sh), (vl, vh)])
    mask = cv2.inRange(hsv,lower_blue, upper_blue)

    result = cv2.bitwise_and(frame,frame,mask = mask)

    cv2.imshow('result',result)

    k = cv2.waitKey(200) & 0xFF
    # cv2.waitKey(0)
    if k == 27:
        break

cv2.destroyAllWindows()
