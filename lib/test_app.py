#!/usr/bin/env python

import numpy as np
import pickle
import rospy
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import MultiArrayDimension
from std_msgs.msg import String

class Node(object):
    def __init__(self):
        rospy.init_node('test_app', anonymous=True)
        self.pub = rospy.Publisher('route2', Float32MultiArray, queue_size=10)
        self.pub2 = rospy.Publisher('generate', String, queue_size=10)
        self.executing = False
        rospy.Subscriber('position', Float32MultiArray, self.callback)
    def callback(self, msg):
        if self.executing:
            return
        self.executing = True
        r = rospy.Rate(5)
        mat = Float32MultiArray()
        mat.layout.dim.append(MultiArrayDimension())
        mat.layout.dim[0].label = "height"
        mat.layout.dim[0].size = 16
        mat.data = [0]*16
        for i in range(2):
            route = [
                -129000, -135000, 0.0, 1.0,
                0.0, 0.0, 1.0, 0.1,
                0.0, 0.0, 0.0, 0.1,
                0.0, 0.0, 0.0, 1.0
            ]
            mat.data = route
            self.pub.publish(mat)
            r.sleep()

        for i in range(2):
            route = [
                -94000, -200000, 0.0, 1.0,
                0.0, 0.0, 1.0, 0.1,
                0.0, 0.0, 0.0, 0.1,
                0.0, 0.0, 0.0, 1.0
            ]
            mat.data = route
            self.pub.publish(mat)
            r.sleep()

        for i in range(5):
            self.pub2.publish("error")
            r.sleep()

    def run(self):
        r = rospy.Rate(1.0)
        while not rospy.is_shutdown():
            r.sleep()
def main():
    n = Node()
    n.run()
if __name__ == '__main__':
    main()
