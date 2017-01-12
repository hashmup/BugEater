#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Quaternion
from std_msgs.msg import Empty
import json

x = 0
y = 0

rospy.init_node('demo', anonymous=True)
pub = rospy.Publisher('route2', Quaternion, queue_size=100)
r = rospy.Rate(0.5)

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
    label = ['lt', 'ld', 'rt', 'rd']
    with open('../config.json', 'r') as fp:
        data = json.load(fp)
    try:
        for x in label:
            r.sleep()
            move(data[x]['x'], data[x]['y'])
            r.sleep()
        move(0, 0)
        r.sleep()
    except rospy.ROSInterruptException: pass

if __name__ == '__main__':
    main()
