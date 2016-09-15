#include "ANT.h"

using namespace std;

void grid_callback(const nav_msgs::OccupancyGrid::ConstPtr& msg){
	map_recieved = true;
	width = msg->info.width;
 	height = msg->info.height;
 	reso = msg->info.resolution;
	grid.resize(0);
    	for(int i=0 ; i < msg->data.size() ; i++){
		if(msg->data.at(i) == 100)
			grid.push_back(2000);
		else if(msg->data.at(i) == -50)
			grid.push_back(2001);
		else
    			grid.push_back(msg->data.at(i));
    	}
	if(former_size == grid.size()){
		for (int i = 0; i < best_path.size(); i++){
			for(int j=best_path.at(i).y - 1; j<best_path.at(i).y + 2; j++){
				for(int k=best_path.at(i).x - 1; k<best_path.at(i).x + 2; k++){
					grid.at(j * width + k) = 2002;
				}
			}
		}
	}
	former_size = grid.size();	
	msg_grid.data.resize(0);
	msg_grid.data.push_back(height); msg_grid.data.push_back(width); msg_grid.data.push_back(reso);/* msg_grid.data.push_back(st.x); msg_grid.data.push_back(st.y);*/
    	for(int i=0 ; i < grid.size(); i++){
		msg_grid.data.push_back(grid.at(i));
    	}
	pub.publish(msg_grid);
	ros::spinOnce();
   	
}

void odom_callback(const nav_msgs::Odometry::ConstPtr& msg){
	odom_recieved = true;

	st.x = msg->pose.pose.position.x/reso + width/2 - 1;
	st.y = msg->pose.pose.position.y/reso + height/2 - 1;

	msg_carot.data.at(3) = msg->pose.pose.position.z;

}

void target_callback(const std_msgs::Float64MultiArray::ConstPtr& msg){
	desti.x = msg->data.at(0);
	desti.y = msg->data.at(1);

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if ( grid.at(i*width + j) == 2002 )
				grid.at(i*width + j) = 0;
		}
	}
	
	if(map_recieved ){
		ROS_INFO("\nsending ants\n");
		main_func();
		ROS_INFO("\npath generating finished\n");
	}
}

int main(int argc, char **argv){
   SDL_Init(SDL_INIT_EVERYTHING);

   srand(time(0));

   msg_carot.data.resize(4,0);
   //boost::thread th(carot_pub);
   	
   ros::init(argc, argv, "ant");
   ros::NodeHandle nh;
   ros::Subscriber sub = nh.subscribe<nav_msgs::OccupancyGrid>("/AUTMAV/costmap", 1000, grid_callback);
   ros::Subscriber sub1 = nh.subscribe<nav_msgs::Odometry>("/rtabmap/odom", 1000, odom_callback);
   ros::Subscriber sub2 = nh.subscribe<std_msgs::Float64MultiArray>("/AUTMAV/target_point", 1000, target_callback);
   pub = nh.advertise<std_msgs::Float64MultiArray>("AUTMAV/grid", 1000);
   pub1 = nh.advertise<std_msgs::Float64MultiArray>("AUTMAV/carot", 1000);
   ros::spin();

   return 0;
}

void main_func(){
	insitial_phero(st);
	numOfFailures = 0;
	anyWaysFound = false;

    for (int counter = 0; counter < 2; counter++){
	label1:
		paths.resize(0);
		score_board.resize(0);

		ANT_purposive myAnt(st, desti, 0);
		myAnt.move();

		choose_bests();
		if (numOfFailures > 20)
			break;
		if (failure){
			numOfFailures++;
			ROS_INFO("failure.\n");
			insitial_phero(st);
			failure = false;
			if (!(numOfFailures > 10 && anyWaysFound))
				goto label1;
		}
		else{
			anyWaysFound = true;
		}
		update();

		int c = 0;
		for (int j = 0; j < score_board.size(); j++){
			if (!score_board.at(j).isArrived)
				c++;
		}

		ROS_INFO("FINISHED\t%i\tof\t%i\thad failed." ,c , score_board.size());
	}
	path_filter();
	for (int i = 0; i < best_path.size(); i++){
		for(int j=best_path.at(i).y - 1; j<best_path.at(i).y + 2; j++){
			for(int k=best_path.at(i).x - 1; k<best_path.at(i).x + 2; k++){
				grid.at(j * width + k) = 2002;
			}
		}
	}

	msg_grid.data.resize(0);
	msg_grid.data.push_back(height); msg_grid.data.push_back(width); msg_grid.data.push_back(reso); /*msg_grid.data.push_back(st.x); msg_grid.data.push_back(st.y);*/
    	for(int i=0 ; i < grid.size(); i++){
		msg_grid.data.push_back(grid.at(i));
    	}
	if(best_path.size() > 20){
		msg_carot.data.at(0) = 1; msg_carot.data.at(1) = (best_path.at(20).x + 1 - width/2)*reso; msg_carot.data.at(2) = (best_path.at(20).y + 1 - height/2)*reso; 
		pub1.publish(msg_carot);
		ros::spinOnce();
	}
	pub.publish(msg_grid);
	ros::spinOnce();
}

