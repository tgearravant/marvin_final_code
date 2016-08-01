#ifndef ROBOT_NAVIGATION2_CODE

#define ROBOT_NAVIGATION2_CODE

#include <joyos.h>
#include <math.h>
#include "navigation.h"

#define LEFT_MOTOR   0
#define RIGHT_MOTOR  1
#define ENCODER_CONSTANT 30 //counts per full wheel spin
#define CIRCUMFERENCE 10.2
#define ENCODER_PIN 27
#define VPS_PER_FOOT_M1 682.6666
#define VPS_PER_FOOT_M2 443.4
#define PI 3.14159265358979323846
#define THETA_TO_DEG 0.087890625
#define INNER_REPELLING_CONSTANT -2488320000000000
#define OUTER_REPELLING_CONSTANT -5000
#define ATTRACTING_FORCE_CONSTANT 50
#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.0174532925

#define MID_0_X 1245
#define MID_0_Y 0

#define MID_1_X 623
#define MID_1_Y 1079

#define MID_2_X -623
#define MID_2_Y 1079

#define MID_3_X -1245
#define MID_3_Y 0

#define MID_4_X -623
#define MID_4_Y -1079

#define MID_5_X 623
#define MID_5_Y -1079

#include "sensors.h"

void win(bool team);
int getCurrentTerritory(void);
int getGoalTerritory(int x, int y);


struct sumForces{
	float x;
	float y;
};
typedef struct sumForces SumForces;


int getMidX(int territory) {
  if (territory==0)
    return MID_0_X;
  if (territory==1)
    return MID_1_X;
  if (territory==2)
    return MID_2_X;
  if (territory==3)
    return MID_3_X;
  if (territory==4)
    return MID_4_X;
  if (territory==5)
    return MID_5_X;
  else
    return -1;
}

int getMidY(int territory) {
  if (territory==0)
    return MID_0_Y;
  if (territory==1)
    return MID_1_Y;
  if (territory==2)
    return MID_2_Y;
  if (territory==3)
    return MID_3_Y;
  if (territory==4)
    return MID_4_Y;
  if (territory==5)
    return MID_5_Y;
  else
    return -1;
}

SumForces getForces(int targX, int targY){

	SumForces sf;
	sf.x = 0;
	sf.y = 0;
	
	float diffX = 0;
	float diffY = 0;
	float distance = 0;
	float angle = 0;
	float forceMag = 0;

	copy_objects();
	
	int currX = game.coords[0].x;
	int currY = game.coords[0].y;
	
//	printf("Curr X: %i Curr Y: %i\n", currX, currY);
	
	diffX = targX - currX;
	diffY = targY - currY;
	angle = atan2f(diffY,diffX);
	distance = sqrt(diffX*diffX + diffY*diffY);
	
//	printf("Distance: %f\n", distance);
	
	
	
	forceMag = ATTRACTING_FORCE_CONSTANT;
	sf.x += forceMag*cos(angle);
	sf.y += forceMag*sin(angle);
	

	
//	printf("Total Force X: %f Total Force Y: %f\n", sf.x, sf.y);
	
	
	return sf;
}


