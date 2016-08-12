#include <iostream>
#include "stdio.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64MultiArray.h"
#include <sstream>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/TwistWithCovariance.h"
#include <string>
#include "licence.h"
#include <math.h>
#include "Marvel_SEN_Project/Eulerdata.h"



ros::Publisher pub;
Marvel_SEN_Project::Eulerdata Eulerangle_msg;

void RmatriX();
void convertionToEulerian();
float RadtoDeg(float rad);
