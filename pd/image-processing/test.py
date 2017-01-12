import cv2
import numpy as np
import params
def nothing(x):
    pass
# Creating a window for later use
cv2.namedWindow('result')

# Starting with 100's to prevent error while masking

# Creating track bar
cv2.createTrackbar('hl', 'result',0,400,nothing)
cv2.createTrackbar('hh', 'result',0,400,nothing)
cv2.createTrackbar('wl', 'result',0,300,nothing)
cv2.createTrackbar('wh', 'result',0,300,nothing)

hl = 0
hh = 300
wl = 0
wh = 400
# for i in range(1, 5):
cap = cv2.VideoCapture(params.cam_number)
while(cap.isOpened()):
# frame = cv2.imread('images/'+ str(i) + '.png')
    ret, frame = cap.read()
    frame = cv2.imread('images/sample.png')
    frame = cv2.resize(frame, (300, 400))
    hl = cv2.getTrackbarPos('hl','result')
    hh = cv2.getTrackbarPos('hh','result')
    wl = cv2.getTrackbarPos('wl','result')
    wh = cv2.getTrackbarPos('wh','result')
    if hl==hh:
        hl = 0
        hh = 300
        wl = 0
        wh = 400
    frame = frame[hl:hh, wl:wh]
    print((hl, hh), (wl, wh))
    cv2.imshow('result',frame)

    k = cv2.waitKey(2000) & 0xFF
    # cv2.waitKey(0)
    if k == 27:
        break

cv2.destroyAllWindows()