//returns 0 if successful
//returns 1 if timeout
int moveTo2(int targX, int targY, int maxSpeed, int tolerance, float forwardGain){
	//constants
	const int minSpeed = 60;
	const int maxStationaryDelta = 100;
	const int timeout=4000;
	//gains
	const float turningPGain = 6;
	const float turningDGain = -2;
	
	//initialize variables
	float curAngle = 0;
	float refAngle = 0;
	float deltaAngle = 0;
	float lastDeltaAngle = 0;
	int delta = 0;
	int speed = 0;

	
	//find distance to goal
	int currX = game.coords[0].x;
	int currY = game.coords[0].y;
	float diffX = targX - currX;
	float diffY = targY - currY;
	float distance = sqrt(diffX*diffX + diffY*diffY);
	
	
//	printf("\nGoal x: %i Goal Y: %i\n", targX, targY);
//	printf("Distance: %f\n", distance);
	
	
	int currTerritory = getCurrentTerritory();
	int goalTerritory = getGoalTerritory(targX, targY);
	
//	printf("Current Territory: %i\n", currTerritory);
//	printf("Goal Territory: %i\n", goalTerritory);
	
	int terrDistance = abs(goalTerritory - currTerritory);
//	printf("Territory Distance: %i\n", terrDistance);
	
	if(terrDistance > 1 && terrDistance < 5){
		if(terrDistance !=4 && (currTerritory == 4 || currTerritory == 5)){
			//currTerritory - 1
			moveTo2(getMidX(currTerritory - 1), getMidY(currTerritory - 1), 220, 200, .3);
			
		}else if(terrDistance == 4 && (currTerritory = 0 || currTerritory == 4)){
			//currTeritory = 5
			moveTo2(getMidX(5), getMidY(5), 220, 200, .3);
			
		}else if(terrDistance == 4 && (currTerritory = 5 || currTerritory == 1)){
			//currTerritory = 0
			moveTo2(getMidX(0), getMidY(0), 220, 200, .3);
			
		}else if(terrDistance !=4){
			//currTerritory + 1;
			moveTo2(getMidX(currTerritory + 1), getMidY(currTerritory + 1), 220, 200, .3);
		}
	}
	
	long startTime=get_time();

	
	do{
	
	
		//find the sum of the forces
		SumForces sf = getForces(targX, targY);
//		printf("Force x: %f, Force y: %f\n", sf.x, sf.y);
		
		//calculate distance to the goal
		
		currX = game.coords[0].x;
		currY = game.coords[0].y;
		diffX = targX - currX;
		diffY = targY - currY;
		distance = sqrt(diffX*diffX + diffY*diffY);
		
		//find angle delta
		curAngle = gyroToDegrees();
//		printf("CurAngle: %f\n", curAngle);
		
		if(curAngle < 0){
			curAngle += 360;
		}
		
		refAngle = atan2f(sf.y, sf.x)*RAD_TO_DEG;
//		printf("Force Angle: %f\n", refAngle);
		
		if(refAngle < 0){
			refAngle += 360;
		}
		
		lastDeltaAngle = deltaAngle;
		deltaAngle = refAngle - curAngle;
		
		//make delta 180 -> -180
		if(deltaAngle > 180){
			deltaAngle -= 360;
		}
		if(deltaAngle < -180){
			deltaAngle += 360;
		}
		
//		printf("Delta Angle: %f\n", deltaAngle);
		
		//set the differential in motor speed
		delta = deltaAngle*turningPGain + (lastDeltaAngle - deltaAngle)*turningDGain;
		
		/*
		//make sure delta is above minimum delta
		if(delta < minStationaryDelta && delta > 0 && speed == 0){
			delta = minStationaryDelta;
		}else if(delta > -minStationaryDelta && delta < 0 && speed == 0){
			delta = -minStationaryDelta;
		}*/
		
		//make sure delta is below max delta
		if(delta > maxStationaryDelta){
			delta = maxStationaryDelta;
		}else if(delta < -maxStationaryDelta){
			delta = -maxStationaryDelta;
		}

		//find the base forward speed
		speed = distance*forwardGain;
		
		if(speed > maxSpeed)
			speed = maxSpeed;
		
		
		//adjust speed based on delta Angle
		speed = speed - 5*abs(deltaAngle);
		
		if(speed < minSpeed)
			speed = minSpeed;
		
		
		
//		printf("Delta Motor: %i\n", delta);
		
//		printf("Speed: %i\n\n", speed);
		
		
		
		
		motor_set_vel(LEFT_MOTOR, speed - delta);
		motor_set_vel(RIGHT_MOTOR, speed + delta);
		//pause(1000);
		
		if (get_time()-startTime>timeout){
			stop();
			return 1;
		}
	} while(distance > tolerance);
	
	
	motor_set_vel(LEFT_MOTOR, 0);
	motor_set_vel(RIGHT_MOTOR, 0);
	
	return 0;
}

