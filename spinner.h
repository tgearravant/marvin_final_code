#ifndef ROBOT_SPINNNER_CODE

#define ROBOT_SPINNNER_CODE

#include <joyos.h>  
#include "navigation.h"
#include "navigation2.h"

#define SPINNER_MOTOR_PORT 2
#define SPINNER_SPEED   240

//400 distance perpendicular to wall
//#define SPINNER_0_X	1791
//#define SPINNER_0_Y	443
//#define SPINNER_0_X		1445
//#define SPINNER_0_Y 	243
#define SPINNER_0_THETA 210
#define SPINNER_0_X 1389
#define SPINNER_0_Y 192

//400 distance perpendicular to wall
//#define SPINNER_1_X	512
//#define SPINNER_1_Y	1773
//#define SPINNER_1_X     512
//#define SPINNER_1_Y     1373
#define SPINNER_1_THETA 270
#define SPINNER_1_X 540
#define SPINNER_1_Y 1288

//400 distance perpendicular to wall
//are always too low, move positive Y
//#define SPINNER_2_X	-1280
//#define SPINNER_2_Y	1330
//#define SPINNER_2_X     -934
//#define SPINNER_2_Y     1170
#define SPINNER_2_THETA 330
#define SPINNER_2_X -840
#define SPINNER_2_Y 1114

//400 distance perpendicular to wall
//#define SPINNER_3_X	-1791
//#define SPINNER_3_Y	-443
//#define SPINNER_3_X     -1445
//#define SPINNER_3_Y     -243
#define SPINNER_3_THETA 30
#define SPINNER_3_X -1440
#define SPINNER_3_Y -183

//400 distance perpendicular to wall
//#define SPINNER_4_X	-512
//#define SPINNER_4_Y	-1773
//#define SPINNER_4_X     -512
//#define SPINNER_4_Y     -1373
#define SPINNER_4_THETA 90
#define SPINNER_4_X -557
#define SPINNER_4_Y -1388

//400 distance perpendicular to wall
//#define SPINNER_5_X 	1280
//#define SPINNER_5_Y	-1330
//#define SPINNER_5_X     934
//#define SPINNER_5_Y     -1130
#define SPINNER_5_THETA 150
#define SPINNER_5_X 840
#define SPINNER_5_Y -1120


void spinnerOn(bool direction){
  if (direction){
    motor_set_vel(SPINNER_MOTOR_PORT,-SPINNER_SPEED);
  }
  else{
    motor_set_vel(SPINNER_MOTOR_PORT,SPINNER_SPEED);
  }  
  return;
}

void spinnerOff(void){
  motor_set_vel(SPINNER_MOTOR_PORT,0);
  return;
}


int getSpinnerX(int territory){
  if (territory==0)
    return SPINNER_0_X;
  if (territory==1)
    return SPINNER_1_X;
  if (territory==2)
    return SPINNER_2_X;
  if (territory==3)
    return SPINNER_3_X;
  if (territory==4)
    return SPINNER_4_X;
  if (territory==5)
    return SPINNER_5_X;
  else
    return -1;
}
int getSpinnerY(int territory){
  if (territory==0)
    return SPINNER_0_Y;
  if (territory==1)
    return SPINNER_1_Y;
  if (territory==2)
    return SPINNER_2_Y;
  if (territory==3)
    return SPINNER_3_Y;
  if (territory==4)
    return SPINNER_4_Y;
  if (territory==5)
    return SPINNER_5_Y;
  else
    return -1;
}
float getSpinnerTheta(int territory){
  if (territory==0)
    return SPINNER_0_THETA;
  if (territory==1)
    return SPINNER_1_THETA;
  if (territory==2)
    return SPINNER_2_THETA;
  if (territory==3)
    return SPINNER_3_THETA;
  if (territory==4)
    return SPINNER_4_THETA;
  if (territory==5)
    return SPINNER_5_THETA;
  else
    return -1;
}

//returns true if we own the territory
//returns false otherwise
bool isCaptured(int territory){
  copy_objects();
  if (game.territories[territory].owner==3)
    return true;
  else
    return false;
}


//returns 0 if successful
//returns 1 if cannot capture territory
//returns 2 if cannot get to territory
//returns 3 if cannot face direction
//returns 4 if less than 10 sec in round
int capture(int territory, bool direction){
  int timeout=3000; 
  const int attempts=3;
  int spinnerX = getSpinnerX(territory);
  int spinnerY = getSpinnerY(territory);
  float spinnerTheta = getSpinnerTheta(territory);

  encourage();
  
  if(isCaptured(territory)){
	return 0;
  }
  
  if(get_time() - masterStartTime > 105000 && caughtBalls > 0)
	return 4;
  
  
  for (int i=0;moveTo2(spinnerX, spinnerY, 200, 100, 0.2)!=0; i++){
    motor_set_vel(LEFT_MOTOR,-150);
    motor_set_vel(RIGHT_MOTOR,-150);
    pause(300);
	stop();
    encourage();
    if (i>attempts)
      return 2;  
  }
  
  for (int i=0;faceDirection(spinnerTheta)!=0; i++){
	motor_set_vel(LEFT_MOTOR,150);
    motor_set_vel(RIGHT_MOTOR,150);
    pause(300);
    if (i>attempts)
      return 3;  
  }
	
	spinnerOn(direction);
	
	//move backwards
	driveBackwards(1000, 80, spinnerTheta);
	
  
  long startTime=get_time();
  if (game.territories[territory].owner==0)
	timeout=2000;
  else
	timeout=4000;
  while(!isCaptured(territory)){//territory is not captured
    if (get_time()-startTime>timeout){
      spinnerOff();
	    motor_set_vel(LEFT_MOTOR,150);
		motor_set_vel(RIGHT_MOTOR,150);
		pause(300);
		stop();
	  return 1;  
	  }
  }
  spinnerOff();
  motor_set_vel(LEFT_MOTOR,100);
  motor_set_vel(RIGHT_MOTOR,100);
  pause(300);
  stop();

  return 0;
}
#endif