#include "ReadMap.h"

using namespace std;

void getmapdata (const sensor_msgs::PointCloud2::ConstPtr&input){
    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(*input,pcl_pc2);
    pcl::PointCloud<pcl::PointXYZ>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromPCLPointCloud2(pcl_pc2,*temp_cloud);

    clear_grid();

    for(unsigned int i=0;i<temp_cloud->points.size();i++){
	if(temp_cloud->points.at(i).z > z-0.025 && temp_cloud->points.at(i).z < z+0.025){
		grid_x = temp_cloud->points.at(i).x/PP_grid.info.resolution + PP_grid.info.width/2;
		grid_y = temp_cloud->points.at(i).y/PP_grid.info.resolution + PP_grid.info.height/2;

		/*if(grid_x<0 || grid_x>=PP_grid.info.width){
			PP_grid.info.width = PP_grid.info.width + abs(grid_x - PP_grid.info.width);
			PP_grid.info.origin.position.x = -(PP_grid.info.width/2)*PP_grid.info.resolution;
			PP_grid.data.resize(PP_grid.info.height * PP_grid.info.width);
		}
		if(grid_y<0 || grid_y>=PP_grid.info.height){
			PP_grid.info.height = PP_grid.info.height + abs(grid_y - PP_grid.info.height);
			PP_grid.info.origin.position.y = (PP_grid.info.height/2)*PP_grid.info.resolution;
			PP_grid.data.resize(PP_grid.info.height * PP_grid.info.width);
		}*/
		
		PP_grid.data.at( (grid_y-1)*PP_grid.info.width + grid_x ) = 100;
	}
    }
    draw();
    pub.publish(PP_grid);
    ros::spinOnce();

}

void chatterCallback(const nav_msgs::Odometry::ConstPtr&msg)
{
	z=msg->pose.pose.position.z;
	cout << z << endl;
}

int main (int argc, char **argv){
//instializing opengl/***************************************************************************
   glutInit(&argc, argv);
   glutInitWindowSize(640,640);
   glutInitWindowPosition(50,25);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
   glutCreateWindow("Map_View");
   glOrtho(0,640,640,0,-1,1);
//nodehandle stuff/*****************************************************************************
licence();
grid_init();
ros::init(argc, argv, "ReadMaP");
ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("/rtabmap/cloud_map", 1000, getmapdata);
ros::Subscriber sub1 = n.subscribe("/rtabmap/odom", 1000, chatterCallback);
pub = n.advertise<nav_msgs::OccupancyGrid>("new_map", 1000);
ros::spin();
}

void grid_init(){
	PP_grid.info.resolution = 0.05;
	PP_grid.info.width = 640;
	PP_grid.info.height = 640;
	PP_grid.info.origin.position.x = -(PP_grid.info.width/2)*PP_grid.info.resolution;
	PP_grid.info.origin.position.y = (PP_grid.info.height/2)*PP_grid.info.resolution;
	PP_grid.data.resize( PP_grid.info.width * PP_grid.info.height );
	clear_grid();
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT );
	glBegin(GL_POINTS);

	int dis_x = 320 - PP_grid.info.width/2;
	int dis_y = 320 - PP_grid.info.height/2; 

	for (unsigned int i = 0; i < PP_grid.info.height; i++){
		for (unsigned int j = 0; j < PP_grid.info.width; j++){

			if(PP_grid.data.at(i*PP_grid.info.width + j) == 0 || PP_grid.data.at(i*PP_grid.info.width + j) == -1){
				glColor3f(0,0,100);
			}

			if(PP_grid.data.at(i*PP_grid.info.width + j) == 100){
				glColor3f(0,200,0);
			}

			if(PP_grid.data.at(i*PP_grid.info.width + j) == -50){
				glColor3f(200,0,0);
			}

			glVertex3f(dis_x + j,dis_y + i,0);
		}
	}

	glEnd();
  	glFlush();
}

void clear_grid(){
	for(unsigned int i=0 ; i<PP_grid.data.size() ; i++){
		PP_grid.data.at(i) = -1;	
	}
}