int exploreMoveTo2(int targX, int targY, int maxSpeed, int tolerance, float forwardGain){
	//constants
	const int minSpeed = 60;
	const int maxStationaryDelta = 100;
	const int timeout=5000;
	//gains
	const float turningPGain = 6;
	const float turningDGain = -2;
	
	//initialize variables
	float curAngle = 0;
	float refAngle = 0;
	float deltaAngle = 0;
	float lastDeltaAngle = 0;
	int delta = 0;
	int speed = 0;
	long startTime=get_time();
	
	//find distance to goal
	int currX = game.coords[0].x;
	int currY = game.coords[0].y;
	float diffX = targX - currX;
	float diffY = targY - currY;
	float distance = sqrt(diffX*diffX + diffY*diffY);
	
	
//	printf("\nGoal x: %i Goal Y: %i\n", targX, targY);
//	printf("Distance: %f\n", distance);
	
	
	do{
	
	
		//find the sum of the forces
		SumForces sf = getForces(targX, targY);
//		printf("Force x: %f, Force y: %f\n", sf.x, sf.y);
		
		//calculate distance to the goal
		
		currX = game.coords[0].x;
		currY = game.coords[0].y;
		diffX = targX - currX;
		diffY = targY - currY;
		distance = sqrt(diffX*diffX + diffY*diffY);
		
		//find angle delta
		curAngle = gyroToDegrees();
//		printf("CurAngle: %f\n", curAngle);
		
		if(curAngle < 0){
			curAngle += 360;
		}
		
		refAngle = atan2f(sf.y, sf.x)*RAD_TO_DEG;
//		printf("Force Angle: %f\n", refAngle);
		
		if(refAngle < 0){
			refAngle += 360;
		}
		
		lastDeltaAngle = deltaAngle;
		deltaAngle = refAngle - curAngle;
		
		//make delta 180 -> -180
		if(deltaAngle > 180){
			deltaAngle -= 360;
		}
		if(deltaAngle < -180){
			deltaAngle += 360;
		}
		
//		printf("Delta Angle: %f\n", deltaAngle);
		
		//set the differential in motor speed
		delta = deltaAngle*turningPGain + (lastDeltaAngle - deltaAngle)*turningDGain;
		
		/*
		//make sure delta is above minimum delta
		if(delta < minStationaryDelta && delta > 0 && speed == 0){
			delta = minStationaryDelta;
		}else if(delta > -minStationaryDelta && delta < 0 && speed == 0){
			delta = -minStationaryDelta;
		}*/
		
		//make sure delta is below max delta
		if(delta > maxStationaryDelta){
			delta = maxStationaryDelta;
		}else if(delta < -maxStationaryDelta){
			delta = -maxStationaryDelta;
		}

		//find the base forward speed
		speed = distance*forwardGain;
		
		if(speed > maxSpeed)
			speed = maxSpeed;
		
		
		//adjust speed based on delta Angle
		speed = speed - 5*abs(deltaAngle);
		
		if(speed < minSpeed)
			speed = minSpeed;
		
		
		
//		printf("Delta Motor: %i\n", delta);
		
//		printf("Speed: %i\n\n", speed);
		
		
		
		
		motor_set_vel(LEFT_MOTOR, speed - delta);
		motor_set_vel(RIGHT_MOTOR, speed + delta);
		//pause(1000);
		
		if (get_time()-startTime>timeout){
			stop();
			return 1;
		}
	} while(distance > tolerance);
	
	
	motor_set_vel(LEFT_MOTOR, 0);
	motor_set_vel(RIGHT_MOTOR, 0);
	
	return 0;
}

void encourage(void){
	copy_objects();
	
	int x = game.coords[0].x;
	int y = game.coords[0].y;
	
	moveTo2(x,y,180,10000, 0.2);
	
	return;
}

void driveBackwards(int time, int speed, float heading){
	long startTime = get_time();
	
	//gain
	float Pgain = 2;
	
	while (get_time() - startTime < time){
		float error = heading - gyroToDegrees();
		
		if(error > 180){
			error -= 360;
		}
		if(error < -180){
			error += 360;
		}
		
		int delta = error*Pgain;
		
		motor_set_vel(LEFT_MOTOR, -speed - delta);
		motor_set_vel(RIGHT_MOTOR, -speed + delta);
	}
	motor_set_vel(LEFT_MOTOR, 0);
	motor_set_vel(RIGHT_MOTOR, 0);
}

void explore(bool direction, bool team){
	
	
	copy_objects();
	//set the gyro to the correct value-copied objects just before explore was called
	
	float VPSheading = game.coords[0].theta*THETA_TO_DEGREES;
	
	if(VPSheading < 0)
		VPSheading+=360;
	
	gyro_set_degrees(VPSheading);
	
	if(direction == false && team == true){
		
		if(exploreMoveTo2(700, 1000, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(-900, 900, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(-900, -900, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(700, -900, 240, 400, 0.3) == 1)
			win(team);
		
	}else if(direction == false && team == false){
		if(exploreMoveTo2(-700, -1000, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(900, -900, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(900, 900, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(-700, 900, 240, 400, 0.3) == 1)
			win(team);
			
	}else if(direction == true && team == true){
	
		if(exploreMoveTo2(700, -1000, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(-900, -900, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(-900, 900, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(300, 900, 240, 400, 0.3) == 1)
			win(team);
			
	}else if(direction == true && team == false){
	
		if(exploreMoveTo2(-700, 1000, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(900, 900, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(900, -900, 240, 400, 0.3) == 1)
			win(team);
		if(exploreMoveTo2(-300, -900, 240, 400, 0.3) == 1)
			win(team);
	
	}
	return;
}	
#endif