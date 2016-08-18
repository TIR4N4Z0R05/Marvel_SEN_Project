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

    //if(z-z0>0.3){
	//cout << "clearing" << endl;
	//z0 = z;
    	clear_grid();
    //}
    lines.resize(0);
    int n;
    bool isResized = false;

    for(unsigned int i=0;i<temp_cloud->points.size();i++){
	if(temp_cloud->points.at(i).z > z-0.1 && temp_cloud->points.at(i).z < z+0.1){
		grid_x = temp_cloud->points.at(i).x/PP_grid.info.resolution + PP_grid.info.width/2 - 1;
		grid_y = temp_cloud->points.at(i).y/PP_grid.info.resolution + PP_grid.info.height/2 - 1;

		xs.push_back(temp_cloud->points.at(i).x);
		ys.push_back(temp_cloud->points.at(i).y);

		if(grid_x<=0 || grid_x>=PP_grid.info.width){
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
		}
		
		PP_grid.data.at( (grid_y-1)*PP_grid.info.width + grid_x ) = 100;
		
	}
    }
    //ransac_filter();
    //draw();
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
//instializing opengl/***************************************************************************
  
/* glutInit(&argc, argv);
   glutInitWindowSize(640,640);
   glutInitWindowPosition(50,25);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
   glutCreateWindow("Map_View");
   glOrtho(0,640,640,0,-1,1);
*/
//nodehandle stuff/*****************************************************************************

tr_init();
z0 = 0;

licence();
grid_init();
ros::init(argc, argv, "ReadMaP");
ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("/rtabmap/octomap_cloud", 1000, getmapdata);
ros::Subscriber sub1 = n.subscribe("/rtabmap/odom", 1000, chatterCallback);
pub = n.advertise<nav_msgs::OccupancyGrid>("Marvel/new_map", 1000);
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
	int tr_x, tr_y;
	//for(unsigned int i=0 ; i<trajectory.size(); i++){
	//	tr_x = trajectory.at(i).x/PP_grid.info.resolution + PP_grid.info.width/2;
	//	tr_y = trajectory.at(i).y/PP_grid.info.resolution + PP_grid.info.height/2;	
		
		for(unsigned int j=0; j< PP_grid.data.size(); j++){
			if(/*grid_dis(tr_x,tr_y,j/PP_grid.info.width,j%PP_grid.info.width) < 20 &&*/ PP_grid.data.at(j) == 100)
				PP_grid.data.at(j) == -1;		
		}
		
	//}
}

void ransac_filter(){
	TPoint2D p1, p2;
	vector<pair<size_t,TLine2D > > detectedLines;
	const double DIST_THRESHOLD = 0.1;

	ransac_detect_2D_lines(xs,ys,detectedLines,DIST_THRESHOLD,100 );

	for (vector<pair<size_t,TLine2D> >::iterator p=detectedLines.begin();p!=detectedLines.end();++p)
	{
		CVectorDouble lx(2),ly(2);
		lx[0] = -1;
		lx[1] = 1;
		for (CVectorDouble::Index q=0;q<lx.size();q++){
			ly[q] = -(p->second.coefs[2]+p->second.coefs[0]*lx[q])/p->second.coefs[1];
			//cout << p->first
		}
		grid_x = lx[0]/PP_grid.info.resolution + PP_grid.info.width/2;
		grid_y = ly[0]/PP_grid.info.resolution + PP_grid.info.height/2;
		grid_x1 = lx[1]/PP_grid.info.resolution + PP_grid.info.width/2;
		grid_y1 = ly[1]/PP_grid.info.resolution + PP_grid.info.height/2;
		lines.push_back({grid_x, grid_x1, grid_y, grid_y1});
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
