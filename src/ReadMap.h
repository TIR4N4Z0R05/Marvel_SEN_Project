#include <mrpt/math/ransac_applications.h>
#include <mrpt/random.h>
#include <mrpt/utils/metaprogramming.h>
#include <mrpt/poses/CPose3D.h>
#include <mrpt/opengl/CGridPlaneXY.h>
#include <mrpt/opengl/CPointCloud.h>
#include <mrpt/opengl/stock_objects.h>
#include <mrpt/opengl/CTexturedPlane.h>
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
#include <stdlib.h>
#include <time.h>



using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::math;
using namespace mrpt::random;
using namespace mrpt::poses;
using namespace std;

struct line{
	int x1;
	int x2;
	int y1;
	int y2;
};


float z, z0;
int grid_x, grid_y, grid_x1, grid_y1;
vector <pcl::PointXYZ> grid_points;
CVectorDouble xs,ys;
CVectorDouble lxs,lys;
nav_msgs::OccupancyGrid PP_grid;
ros::Publisher pub;
vector <line> lines;
vector <pcl::PointXYZ> trajectory;
pcl::PointXYZ p;

double movement(pcl::PointXYZ);
void grid_init();
void draw();
void clear_grid();
void ransac_filter();
void registerOnGrid(float x1, float x2, float y1, float y2);
void tr_init();
double grid_dis(int ,int ,int ,int);
