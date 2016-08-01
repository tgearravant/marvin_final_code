#ifndef ROBOT_BASKET_CODE

#define ROBOT_BASKET_CODE

#include <joyos.h> 

#include "navigation.h"

#define BASKET_SERVO_PORT 1
#define BASKET_SERVO_UP_POS 374
#define BASKET_SERVO_DOWN_POS 194
#define BASKET_SERVO_TILT_POS 295

//moved down from midpoint by 200
#define DUMP_POS_0_X 1245
#define DUMP_POS_0_Y -200
#define DUMP_THETA_0 180

//moved left from midpoint by 200
#define DUMP_POS_1_X 423
#define DUMP_POS_1_Y 1079
#define DUMP_THETA_1 240

//at midpoint
#define DUMP_POS_2_X -623
#define DUMP_POS_2_Y 1079
#define DUMP_THETA_2 300

//moved up from midpoint by 200
#define DUMP_POS_3_X -1245
#define DUMP_POS_3_Y 200
#define DUMP_THETA_3 0

//moved right from midpoint by 200
#define DUMP_POS_4_X -423
#define DUMP_POS_4_Y -1079
#define DUMP_THETA_4 60

//at midpoint
#define DUMP_POS_5_X 623
#define DUMP_POS_5_Y -1079
#define DUMP_THETA_5 120

int moveTo2(int targX, int targY, int maxSpeed, int tolerance, float forwardGain);
int territoryDistance(int x1, int y1, int x2, int y2);

int getDumpX(int territory) {
  if (territory==0)
    return DUMP_POS_0_X;
  if (territory==1)
    return DUMP_POS_1_X;
  if (territory==2)
    return DUMP_POS_2_X;
  if (territory==3)
    return DUMP_POS_3_X;
  if (territory==4)
    return DUMP_POS_4_X;
  if (territory==5)
    return DUMP_POS_5_X;
  else
    return -1;
}

int getDumpY(int territory) {
  if (territory==0)
    return DUMP_POS_0_Y;
  if (territory==1)
    return DUMP_POS_1_Y;
  if (territory==2)
    return DUMP_POS_2_Y;
  if (territory==3)
    return DUMP_POS_3_Y;
  if (territory==4)
    return DUMP_POS_4_Y;
  if (territory==5)
    return DUMP_POS_5_Y;
  else
    return -1;
}

int getDumpTheta(int territory) {
  if (territory==0)
    return DUMP_THETA_0;
  if (territory==1)
    return DUMP_THETA_1;
  if (territory==2)
    return DUMP_THETA_2;
  if (territory==3)
    return DUMP_THETA_3;
  if (territory==4)
    return DUMP_THETA_4;
  if (territory==5)
    return DUMP_THETA_5;
  else
    return -1;
}

void emptyBasket(void){

	servo_set_pos(BASKET_SERVO_PORT, BASKET_SERVO_DOWN_POS);
	pause(1000);
	servo_set_pos(BASKET_SERVO_PORT, BASKET_SERVO_UP_POS);
	
	return;
}

void emptyBasketSlowly(void){
	for(int i = BASKET_SERVO_UP_POS; i > BASKET_SERVO_DOWN_POS; i--){
		servo_set_pos(BASKET_SERVO_PORT, i);
		pause(3);
	}
	pause(500);
	servo_set_pos(BASKET_SERVO_PORT, BASKET_SERVO_UP_POS);
}

void basketTilt(void){
	servo_set_pos(BASKET_SERVO_PORT, BASKET_SERVO_TILT_POS);
}

void basketRaise(void){
	servo_set_pos(BASKET_SERVO_PORT, BASKET_SERVO_UP_POS);
}

	


