#include "costmap.h"

using namespace cv;
using namespace std;

 ros::Publisher pub;

 typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
 
 void callback(const nav_msgs::OccupancyGrid::ConstPtr& msg)
 {
 	width = msg->info.width;
 	height = msg->info.height;
 	reso = msg->info.resolution;

 	grid.resize(msg->data.size());

    for(int i=0 ; i < msg->data.size() ; i++){
    	grid.at(i) = msg->data.at(i);
    }

    create_safe_zone();

    draw();
 }
 
 int main(int argc, char** argv)
 {
//instializing opengl/***************************************************************************
   glutInit(&argc, argv);
   glutInitWindowSize(640,640);
   glutInitWindowPosition(50,25);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
   glutCreateWindow("Map_View");
   glOrtho(0,640,640,0,-1,1);
//nodehandle stuff/*****************************************************************************
   ros::init(argc, argv, "sub_pcl");
   ros::NodeHandle nh;
   ros::Subscriber sub = nh.subscribe<nav_msgs::OccupancyGrid>("/Marvel/new_map", 1, callback);
   ros::spin();
 }

void create_safe_zone(){
	float dis;
	for(int i=0; i<height ; i++){
		for(int j=0; j < width ; j++ ){
			if(grid.at(i*width + j) == 100){
				for(int k=0; k<height ; k++){
					for(int l=0; l < width ; l++ ){
						dis = find_dis(j,l,i,k);
						if( dis< 0.4 && dis!=0 && grid.at(k*width + l) != 100){
							grid.at(k*width + l) = -50;
						}
					}
				}
			}
		}
	}
}

float find_dis(int grid_x1, int grid_x2, int grid_y1, int grid_y2){

	int grid_dy = abs(grid_y2 - grid_y1);
	int grid_dx = abs(grid_x2 - grid_x1);

	float dy = grid_dy * reso;
	float dx = grid_dx * reso;

	float dis = sqrt(pow(dy,2) + pow(dx,2));

	return dis;
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT );
	glBegin(GL_POINTS);

	int dis_x = 320 - width/2;
	int dis_y = 320 - height/2; 

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){

			if(grid.at(i*width + j) == 0 || grid.at(i*width + j) == -1){
				glColor3f(0,0,100);
			}

			if(grid.at(i*width + j) == 100){
				glColor3f(0,200,0);
			}

			if(grid.at(i*width + j) == -50){
				glColor3f(200,0,0);
			}

			glVertex3f(dis_x + j,dis_y + i,0);
		}
	}

	glEnd();
  	glFlush();
}

