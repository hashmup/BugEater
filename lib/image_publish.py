#!/usr/bin/env python

import rospy
import json
import roslib
import cv2
import params
from sensor_msgs.msg import CompressedImage

def main():
    rospy.init_node('processed_image', anonymous=True)
    pub = rospy.Publisher("/processed/image_raw/compressed", CompressedImage)
    cap = cv2.VideoCapture(params.cam_number)
    r = rospy.Rate(10)
    with open("../config.json", "r") as fp:
        data = json.load(fp)
    try:
        while(cap.isOpened() and not rospy.is_shutdown()):
            ret, frame = cap.read()
            
            rospy.spin()
    except rospy.ROSInterruptException: pass
if __name__ == "__main__":
    main()
