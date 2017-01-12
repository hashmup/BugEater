import socket
import numpy as np
import pickle
import rospy
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import MultiArrayDimension

host = '192.168.179.2'
port = 12345

class Node(object):
    def __init__(self):
        rospy.init_node('client')
        self.executing = False
        self.cnt = 0
        rospy.Subscriber('route2', Float32MultiArray, self.send)
        rospy.Subscriber('setup', Float32MultiArray, self.sendSetup)
    def sendSetup(self, msg):
        if self.cnt:
            return
        self.cnt+=1
        self.send(msg)
    def send(self, msg):
        if self.executing:
            return
        self.executing = True
        print(np.array(msg.data))
        li = []
        tmp = []
        for i in range(len(msg.data)):
            tmp.append(msg.data[i])
            if i % 4 == 3:
                li.append(tmp)
                tmp = []
        print(li)
        self.s = socket.socket()
        self.s.connect((host, port))
        self.s.send(pickle.dumps(li))
        while True:
            data = self.s.recv(1024)
            if data:
                print(data)
            if data == b'done':
                break
        self.executing = False
        self.s.close()
    def run(self):
        r = rospy.Rate(10)
        while not rospy.is_shutdown():
            r.sleep()
def main():
    n = Node()
    n.run()
if __name__ == "__main__":
    main()
