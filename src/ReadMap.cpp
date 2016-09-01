#include "ReadMap.h"

using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::math;
using namespace mrpt::random;
using namespace mrpt::poses;
using namespace std;

void getmapdata (const sensor_msgs::PointCloud2::ConstPtr&input){

    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(*input,pcl_pc2);
    pcl::PointCloud<pcl::PointXYZ>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromPCLPointCloud2(pcl_pc2,*temp_cloud);

    cout << "gotit" << endl;

    
    clear_grid();

    int n;
    bool isResized = false;

	PP_grid.data.resize(0);
	PP_grid.data.resize(PP_grid.info.width*PP_grid.info.height);

    for(unsigned int i=0;i<temp_cloud->points.size();i++){
	if(temp_cloud->points.at(i).z > z-0.1 && temp_cloud->points.at(i).z < z+0.1){
		grid_x = temp_cloud->points.at(i).x/PP_grid.info.resolution + PP_grid.info.width/2 - 1;
		grid_y = temp_cloud->points.at(i).y/PP_grid.info.resolution + PP_grid.info.height/2 - 1;

		/*if(grid_x<=0 || grid_x>=PP_grid.info.width){
			PP_grid.info.width = PP_grid.info.width + abs(grid_x - PP_grid.info.width);
			PP_grid.info.origin.position.x = -(PP_grid.info.width/2)*PP_grid.info.resolution;
			isResized = true;
			
		}
		if(grid_y<=0 || grid_y>=PP_grid.info.height){
			PP_grid.info.height = PP_grid.info.height + abs(grid_y - PP_grid.info.height);
			PP_grid.info.origin.position.y = (PP_grid.info.height/2)*PP_grid.info.resolution;
			isResized = true;
		}

		if(isResized == true){
			n = (PP_grid.info.height * PP_grid.info.width) - PP_grid.data.size();
			for(int j=0; j<n ; j++){
				PP_grid.data.push_back(-1);
			}
			isResized = false;
		}*/
		
		PP_grid.data.at( (grid_y-1)*PP_grid.info.width + grid_x ) = 100;
		
	}
    }
    pub.publish(PP_grid);
    ros::spinOnce();
}

void chatterCallback(const nav_msgs::Odometry::ConstPtr&msg)
{
	z=msg->pose.pose.position.z;
	p.x = msg->pose.pose.position.x; p.y = msg->pose.pose.position.y; p.z = msg->pose.pose.position.z;
	if(movement(p) > 1)
		trajectory.push_back(p);
}

int main (int argc, char **argv){

grid_init();

tr_init();
z0 = 0;

ros::init(argc, argv, "ReadMap");
ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("/rtabmap/octomap_cloud", 1000, getmapdata);
ros::Subscriber sub1 = n.subscribe("/rtabmap/odom", 1000, chatterCallback);
pub = n.advertise<nav_msgs::OccupancyGrid>("AUTMAV/new_map", 1000);
ros::spin();
}

void grid_init(){
	PP_grid.info.resolution = 0.015;
	PP_grid.info.width = 640;
	PP_grid.info.height = 640;
	PP_grid.info.origin.position.x = -(PP_grid.info.width/2)*PP_grid.info.resolution;
	PP_grid.info.origin.position.y = (PP_grid.info.height/2)*PP_grid.info.resolution;
	PP_grid.data.resize( PP_grid.info.width * PP_grid.info.height );
	clear_grid();
}

void clear_grid(){
	int tr_x, tr_y;	
		
	for(unsigned int j=0; j< PP_grid.data.size(); j++){	
		PP_grid.data.at(j) == 0;		
	}
}

double movement(pcl::PointXYZ curr){
	return sqrt(pow(trajectory.back().x-curr.x,2) + pow(trajectory.back().y-curr.y,2));
}

void tr_init(){
	pcl::PointXYZ p0; p0.x = 0; p0.y = 0; p0.z = 0;
	trajectory.push_back(p0);
}

double grid_dis(int x0, int y0, int x1, int y1){
	return sqrt(pow(x1-x0,2) + pow(y1-y0,2));
}
