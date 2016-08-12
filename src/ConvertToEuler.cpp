#include "ConvertToEuler.h"
using namespace std;


double q0,q1,q2,q3 ;   //quaternian angles
double say,theta,phi;  // Eulerian angles
double R[3][3];
double Say,Theta,Phi;




void getdata (const nav_msgs::Odometry::ConstPtr&msg) {

// getting eulerian angles from rtabmap
q0=msg->pose.pose.orientation.x;
q1=msg->pose.pose.orientation.y;
q2=msg->pose.pose.orientation.z;
q3=msg->pose.pose.orientation.w;
RmatriX();
convertionToEulerian();
}

// orthogonal Matrix
void RmatriX(){
R[0][0]=1-2*(pow(q2,2)+pow(q3,2));
R[0][1]=2*(q1*q2-q0*q3);
R[0][2]=2*(q0*q2+q1*q3);
R[1][0]=2*(q1*q2+q0*q3);
R[1][1]=1-2*(pow(q1,2)+pow(q3,2));
R[1][2]=2*(q2*q3-q0*q1);
R[2][0]=2*(q1*q3-q0*q2);
R[2][1]=2*(q0*q1+q2*q3);
R[2][2]=1-2*(pow(q1,2)+pow(q2,2));
}

//Converter Function
void convertionToEulerian(){

phi=atan((2*(q0*q1+q2*q3))/(1-2*(pow(q1,2)+pow(q2,2))));
theta=asin(2*(q0*q2-q3*q1));
say=atan((2*(q0*q3+q1*q2))/(1-2*(pow(q2,2)+pow(q3,2))));

/*
phi=atan2(2*(2*(q0*q1+q2*q3)),1-2*(pow(q1,2)+pow(q2,2)));
theta=asin(2*(q0*q2-q3*q1));
say=atan2((2*(q0*q3+q1*q2)),1-2*(pow(q2,2)+pow(q3,2)));
*/
Say=RadtoDeg(say);
Theta=RadtoDeg(theta);
Phi=RadtoDeg(phi);

Eulerangle_msg.say=Say;
Eulerangle_msg.theta=Theta;
Eulerangle_msg.phi=Phi;
pub.publish(Eulerangle_msg);

}

float RadtoDeg(float rad){
rad=180*rad/(3.141565357989);
return rad;
}







int main(int argc , char **argv){
licence();
ros::init(argc, argv, "EulerConverter");
ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("/rtabmap/odom", 1000, getdata);
pub = n.advertise<Marvel_SEN_Project::Eulerdata>("Euleriandata", 1000);

ros::spin();
return (0);
}
