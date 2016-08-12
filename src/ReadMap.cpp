#include "ros/ros.h"
#include <math.h>
#include <vector>
#include <iostream>
#include "ros/ros.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <GL/glut.h>
#include <sensor_msgs/PointCloud2.h>
#include "licence.h"

#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>

#include <sensor_msgs/PointCloud2.h>
#include <pcl/conversions.h> //I believe you were using pcl/ros/conversion.h
#include <pcl/io/pcd_io.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>
#include <nav_msgs/OccupancyGrid.h>


using namespace std;

void getmapdata (const sensor_msgs::PointCloud2::ConstPtr&input){
    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(*input,pcl_pc2);
    pcl::PointCloud<pcl::PointXYZ>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromPCLPointCloud2(pcl_pc2,*temp_cloud);

}


int main (int argc, char **argv){
licence();
ros::init(argc, argv, "ReadMaP");
ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("/rtabmap/cloud_map", 1000, getmapdata);

ros::spin();
}


