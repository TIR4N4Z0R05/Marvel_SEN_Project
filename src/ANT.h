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

const int MAX_ANT_STEPS = 1000;
const int STEP_LENGHT = 2;

struct point{
	double x;
	double y;
};

struct g_point{
	int x;
	int y;
};

struct Score{
	int code;
	bool isArrived;
	double score;
};

vector <pair<int, vector<g_point> > > paths;
vector <Score> score_board;
vector <g_point> best_path;
Score best_score;
vector <g_point> wall_side;
ros::Publisher pub;
ros::Publisher pub1;
std_msgs::Float64MultiArray msg_grid;
std_msgs::Float64MultiArray msg_carot;
boost::mutex mtx;

void main_func();
double find_grid_dis(g_point, g_point);
void choose_bests();
void update();
void insitial_phero(g_point);
void path_filter();
void carot_pub();

unsigned int width, height;
double reso;
bool failure = false;
bool anyWaysFound = false;
bool odom_recieved = false;
bool map_recieved = false;
int numOfFailures = 0;
g_point st = {320, 320};
g_point desti;
g_point carot;
vector <double> grid;

class ANT_purposive{
public:
	int code;
	int father_code;
	int genealogy;
	int step;
	g_point status;
	g_point start_p;
	g_point destination;
	vector <g_point> ANT_PATH;

	ANT_purposive(int n, vector<g_point> father_path, g_point start, g_point des, int g){
		father_code = n;
		pair<int, vector<g_point> > p;
		code = paths.size();
		p.first = code;
		ANT_PATH.resize(0);
		for (int z = 0; z < father_path.size(); z++){
			ANT_PATH.push_back({ father_path.at(z).x, father_path.at(z).y });
		}
		for (int z = 0; z < ANT_PATH.size(); z++){
			p.second.push_back({ ANT_PATH.at(z).x, ANT_PATH.at(z).y });
		}
		paths.push_back(p);
		step = 0;
		status.x = start.x;
		status.y = start.y;

		start_p.x = start.x;
		start_p.y = start.y;
		destination.x = des.x;
		destination.y = des.y;
		genealogy = g;
	}

	ANT_purposive(g_point start, g_point des, int g){
		pair<int, vector<g_point> > p;
		code = paths.size();
		p.first = code;
		ANT_PATH.resize(0);
		for (int z = 0; z < ANT_PATH.size(); z++){
			p.second.push_back({ ANT_PATH.at(z).x, ANT_PATH.at(z).y });
		}
		paths.push_back(p);
		step = 0;
		status.x = start.x;
		status.y = start.y;

		start_p.x = start.x;
		start_p.y = start.y;
		destination.x = des.x;
		destination.y = des.y;
		genealogy = g;
	}

	void evaluation(bool isArrived){
		double ant_score;
		double t1, t2, dy, dx, dt, sdt;
		sdt = 0;
		if (ANT_PATH.size() >= 3){
			for (int i = 1; i < ANT_PATH.size() - 1; i++){
				dy = (ANT_PATH.at(i).y - ANT_PATH.at(i - 1).y);
				dx = (ANT_PATH.at(i).x - ANT_PATH.at(i - 1).x);

				t1 = atan(dy / dx)*(180.0 / 3.1415);

				dy = (ANT_PATH.at(i + 1).y - ANT_PATH.at(i).y);
				dx = (ANT_PATH.at(i + 1).x - ANT_PATH.at(i).x);

				t2 = atan(dy / dx)*(180.0 / 3.1415);

				dt = abs(t2 - t1);

				sdt = sdt + dt;
			}


			ant_score = (find_grid_dis(start_p, destination) - find_grid_dis(ANT_PATH.back(), destination)) / (find_grid_dis(start_p, destination) - ANT_PATH.size());

			score_board.push_back({ code, isArrived, ant_score });

		}
	}

