#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <turtlesim/TeleportAbsolute.h>
#include <iostream>

void robotPositionCallback(const geometry_msgs::Pose::ConstPtr& msg) {
    ros::NodeHandle nh;
    ros::ServiceClient robot_client = nh.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute");

    turtlesim::TeleportAbsolute srv;
    srv.request.x = msg->position.x;
    srv.request.y = msg->position.y;

    if (robot_client.call(srv)) {
        ROS_INFO("Moved robot turtle to: (%f, %f)", msg->position.x, msg->position.y);
        ros::Duration(0.05).sleep();
    }
}

void ballPositionCallback(const geometry_msgs::Pose::ConstPtr& msg) {
    ros::NodeHandle nh;
    ros::ServiceClient ball_client = nh.serviceClient<turtlesim::TeleportAbsolute>("/turtle2/teleport_absolute");

    turtlesim::TeleportAbsolute srv;
    srv.request.x = msg->position.x;
    srv.request.y = msg->position.y;

    if (ball_client.call(srv)) {
        ROS_INFO("Moved ball turtle to: (%f, %f)", msg->position.x, msg->position.y);
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "turtle_controller");
    ros::NodeHandle nh;

    ros::Subscriber robot_sub = nh.subscribe("robot_position", 10, robotPositionCallback);
    ros::Subscriber ball_sub = nh.subscribe("ball_position", 10, ballPositionCallback);

    ros::spin();
    return 0;
}
