#include "ros/ros.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "ros/ros.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "image_transport/image_transport.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <ros/ros.h>
#include <nav_msgs/GetMap.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <costmap_2d/costmap_2d.h>
#include <navfn/navfn_ros.h>
#include <GL/glut.h>
#include <sensor_msgs/PointCloud2.h>
#include "rtabmap_ros/MapData.h"
#include "licence.h"


using namespace std;

void getmapdata (const sensor_msgs::PointCloud2::ConstPtr&msg){
cout << "i get it ! " << endl;
}





int main (int argc, char **argv){
licence();
ros::init(argc, argv, "ReadMaP");
ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("/rtabmap/cloud_map", 1000, getmapdata);

ros::spin();
}
