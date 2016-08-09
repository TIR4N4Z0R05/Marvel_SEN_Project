#include <iostream>
#include "stdio.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64MultiArray.h"
#include <sstream>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/TwistWithCovariance.h"
#include "PositionH/PH_RK.h"
#include <string>
#include "licence.h"

// Initializing variable
float x,y,z,xg,yg,zg;
float deltaX,deltaY,deltaZ;
int i=1;
const float Initialh=1;
int kill,start,result;

//Initializing ros msg
std_msgs::Float64MultiArray PHmsg;
ros::Publisher pub;

// defining header of function
void PositionHold();
bool Checkstate();
