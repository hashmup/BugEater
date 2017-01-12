#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Quaternion
from std_msgs.msg import Empty
from std_msgs.msg import Int32MultiArray, MultiArrayLayout, MultiArrayDimension
import json

x = 0
y = 0

rospy.init_node('demo', anonymous=True)
pub = rospy.Publisher('route2', Int32MultiArray, queue_size=100)
r = rospy.Rate(0.2)

def move(tx, ty):
    global x, y, pub
    cmd = Quaternion()
    cmd.x = tx - x
    cmd.y = ty - y
    cmd.z = -1.0
    cmd.w = 500.0
    x += cmd.x
    y += cmd.y
    pub.publish(cmd)

def main():
    a = Int32MultiArray()
    a.layout.dim.append(MultiArrayDimension())
    a.layout.dim[0].size = 2
    a.data = [100, 100]
    while True:
        try:
            pub.publish(a)
            r.sleep()

        except rospy.ROSInterruptException: pass

if __name__ == '__main__':
    main()
