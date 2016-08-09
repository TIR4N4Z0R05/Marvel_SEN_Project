/*		
					IN ALLAH WE TRUST
		THIS PROGRAM IS DEBUGED AND DEVELOPED BY AUTMAV TEAM UNDER GUI LICENCE
THIS PACKAGE IS DEVELOPED FOR HOLDING POSITION FOR MARVEL QUADROTOR DRONE WHEN THIS DRONE HELD IN TAKE-OFF STATUS.
				ALL-RIGHT RESERVED 2015-2016

*/
#include "PositionHold.h"



using namespace std;
void chatterCallback(const nav_msgs::Odometry::ConstPtr&msg)
{
void Checkstate();

if (i==1){
x=msg->pose.pose.position.x;
y=msg->pose.pose.position.y;
z=msg->pose.pose.position.z;
}
else
{
xg=msg->pose.pose.position.x;
yg=msg->pose.pose.position.y;
zg=msg->pose.pose.position.z;
}
PositionHold();
i++;
}

void PositionHold()
{
deltaX=x-xg;
deltaY=y-yg;
deltaZ=Initialh-zg;
PHmsg.data[0]=deltaX;
PHmsg.data[1]=deltaY;
PHmsg.data[2]=deltaZ;
if (result==0){
pub.publish(PHmsg);
}
ros::spinOnce();
}

bool Checkstate(PositionH::PH_RK::Request &req, PositionH::PH_RK::Response &res){
kill=req.kill;
start=req.run;
res.done=kill; // kill status if it was 1 means that position hold program is off and when its be 0 means that this program running properly and publishing data to specific path this change apply for 		  managing band width usage and saving power of robot
result=res.done;
	if (kill==1 && start==0){
		result=1;
		cout << "Kill state" << endl;
				}
		else if (kill==0 && start==0){
		result=2;
				}
		else if(kill==0 && start==1){
                cout << "publishing state" << endl;
		    }
return true;
}


int main(int argc , char **argv){
licence();
ros::init(argc, argv, "PositionHold");
ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("/rtabmap/odom", 1000, chatterCallback);
pub = n.advertise<std_msgs::Float64MultiArray>("PositionCommand", 1000);

//Creating server for this code!
 ros::ServiceServer service = n.advertiseService("PositionHold/State", Checkstate);
 ROS_INFO("Ready to get command.");

PHmsg.data.resize(3);

		if (result==1){
			return(-1);
          		    }

ros::spin();

return 0;
}
