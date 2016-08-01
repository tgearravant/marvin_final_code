#ifndef ROBOT_LEVER_CODE

#define ROBOT_LEVER_CODE

#include "navigation2.h"
#include "navigation.h"

#define LEVER_PORT 0
#define LEVER_CLOSED 540
#define LEVER_OPEN 206
#define LEVER_HALF_OPEN 310

//340 distance perpendicular to wall
//#define LEVER_0_X	1791
//#define LEVER_0_Y	-443
#define LEVER_0_X		1497
#define LEVER_0_Y		-273
#define LEVER_0_THETA 330
//#define LEVER_0_X 1536
//#define LEVER_0_Y -294

//340 distance perpendicular to wall
//#define LEVER_1_X	1280
//#define LEVER_1_Y	1330
#define LEVER_1_X		986
#define LEVER_1_Y		1160
#define LEVER_1_THETA 30
//#define LEVER_1_X 994
//#define LEVER_1_Y 1179

//340 distance perpendicular to wall
//#define LEVER_2_X	-512
//#define LEVER_2_Y	1773
#define LEVER_2_X		-512
#define LEVER_2_Y		1433
#define LEVER_2_THETA 90
//#define LEVER_2_X -519
//#define LEVER_2_Y 1460

//340 distance perpendicular to wall
//#define LEVER_3_X	-1791
//#define LEVER_3_Y	443
#define LEVER_3_X		-1497
#define LEVER_3_Y		273
#define LEVER_3_THETA 150
//#define LEVER_3_X -1536
//#define LEVER_3_Y 274

//340 distance perpendicular to wall
//#define LEVER_4_X	-1280
//#define LEVER_4_Y	-1330
#define LEVER_4_X     -986
#define LEVER_4_Y     -1160
#define LEVER_4_THETA 210
//#define LEVER_4_X -1022
//#define LEVER_4_Y -1200

//340 distance perpendicular to wall
//moved to positive x
//#define LEVER_5_X	512
//#define LEVER_5_Y	-1773
#define LEVER_5_X		532
#define LEVER_5_Y     -1433
#define LEVER_5_THETA 270
//#define LEVER_5_X 490
//#define LEVER_5_Y -1484

#include <joyos.h>  

void leverClose(void){
  servo_set_pos(LEVER_PORT, LEVER_CLOSED);
  return;
}

void leverOpen(void){
  servo_set_pos(LEVER_PORT, LEVER_OPEN);
  return;
}

void leverHalfOpen(void){
	servo_set_pos(LEVER_PORT, LEVER_HALF_OPEN);
	return;
}

void leverFlick(void){
  leverHalfOpen();
  pause(350);
  leverClose();
  pause(350);

}

int getLeverX(int territory) {
  if (territory==0)
    return LEVER_0_X;
  if (territory==1)
    return LEVER_1_X;
  if (territory==2)
    return LEVER_2_X;
  if (territory==3)
    return LEVER_3_X;
  if (territory==4)
    return LEVER_4_X;
  if (territory==5)
    return LEVER_5_X;
  else
    return -1;
}

int getLeverY(int territory) {
  if (territory==0)
    return LEVER_0_Y;
  if (territory==1)
    return LEVER_1_Y;
  if (territory==2)
    return LEVER_2_Y;
  if (territory==3)
    return LEVER_3_Y;
  if (territory==4)
    return LEVER_4_Y;
  if (territory==5)
    return LEVER_5_Y;
  else
    return -1;
}

int getLeverTheta(int territory) {
  if (territory==0)
    return LEVER_0_THETA;
  if (territory==1)
    return LEVER_1_THETA;
  if (territory==2)
    return LEVER_2_THETA;
  if (territory==3)
    return LEVER_3_THETA;
  if (territory==4)
    return LEVER_4_THETA;
  if (territory==5)
    return LEVER_5_THETA;
  else
    return -1;
}

int mineQuantity(int territory){
  copy_objects();
  int quantity = 0;
  //get quantity from vps
  return quantity;
}


bool exhaustMine (int territory){ //assumes that you are already in place for the lever pulls
  copy_objects();
  //int attempts=game.territories[territory].remaining+2;
  int attempts = 10;
  //if (game.territories[territory].remaining == 0)
    //return true;
	
  //get quantity remaining in mine territory from VPS
  //for(int i=0; game.territories[territory].rate_limit==0; i++){
  /*for(int i = 0; i <=attempts; i++){
    leverFlick();
    copy_objects();
    if (i>attempts)
      return false;
  }*/
  
  int counter = 0;
  
	while(game.territories[territory].remaining > 0){
		//printf("Rate Limit: %i\n", game.territories[territory].rate_limit);
		//printf("Remaining: %i\n", game.territories[territory].remaining);
		leverFlick();
		copy_objects();
		counter++;
		if(counter == attempts){
			break;
		}
	}
	
  return true;
}




