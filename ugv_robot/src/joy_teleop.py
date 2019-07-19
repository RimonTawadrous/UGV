#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy
from std_msgs.msg import Int8

prev_twist_msg = Twist()

def callback(joy_msg):
    global prev_twist_msg

    new_msg = Twist()
    joy_buttons = Int8()

    speed = joy_msg.axes[1]*255

    if joy_msg.buttons[1]:
        angle = joy_msg.axes[0]*90*(-1)
    else:
        angle = 0
    yaw = joy_msg.axes[2]*255


    new_msg.linear.x = int(speed)
    new_msg.angular.z = int(angle)

    joy_buttons.data |= joy_msg.buttons[10]
    joy_buttons.data |= joy_msg.buttons[11]<<1


    if not prev_twist_msg == new_msg:
        message_pub.publish(new_msg)
        prev_twist_msg = new_msg
    else:
        buttons_pub.publish(joy_buttons)



rospy.init_node("our_joy")
rospy.Subscriber("joy", Joy, callback)
message_pub = rospy.Publisher("/cmd_vel", Twist, queue_size=1)
buttons_pub = rospy.Publisher("/buttons", Int8, queue_size=1)
rate = rospy.Rate(10)
while not rospy.is_shutdown():
    rate.sleep()
