
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/TeleportAbsolute.h>
#include <turtlesim/SetPen.h>
#include <std_srvs/Empty.h>
#include <math.h>
#define _USE_MATH_DEFINES

ros::Publisher turtle1_pub;
ros::ServiceClient turtle1_set_pen_client;
ros::ServiceClient turtle1_teleport_client;
turtlesim::SetPen pen;
turtlesim::TeleportAbsolute tele;
geometry_msgs::Twist move;

void moveTurtle(double speed) {
    move.linear.x = speed;
    turtle1_pub.publish(move);
    ros::Duration(1).sleep();
    move.linear.x = 0;
    turtle1_pub.publish(move);
}

void rotate(double degree) {
    move.angular.z = degree * M_PI / 180;
    turtle1_pub.publish(move);
    ros::Duration(1.5).sleep();
    move.angular.z = 0;
    turtle1_pub.publish(move);
}

void setpen(int red, int green, int blue) {
    pen.request.r = red;
    pen.request.g = green;
    pen.request.b = blue;
    pen.request.width = 2;
    pen.request.off = 0;

    if (!turtle1_set_pen_client.call(pen)) {
        ROS_ERROR("Failed to call set_pen service");
    }
}

void teleport(float x, float y, float theta = 0) {
    tele.request.x = x;
    tele.request.y = y;
    tele.request.theta = theta;
    ros::Duration(1).sleep();

    if (!turtle1_teleport_client.call(tele)) {
        ROS_ERROR("Failed to call teleport_absolute service");
    }
}


void write_J(int r, int g, int b, double x, double y) {
    setpen(255, 255, 255);
    teleport(x, y);
    setpen(r, g, b);

    // rotate(180.0);
    moveTurtle(1.0);
    rotate(270.0);
    moveTurtle(2.0);
    rotate(-90.0);
    moveTurtle(1.2);
}

void write_E(int r, int g, int b, double x, double y) {
    setpen(255, 255, 255);
    teleport(x, y);
    setpen(r, g, b);

    rotate(-90.0);
    moveTurtle(2.0);

    rotate(90.0);
    moveTurtle(1.0);

    setpen(255, 255, 255);
    teleport(x, y - 1); 
    setpen(r, g, b);
    moveTurtle(1.0);

    setpen(255, 255, 255);
    teleport(x, y);
    setpen(r, g, b);
    moveTurtle(1.0);
}


void write_R(int r, int g, int b, double x, double y) {
    setpen(255, 255, 255);
    teleport(x, y);
    setpen(r, g, b);

    rotate(90.0);
    moveTurtle(2.0);

    rotate(-120.0);
    moveTurtle(1.0);

    rotate(-120.0);
    moveTurtle(1.0);

    rotate(105.0);
    moveTurtle(sqrt(2));
}

void write_M(int r, int g, int b, double x, double y) {
    setpen(255, 255, 255);
    teleport(x, y);
    setpen(r, g, b);

    rotate(90.0);
    moveTurtle(2.0);

    rotate(-135.0);
    moveTurtle(sqrt(2));

    rotate(90.0);
    moveTurtle(sqrt(2));

    rotate(-135.0);
    moveTurtle(2.0);
}

void write_Y(int r, int g, int b, double x, double y) {
    setpen(255, 255, 255);
    teleport(x, y);
    setpen(r, g, b);

    rotate(240.0);
    moveTurtle(1.2);

    rotate(-120.0);
    moveTurtle(1.2);

    teleport(x - ((sqrt(1.2))/2), y - 1.05); 
    setpen(255, 255, 255);
    setpen(r, g, b);
    rotate(-90.0);
    moveTurtle(1.0);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "control_turtle1_JEREMY");
    ros::NodeHandle nh;

    nh.setParam("/turtlesim/background_r", 255); // putih
    nh.setParam("/turtlesim/background_g", 255);
    nh.setParam("/turtlesim/background_b", 255);

    turtle1_pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
    turtle1_set_pen_client = nh.serviceClient<turtlesim::SetPen>("/turtle1/set_pen");
    turtle1_teleport_client = nh.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute");
    ros::ServiceClient clear_client = nh.serviceClient<std_srvs::Empty>("/clear");
    

    std_srvs::Empty srv;
    ros::Rate rate(0.1);
    ros::Duration(1.0).sleep();

    while (ros::ok()) {
        write_J(0, 0, 0, 0.5, 6);
        write_E(0, 0, 0, 2.0, 6);
        write_R(0, 0, 0, 4.0, 4);
        write_E(0, 0, 0, 5.5, 6);
        write_M(0, 0, 0, 7.0, 4);
        write_Y(0, 0, 0, 11, 6);

        write_J(255, 255, 255, 0.5, 6);
        write_E(255, 255, 255, 2.0, 6);
        write_R(255, 255, 255, 4.0, 4);
        write_E(255, 255, 255, 5.5, 6);
        write_M(255, 255, 255, 7.0, 4);
        write_Y(255, 255, 255, 11, 6);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}


