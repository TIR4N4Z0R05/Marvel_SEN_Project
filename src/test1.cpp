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


using namespace cv;
using namespace std;

 typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
 
 void callback(const PointCloud::ConstPtr& msg)
 {
   cout << msg->weight << "\t" << msg->length << endl;
 }
 
 int main(int argc, char** argv)
 {
   ros::init(argc, argv, "sub_pcl");
   ros::NodeHandle nh;
   ros::Subscriber sub = nh.subscribe<PointCloud>("/rtabmap/proj_map", 1, callback);
   ros::spin();
 }