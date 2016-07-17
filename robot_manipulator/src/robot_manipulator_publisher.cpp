#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/JointState.h>
#include <list>

#include <sstream>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init(): remapping that were provided at the command line.
   * 
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "robot_manipulator_publisher");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  ros::Publisher joint_command_pub = n.advertise<sensor_msgs::JointState>("joint_command", 10);

  ros::Rate loop_rate(100);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  float step = 0.001;
  float current_joint_state = 0.0;
  bool turn_right = true;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    sensor_msgs::JointState joint_msg;

    /**
     * For testing purposes, create a waiste joint motion between 2 fixed ponts.
     */
    if (turn_right == true && current_joint_state < 1)
    {
      current_joint_state += step;
    }
    else
    {
      turn_right = false;
      if (turn_right == false && current_joint_state > -1)
      {
        current_joint_state -= step;
      }
      else
      {
        turn_right = true;
      }
    }

    /**
     * Update the joint state positions
     */
     
    //joint_msg.header = auto;
    joint_msg.name.push_back("waist_rotate");
    joint_msg.position.push_back(current_joint_state);
    //joint_msg.velocity.push_back();
    //joint_msg.effort.push_back();

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    joint_command_pub.publish(joint_msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}