void insitial_phero(g_point current){
	double max_dis = -1;
	double distance;

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (grid.at(i*width + j) != 2000 && grid.at(i*width + j) != 2001 )
				grid.at(i*width + j) = 1;
		}
	}
}

double find_grid_dis(g_point p1, g_point p2){
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

void update(){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (grid.at(i*width + j) != 2000 && grid.at(i*width + j) != 2001)
				grid.at(i*width + j) = grid.at(i*width + j) * 0.8;
		}
	}

	for (int i = 0; i < best_path.size(); i++){
		for (int j = best_path.at(i).y - 10; j < best_path.at(i).y + 11; j++){
			for (int k = best_path.at(i).x - 10; k < best_path.at(i).x + 11; k++){
				if (grid.at(j*width + k) != 2000 && grid.at(j*width + k) != 2001){
					if (j >= 0 && j <= height - 1 && k >= 0 && k <= width - 1){
						grid.at(j*width + k) = grid.at(j*width + k) + 15;

						if (grid.at(j*width + k) > 127)
							grid.at(j*width + k) = 127;
					}
				}
			}
		}
	}
}

void choose_bests(){
	Score s;

	for (int i = 0; i < score_board.size(); i++){
		for (int j = 0; j < score_board.size() - i - 1; j++){
			if (score_board.at(j).score > score_board.at(j + 1).score){
				s = score_board.at(j + 1);
				score_board.at(j + 1) = score_board.at(j);
				score_board.at(j) = s;
			}

		}
	}

	int c = score_board.size() - 1;

	while (1){
		if (score_board.at(c).isArrived){
			best_score = score_board.at(c);
			break;
		}
		if (c == 0){
			failure = true;
			break;
		}
		c--;
	}

	best_path.resize(0);

	for (int i = 0; i < paths.at(best_score.code).second.size(); i++){
		best_path.push_back(paths.at(best_score.code).second.at(i));
	}
}

void path_filter(){
	double a, b;
	int tx;
	int x, y;
	bool noObs = false;
	g_point keyp;
	vector <g_point> modified;
	int keyp_num = 0;
	keyp = best_path.front();
	while(!(keyp.x == best_path.back().x && keyp.y == best_path.back().y)){
		for(int i=best_path.size() - 1; i>=0 ; i--){
			if(best_path.at(i).x == keyp.x && best_path.at(i).y == keyp.y){
				keyp_num = i;
				break;		
			}
		}

		for(int i=best_path.size() - 1; i>keyp_num ; i--){
			noObs = false;
			a = (best_path.at(i).x - keyp.x) / sqrt(pow((best_path.at(i).x - keyp.x), 2) + pow((best_path.at(i).y - keyp.y), 2));
			b = (best_path.at(i).y - keyp.y) / sqrt(pow((best_path.at(i).x - keyp.x), 2) + pow((best_path.at(i).y - keyp.y), 2));
			tx = (best_path.at(i).x - keyp.x) / a;

			for(int t = 0; t<=tx ; t++){
				x = a * t  + keyp.x;
				y = b * t  + keyp.y;
				if(grid.at(y*width + x) == 2000 || grid.at(y*width + x) == 2001)
					break;
				if(t == tx)	
					noObs = true;
			}
			
			if(noObs){
				for(int t = 0; t<=tx ; t++){
				x = a * t  + keyp.x;
				y = b * t  + keyp.y;
				modified.push_back({x, y});
				}
				keyp = best_path.at(i);
				if(noObs){
					break;
				}
			}
		}
	}
	best_path.resize(0);
	for (int i=0; i<modified.size() ; i++){
		best_path.push_back(modified.at(i));
	}
}

void carot_pub(){
	ROS_INFO("****************************");
	while(ros::ok()){
		mtx.lock();
		if(msg_carot.data.at(0) == 1){
			pub1.publish(msg_carot);
			ros::spinOnce();
		}
		mtx.unlock();
	}
}
