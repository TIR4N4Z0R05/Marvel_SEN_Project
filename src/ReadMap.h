#include "ros/ros.h"
#include <math.h>
#include <vector>
#include <iostream>
#include "ros/ros.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/conversions.h>
#include <pcl/io/pcd_io.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/conversions.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl_ros/transforms.h>
#include <sensor_msgs/PointCloud2.h>
#include "licence.h"
#include <sensor_msgs/PointCloud2.h>
#include <nav_msgs/OccupancyGrid.h>
#include "nav_msgs/Odometry.h"
#include <GL/glut.h>

using namespace std;
float z;
int grid_x, grid_y, grid_z;
vector <pcl::PointXYZ> grid_points;
nav_msgs::OccupancyGrid PP_grid;
ros::Publisher pub;

void grid_init();
void draw();
void clear_grid();
