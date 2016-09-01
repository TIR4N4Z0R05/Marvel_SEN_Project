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
    grid_msg.info.width = width;
    grid_msg.info.height = height;
    grid_msg.info.resolution = reso;
    grid_msg.data.resize(grid.size());
    for(int i=0 ; i < grid.size() ; i++){
    	grid_msg.data.at(i) = grid.at(i);
    }
    pub.publish(grid_msg);
    ros::spinOnce();

    //draw();
 }
 
 int main(int argc, char** argv)
 {
   ros::init(argc, argv, "sub_pcl");
   ros::NodeHandle nh;
   ros::Subscriber sub = nh.subscribe<nav_msgs::OccupancyGrid>("/AUTMAV/new_map", 10, callback);
   pub = nh.advertise<nav_msgs::OccupancyGrid>("AUTMAV/costmap", 1000);
   ros::spin();
 }

void create_safe_zone(){
	float dis;
	grid_borders();
	for(int i=0; i<height ; i++){
		for(int j=0; j < width ; j++ ){
			if(grid.at(i*width + j) == 100){
				for(int k=0; k<height ; k++){
					for(int l=0; l < width ; l++ ){
						dis = find_dis(j,l,i,k);
						if( dis< 0.1 && dis!=0 && grid.at(k*width + l) != 100){
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

void grid_borders(){
	for (int i = 0; i < width; i++){
		grid.at(i) = -50;
		grid.at((height-1)*width +i) = -50;
	}

	for (int i = 0; i < height; i++){
		if(i != height - 1)
			grid.at(i*width) = -50;
		if(i != 0)
			grid.at(i*width - 1) = -50;
	}
}
