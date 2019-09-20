#!/usr/bin/env python
import rospy
from std_msgs.msg import Int16
import sys, tty, termios
import threading
inc_left_encoder = 0 
inc_right_encoder = 0

def get_char():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        ch =  sys.stdin.read(1)    #This number represents the length
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch

def key_listner():
    global inc_left_encoder 
    global inc_right_encoder 
    while 1:
        key = get_char()
        vel_msg = Int16()
        if "a" in str(key):
            inc_left_encoder += 1
            move_left_publisher.publish(inc_left_encoder)
        elif "d" in str(key):
            inc_right_encoder +=1 
            move_right_publisher.publish(inc_right_encoder)
        elif "z" in str(key):
            inc_left_encoder += -1 
            move_left_publisher.publish(inc_left_encoder)
        elif "c" in str(key):
            inc_right_encoder += -1
            move_right_publisher.publish(inc_right_encoder)
        elif key == "q":
            sys.exit()


rospy.init_node("encoders")
move_left_publisher = rospy.Publisher("/lwheel",Int16,queue_size=1)
move_right_publisher = rospy.Publisher("/rwheel",Int16,queue_size=1)
thread = threading.Thread(target=key_listner, args=())
thread.start()
rate = rospy.Rate(10)
rospy.spin
