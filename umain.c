// Include headers from OS

int caughtBalls = 0;
long masterStartTime = 0;

#include <joyos.h>  
#include "navigation2.h"
#include "basket.h"
#include "strategy.h"
#include "tests.h"
#define TEAM_NUMBER 3

extern volatile uint8_t robot_id;


// usetup is called during the calibration period. 
int usetup (void) {
	robot_id = TEAM_NUMBER;
	gyroInit();
	//constructPointArray();
	
	superTest();
	
	return 0;
}

// Entry point to contestant code.
int umain (void) {
	
	motor_set_vel(0, 100);
	motor_set_vel(1, 100);
	pause(100);
	motor_set_vel(0, 0);
	motor_set_vel(1, 0);
	
	
	
	//pause(1000);
	
	
	copy_objects();
	masterStartTime = get_time();

	
	bool direction;
	bool team ;
	basketRaise();
	
	//direction: true = cw, false = ccw
	//team: true = blue = pos x, false = red = minus x
	
	if(game.coords[1].y >= 0 && game.coords[1].x < 0){
		direction = true;
		team = true;
	}else if(game.coords[1].y >= 0 && game.coords[1].x >= 0){
		direction = false;
		team = false;
	}else if(game.coords[1].y < 0 && game.coords[1].x < 0){
		direction = false;
		team = true;
	}else{
		direction = true;
		team = false;
	}
	
	//hardcoded ccw explore direction
	direction = false;
	
	explore(direction, team);
	
	win(team);
	
	
	
	
	return 0;
}
