#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Quaternion
from std_msgs.msg import Empty
import curses
import sys, select, termios, tty
import json

def save(n, x, y):
    label = ['lt', 'ld', 'rt', 'rd']
    with open('../config.json', 'r') as fp:
        data = json.load(fp)
    data[label[n]] = {'x': x, 'y': y}
    with open('../config.json', 'w') as fp:
        json.dump(data, fp)

def main():
    screen = curses.initscr()
    curses.noecho()
    curses.cbreak()
    screen.keypad(True)
    solenoid = 1.0
    scale = 1.0
    rospy.init_node('move_robot', anonymous=True)
    pub = rospy.Publisher('route', Quaternion, queue_size=100)
    pub2 = rospy.Publisher('tap', Empty, queue_size=100)
    r = rospy.Rate(10)
    x = 0
    y = 0
    try:
        while not rospy.is_shutdown():
            char = screen.getch()
            cmd = Quaternion()
            cmd.x = 0.0
            cmd.y = 0.0
            cmd.z = solenoid
            cmd.w = 100.0
            if char == ord('q'):
                break
            elif char == ord('s'):
                solenoid = -solenoid
                cmd.z = solenoid
            elif char == ord('u'):
                scale = 2 * scale
            elif char == ord('d'):
                scale = 0.5 * scale
            elif char == ord('r'):
                cmd.x = -x
                cmd.y = -y
            elif char == ord('t'):
                cmd2 = Empty()
                pub2.publish(cmd2)
                continue
            elif char == ord('1'):
                # left top
                save(0, x, y)
            elif char == ord('2'):
                #left down
                save(1, x, y)
            elif char == ord('3'):
                #right top
                save(2, x, y)
            elif char == ord('4'):
                #right down
                save(3, x, y)
            elif char == curses.KEY_RIGHT:
                cmd.x = 100.0 * scale
            elif char == curses.KEY_LEFT:
                cmd.x = -100.0 * scale
            elif char == curses.KEY_UP:
                cmd.y = 100.0 * scale
            elif char == curses.KEY_DOWN:
                cmd.y = -100.0 * scale
            x += cmd.x
            y += cmd.y
            print (x, y)
            pub.publish(cmd)
            r.sleep()
    except rospy.ROSInterruptException: pass
    finally:
        curses.nocbreak()
        screen.keypad(0)
        curses.echo()
        curses.endwin()


if __name__ == "__main__":
    main()