//Returns 0 if success
//Returns 1 if cannot get to intermediate point
//Returns 2 if cannot get to dumping point
//Returns 3 if cannot face dumping angle
int dump(int territory, bool team){
	const int attempts = 3;
	
	
if(team == true){
	if(territory == 0 || territory == 1){
	
		for(int i = 0; moveTo2(DUMP_POS_1_X,DUMP_POS_1_Y,180,100, 0.2)!=0; i++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(i > attempts)
				return 1;
		}
		
		faceDirection(DUMP_THETA_1);
		driveStraight(2000,DUMP_THETA_1);
		
	}else if(territory == 2){
	
		for(int i = 0; moveTo2(DUMP_POS_2_X,DUMP_POS_2_Y,180,100, 0.2) != 0; i++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(i > attempts)
				return 1;
		}
		
		faceDirection(DUMP_THETA_2);
		driveStraight(2000,DUMP_THETA_2);
	}else if(territory == 3 || territory == 4){
	
		for(int i = 0; moveTo2(DUMP_POS_3_X,DUMP_POS_3_Y,180,100, 0.2) != 0; i++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(i > attempts)
				return 1;
		}
		
		faceDirection(DUMP_THETA_3);
		driveStraight(2000,DUMP_THETA_3);
		
	}else{
		for(int i = 0; moveTo2(DUMP_POS_0_X,DUMP_POS_0_Y,180,100, 0.2) != 0; i++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(i > attempts)
				return 1;
		}
		
		for(int j = 0; moveTo2(DUMP_POS_1_X,DUMP_POS_1_Y,180,100, 0.2) != 0; j++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(j > attempts)
				return 1;
		}
		
		faceDirection(DUMP_THETA_1);
		driveStraight(2000,DUMP_THETA_1);
		
	}
	}else{
	if(territory == 0 || territory == 1){
		for(int i = 0; moveTo2(DUMP_POS_0_X,DUMP_POS_0_Y,180,100, 0.2) != 0; i++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(i > attempts)
				return 1;
		}
		
		faceDirection(DUMP_THETA_0);
		driveStraight(2000,DUMP_THETA_0);
		
	}else if(territory == 2){
		for(int i = 0; moveTo2(DUMP_POS_1_X,DUMP_POS_1_Y,180,100, 0.2) != 0; i++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(i > attempts)
				return 1;
		}
		
		for(int j = 0; moveTo2(DUMP_POS_0_X,DUMP_POS_0_Y,180,100, 0.2) != 0; j++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(j > attempts)
				return 1;
		}
		
		faceDirection(DUMP_THETA_0);
		driveStraight(2000,DUMP_THETA_0);
		
	}else if(territory == 3 || territory == 4){
		for(int i = 0; moveTo2(DUMP_POS_4_X,DUMP_POS_4_Y,180,100, 0.2) != 0; i++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(i > attempts)
				return 1;
		}
		
		faceDirection(DUMP_THETA_4);
		driveStraight(2000,DUMP_THETA_4);
		
	}else{
		for(int i = 0; moveTo2(DUMP_POS_5_X,DUMP_POS_5_Y,180,100, 0.2) != 0; i++){
			motor_set_vel(LEFT_MOTOR, -150);
			motor_set_vel(RIGHT_MOTOR, -150);
			pause(300);
			stop();
			
			if(i > attempts)
				return 1;
		}
		
		faceDirection(DUMP_THETA_5);
		driveStraight(2000,DUMP_THETA_5);
	}
}
	
	emptyBasketSlowly();
	
	motor_set_vel(LEFT_MOTOR,-150);
	motor_set_vel(RIGHT_MOTOR,-150);
	pause(500);
	stop();
	return 0;
}

int preDump(bool team){
	//copy objects is called in getCurrentTerritory
	int territory = getCurrentTerritory();
	int robotTerritory = getGoalTerritory(game.coords[1].x, game.coords[1].y);
	int midX = getMidX(robotTerritory);
	int midY = getMidY(robotTerritory);
	int distance = territoryDistance(game.coords[0].x, game.coords[0].y, game.coords[1].x, game.coords[1].y);
	
	
	int attempts = 2;
	
	if(team == true){
		if(territory == 1 || territory == 2 || territory == 3){
			 return dump(getCurrentTerritory(), team);
		}else if(robotTerritory == 1 || robotTerritory == 3 || distance == 1){
		
			for(int i = 0; moveTo2(-midX,-midY,200,200, 0.2) != 0; i++){
				motor_set_vel(LEFT_MOTOR, -150);
				motor_set_vel(RIGHT_MOTOR, -150);
				pause(300);
				stop();
			
				if(i > attempts)
					return 1;
			}
			
		}
		return dump(getCurrentTerritory(), team);
	}else{
		if(territory == 4 || territory == 5 || territory == 0){
			return dump(getCurrentTerritory(),team);
		}else if(robotTerritory == 0 || robotTerritory == 4 || distance == 1){
		
			for(int i = 0; moveTo2(-midX,-midY,200,200, 0.2) != 0; i++){
				motor_set_vel(LEFT_MOTOR, -150);
				motor_set_vel(RIGHT_MOTOR, -150);
				pause(300);
				stop();
			
				if(i > attempts)
					return 1;
			}
			
		}
		return dump(getCurrentTerritory(), team);
	}
}

#endif