	void move(){
		bool break_check1 = false;
		bool break_check2 = false;
		while (step < MAX_ANT_STEPS){
			for (int i = status.y - 1; i < status.y + 2; i++){
				for (int j = status.x - 1; j < status.x + 2; j++){
					if (i >= 0 && i <= height - 1 && j >= 0 && j <= width - 1){
						if (grid.at(i*width + j) == 2001){
							martyrdom();
							evaluation(false);
							for (int z = 0; z < ANT_PATH.size(); z++){
								paths.at(code).second.push_back({ ANT_PATH.at(z).x, ANT_PATH.at(z).y });
							}
							ANT_PATH.clear();
							break_check1 = true;
							break;
						}
					}
				}
				if (break_check1)
				{
					break_check2 = true;
					break_check1 = false;
					break;
				}
			}

			if (break_check2){
				break_check2 = false;
				break;
			}

			if (step != 0 && genealogy < 7){
				breed();
			}

			double max_prob = -10000000;
			double _prob;
			double dy, dx, t1, t2, dt;
			dt = 0;
			g_point max_prob_p;
			ANT_PATH.push_back(status);
			for (int i = status.y - STEP_LENGHT; i < status.y + STEP_LENGHT + 1; i = i + STEP_LENGHT){
				for (int j = status.x - STEP_LENGHT; j < status.x + STEP_LENGHT + 1; j = j + STEP_LENGHT){
					if (j > width - 1)
						break;

					if (ANT_PATH.size() > 3){
						dy = (ANT_PATH.at(ANT_PATH.size() - 1).y - i);
						dx = (ANT_PATH.at(ANT_PATH.size() - 1).x - j);

						t1 = atan(dy / dx)*(180.0 / 3.1415);

						/*if (dy > 0 && dx < 0)
						t1 = t1 + 180;
						if (dy < 0 && dx < 0)
						t1 = t1 + 180;
						if (dy < 0 && dx > 0)
						t1 = t1 + 360;*/

						dy = (ANT_PATH.at(ANT_PATH.size() - 1).y - ANT_PATH.at(ANT_PATH.size() - 2).y);
						dx = (ANT_PATH.at(ANT_PATH.size() - 1).x - ANT_PATH.at(ANT_PATH.size() - 2).x);

						t2 = atan(dy / dx)*(180.0 / 3.1415);

						/*if (dy > 0 && dx < 0)
						t2 = t2 + 180;
						if (dy < 0 && dx < 0)
						t2 = t2 + 180;
						if (dy < 0 && dx > 0)
						t2 = t2 + 360;*/

						dt = abs(t2 - t1);
					}

					if (i != status.y && j != status.x && dt < 91){
						_prob = prob({ j, i }, dt);
						if (_prob > max_prob){
							max_prob = _prob;
							max_prob_p = { j, i };
						}
					}
				}
				if (i > height - 1)
					break;
			}
			status.x = max_prob_p.x;
			status.y = max_prob_p.y;

			for (int i = destination.y - 2; i < destination.y + 3; i++){
				for (int j = destination.x - 2; j < destination.x + 3; j++){
					if (i <= height - 1 && i >= 0 && j <= width - 1 && j >= 0){
						if (status.y == i && status.x == j){
							evaluation(true);
							for (int z = 0; z < ANT_PATH.size(); z++){
								paths.at(code).second.push_back({ ANT_PATH.at(z).x, ANT_PATH.at(z).y });
							}
							ANT_PATH.clear();
							break;
						}
					}
				}
			}

			if (step == MAX_ANT_STEPS - 1){
				evaluation(false);
				for (int z = 0; z < ANT_PATH.size(); z++){
					paths.at(code).second.push_back({ ANT_PATH.at(z).x, ANT_PATH.at(z).y });
				}
				ANT_PATH.clear();
				break;
			}

			step++;
		}

	}

	double prob(g_point mp, double dt){
		if (dt < 1)
			dt = 1;
		if (grid.at(mp.y * width + mp.x) == 2001)
			return 0;
		else{
			double ran = ((rand() % 1000 + 1));
			return pow(grid.at(mp.y * width + mp.x), 1)*ran / pow(find_grid_dis(mp, destination), 10);
		}
	}

	void martyrdom(){
		int r = 0;
		for (int k = 50; k < ANT_PATH.size() - 1; k++){
			for (int i = ANT_PATH.at(k).y - 2; i < ANT_PATH.at(k).y + 3; i++){
				for (int j = ANT_PATH.at(k).x - 2; j < ANT_PATH.at(k).x + 3; j++){
					if (grid.at(i*width + j) != 2001 && grid.at(i*width + j) != 2000 && i >= 0 && i <= height - 1 && j >= 0 && j <= width - 1)
						grid.at(i*width + j) = grid.at(i*width + j) - 0.5;
				}
			}
		}
		for (int i = ANT_PATH.back().y - 10; i < ANT_PATH.back().y + 11; i++){
			for (int j = ANT_PATH.back().x - 10; j < ANT_PATH.back().x + 11; j++){
				if (grid.at(i*width + j) != 2001 && grid.at(i*width + j) != 2000 && i >= 0 && i <= height - 1 && j >= 0 && j <= width - 1)
					grid.at(i*width + j) = grid.at(i*width + j) - 30;
			}
		}

		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (grid.at(i*width + j) < -127)
					grid.at(i*width + j) = -127;
			}
		}

	}

	void breed(){
		genealogy++;
		ANT_purposive *child = new ANT_purposive(code, ANT_PATH, status, destination, genealogy);
		child->move();
	}
};
