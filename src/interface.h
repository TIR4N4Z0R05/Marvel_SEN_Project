#include <iostream>
#include <vector>
#include <utility> 
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "ros/ros.h"
#include <nav_msgs/GetMap.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64MultiArray.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <costmap_2d/costmap_2d.h>
#include <GL/glut.h>
#include <boost/thread/thread.hpp>
#include <SDL2/SDL.h>

using namespace std;

struct g_point{
	int x;
	int y;
};

vector <double> graphics_grid_model;
vector <double> grid;
unsigned int width, height;
int dis_y;
int dis_x;
g_point desti;
std_msgs::Float64MultiArray desti_msg;
ros::Publisher pub;
boost::mutex mtx;

void Create_ggmodel();
void draw();
void draw_handle();
void ros_handle();

SDL_Window *win;
SDL_Renderer *ren;
SDL_Event e;

