#include "interface.h"


 void callback(const std_msgs::Float64MultiArray::ConstPtr& msg)
 {
    mtx.lock();
    height = msg->data.at(0); width = msg->data.at(1);
    grid.resize(0);
    for(int i=2 ; i < msg->data.size(); i++){
	grid.push_back(msg->data.at(i));
    }
    mtx.unlock();
 }

 int main(int argc, char** argv)
 {
   SDL_Init(SDL_INIT_EVERYTHING);
   ros::init(argc, argv, "interface");

   win = SDL_CreateWindow("Intellegent Adabtive Breeding Ant", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 	               SDL_WINDOW_SHOWN);
   ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

   boost::thread th(ros_handle);
   draw_handle();

   th.join();

   return 0;
 }

void draw(){	
	float temp_pher;
	dis_x = 400 - width/2;
	dis_y = 400 - height/2; 
	SDL_Color col;

	for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (grid.at(width*i + j) == 2000){
					col = { 0, 255, 0, 0 };
				}
				else if (grid.at(width*i + j) == 2001){
					col = { 255, 0, 0, 0 };
				}
				else if (grid.at(width*i + j) == 2002){
					col = { 155, 155, 155, 0 };
				}
				else if (grid.at(width*i + j) == 2003){
					col = { 255, 200, 0, 0 };
				}
				else{
					temp_pher = grid.at(width*i + j) + 128;
					if (temp_pher != 0)
						col = { temp_pher, 0, 100, 0 };
				}
				SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, col.a);
				if(dis_x + j >= 0 && dis_x + j < 800 && dis_y + i >= 0 && dis_y + i < 800)
					SDL_RenderDrawPoint( ren, dis_x + j, dis_y + i );
			}
	}
}

void draw_handle(){
	

	bool QUIT = false;
	bool isClicked = false;

	while(!QUIT){
		while(SDL_PollEvent(&e)){
		if (e.type == SDL_MOUSEBUTTONDOWN){
			
			if (e.button.button == SDL_BUTTON_LEFT){
		 		SDL_GetMouseState(&desti.x, &desti.y);
				desti.x = desti.x - dis_x; desti.y = desti.y - dis_y;
				desti_msg.data.resize(0); desti_msg.data.push_back(desti.x); desti_msg.data.push_back(desti.y); 
				pub.publish(desti_msg);
				cout << "got destination point\tx = " << desti.x << "\ty = " << desti.y << endl;
			}
		}	
		}
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    	SDL_RenderClear(ren);
	mtx.lock();
    	draw();
	mtx.unlock();
    	SDL_RenderPresent(ren);	
	}

}

void ros_handle(){
	ros::NodeHandle nh;
   	ros::Subscriber sub = nh.subscribe<std_msgs::Float64MultiArray>("/AUTMAV/grid", 10, callback);
	pub = nh.advertise<std_msgs::Float64MultiArray>("AUTMAV/target_point", 10);
   	ros::spin();
}