//Returns 0 if success
//Returns 1 if cannot pull lever
//Returns 2 if territory is not owned
//Returns 3 if territory has a timer of more than 5 seconds remaining
//Returns 4 if territory has no balls left
//Returns 5 if less than 10 sec left in round
/*
int mine(int territory){

  const int attempts=3;

  copy_objects();
  if (game.territories[territory].owner != 3)
    return 2;
  if (game.territories[territory].rate_limit > 5)
    return 3;
  if (game.territories[territory].remaining == 0)
    return 4;
	
  if(get_time() - masterStartTime > 105000 && caughtBalls > 0)
	return 5;
	
  int initialBallCount = game.territories[territory].remaining;
  int mineX=getLeverX(territory);
  int mineY=getLeverY(territory);
  int mineTheta=getLeverTheta(territory);
    
  for (int i=0;moveTo2(mineX, mineY, 240, 50, 0.2)!=0; i++){
    motor_set_vel(LEFT_MOTOR,-100);
    motor_set_vel(RIGHT_MOTOR,-100);
    pause(1000);
    encourage();
    if (i>attempts)
      return 2;  
  }
  
  leverHalfOpen();

  
  int faceDirectionErrorCode1 = faceDirection(mineTheta - 25);
  
  if(faceDirectionErrorCode1 == 1){
	motor_set_vel(LEFT_MOTOR,-100);
	motor_set_vel(RIGHT_MOTOR,-100);
	pause(100);
	stop();
	faceDirection(mineTheta - 25);
  }
  
  
  int faceDirectionErrorCode2 = faceDirection(mineTheta + 15);

  if(faceDirectionErrorCode2 == 1){
	motor_set_vel(LEFT_MOTOR,-100);
	motor_set_vel(RIGHT_MOTOR,-100);
	pause(100);
	stop();
	faceDirection(mineTheta + 15);
  }
  
  
  leverClose();
  leverFlick();
  leverFlick();
  
  copy_objects();
  
  //set the gyro to the correct value
  gyro_set_degrees(game.coords[0].theta*THETA_TO_DEGREES);
  
  if (game.territories[territory].remaining == initialBallCount)
    return 1;
	
  exhaustMine(territory);
  
  motor_set_vel(LEFT_MOTOR,70);
  motor_set_vel(RIGHT_MOTOR,70);
  pause(300);
  motor_set_vel(LEFT_MOTOR,-60);
  motor_set_vel(RIGHT_MOTOR,-70);
  pause(800);
  stop();
  
  return 0;
}
*/

int mine2(int territory){
	const int attempts=3;
	
	copy_objects();
	
	if(game.territories[territory].owner != 3)
		return 2;
	if(game.territories[territory].rate_limit > 5)
		return 3;
	if(game.territories[territory].remaining == 0)
    return 4;
	
	if(get_time() - masterStartTime > 105000 && caughtBalls > 0)
		return 5;
	
	int initialBallCount = game.territories[territory].remaining;
	
	int mineTheta=getLeverTheta(territory);
    
	for (int i=0;moveTo2(getMidX(territory), getMidY(territory), 240, 50, 0.18)!=0; i++){
		motor_set_vel(LEFT_MOTOR,-100);
		motor_set_vel(RIGHT_MOTOR,-100);
		pause(500);
		encourage();
		if (i>attempts)
			return 2;  
	}
	
	faceDirection(mineTheta);
	
	driveStraight(1000, mineTheta);
	
	motor_set_vel(LEFT_MOTOR,-100);
	motor_set_vel(RIGHT_MOTOR,-100);
	pause(125);
	stop();
	
	leverHalfOpen();

  
	int faceDirectionErrorCode1 = faceDirection(mineTheta - 25);
  
	if(faceDirectionErrorCode1 == 1){
		motor_set_vel(LEFT_MOTOR,-100);
		motor_set_vel(RIGHT_MOTOR,-100);
		pause(125);
		stop();
		faceDirection(mineTheta - 25);
	}
  
  
	int faceDirectionErrorCode2 = faceDirection(mineTheta + 5);
	
	if(faceDirectionErrorCode2 == 1){
		motor_set_vel(LEFT_MOTOR,-100);
		motor_set_vel(RIGHT_MOTOR,-100);
		pause(125);
		stop();
		faceDirection(mineTheta + 10);
	}
	
	
	leverClose();
	leverFlick();
	leverFlick();
	
	copy_objects();
	
	//set the gyro to the correct value
	gyro_set_degrees(game.coords[0].theta*THETA_TO_DEGREES);
	
	if (game.territories[territory].remaining == initialBallCount)
		return 1;
		
	exhaustMine(territory);
	
	motor_set_vel(LEFT_MOTOR,100);
	motor_set_vel(RIGHT_MOTOR,100);
	pause(200);
	motor_set_vel(LEFT_MOTOR,-100);
	motor_set_vel(RIGHT_MOTOR,-100);
	pause(800);
	stop();
	
	return 0;
}

#endif