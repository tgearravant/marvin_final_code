#ifndef ROBOT_NAVIGATION_CODE

#define ROBOT_NAVIGATION_CODE

#include <joyos.h>
#include <math.h>

#define LEFT_MOTOR   0
#define RIGHT_MOTOR  1
#define ENCODER_CONSTANT 30 //counts per full wheel spin
#define CIRCUMFERENCE 10.2
#define ENCODER_PIN 27
#define VPS_PER_FOOT_M1 682.6666
#define VPS_PER_FOOT_M2 443.4
#define PI 3.14159265358979323846
#define THETA_TO_DEGREES 0.087890625
#include "sensors.h"

//Function takes the reference angle from 0 and drives straight in that direction for driveTime milliseconds
//Returns 0 if successful
//Returns 1 if timeout

void stop(void){
	motor_set_vel(LEFT_MOTOR,0);
	motor_set_vel(RIGHT_MOTOR,0);
	return;
}


int driveStraight(int driveTime, float reference){
  const float pGain_angle = 1.5;
  const float dGain_angle = -.4;
  const float iGain_angle = .0025;
  const int defaultStraightSpeed = 100;
  const int deltaLimit = 254 - defaultStraightSpeed;
  const int minSpeed = 80;
  const int timeout = 7000;
  
  long startTime = get_time();
  
  float lastError = 0;
  float angleError = 0;
  float intError = 0;
  float p_angle = 0;
  float d_angle = 0;
  float i_angle = 0;
  int angleDelta = 0;
  int speed = 0;
    
  //int encoderCounts = driveDistance*ENCODER_CONSTANT/CIRCUMFERENCE;  
  
  encoder_reset(ENCODER_PIN);
  
  
  while(get_time() - startTime < driveTime){

    //angular error correcting code
  	angleError = gyroToDegrees() - reference;
  	if(angleError > 180)
	   	angleError = angleError - 360;
	
	  if(angleError < -180)
		  angleError = angleError + 360;
	
	
	intError += angleError;
	
  	p_angle = pGain_angle*angleError;
  	d_angle = dGain_angle*(lastError - angleError);
  	i_angle = iGain_angle*intError;
  	angleDelta = p_angle + d_angle + i_angle;
  	
  	//Correction limiter code to prevent motor values from being too large in abs value
  	if(angleDelta > deltaLimit)
  		angleDelta = deltaLimit;
  	
  	if(angleDelta < -deltaLimit)
  		angleDelta = -deltaLimit;
  	
	
	  //speed correcting code
	  //int slowDownRate = encoderCounts-encoder_read(ENCODER_PIN)-slowDownRadius*ENCODER_CONSTANT/CIRCUMFERENCE;
	  
	  //speed = defaultStraightSpeed + pGain_speed*slowDownRate;
	  speed = defaultStraightSpeed;
    if(speed > defaultStraightSpeed)
   		speed = defaultStraightSpeed;
  	
  	if(speed < minSpeed)
	   	speed = minSpeed;
  	
	
  	//Set the motors to move forward with the error correction
  	motor_set_vel(LEFT_MOTOR,speed + angleDelta);
  	motor_set_vel(RIGHT_MOTOR,speed - angleDelta);
  	lastError = angleError; 
  	if (get_time()-startTime>timeout){
		stop();
		return 1;
	}
  }
	stop();

  
  return 0;
}

//Returns 0 if successful
//Returns 1 if timeout
int turnLeft(float reference){
    const int timeout = 2000;
    long startTime=get_time();
  
	while((gyroToDegrees()-reference > 40 || gyroToDegrees() - reference < -40)){
		motor_set_vel(LEFT_MOTOR,-80);
		motor_set_vel(RIGHT_MOTOR,80);
    if (get_time()-startTime>timeout){
		stop();
		return 1;
	}
	}
	while((gyroToDegrees()-reference > 5 || gyroToDegrees() - reference < -5)){
		motor_set_vel(LEFT_MOTOR,-60);
		motor_set_vel(RIGHT_MOTOR,60);
    if (get_time()-startTime>timeout){
		stop();
		return 1;  
	}
	}
	stop();
	
  return 0;
}

//Returns 0 if successful
//Returns 1 if timeout
int turnRight(float reference){
    const int timeout = 2000;
  	long startTime=get_time();
	
	while((gyroToDegrees()-reference > 40 || gyroToDegrees() - reference < -40)){
		motor_set_vel(LEFT_MOTOR,80);
		motor_set_vel(RIGHT_MOTOR,-80);
    if (get_time()-startTime>timeout){
		stop();
		return 1;
	}
	}
	while((gyroToDegrees()-reference > 5 || gyroToDegrees() - reference < -5)){
		motor_set_vel(LEFT_MOTOR,60);
		motor_set_vel(RIGHT_MOTOR,-60);
    if (get_time()-startTime>timeout){
		stop();
		return 1;
	}
	}
	stop();

  return 0;
}
//Returns 0 if successful
//Returns 1 if failure
int faceDirection(float reference){

	float curHeading = gyroToDegrees();
	
	if(curHeading < 180){
		if(curHeading + 180 > reference && reference > curHeading){
			return turnLeft(reference);
		}else{
			return turnRight(reference);
		}
	}else{
		if(curHeading - 180 < reference && reference < curHeading){
			return turnRight(reference);
		}else{
			return turnLeft(reference);
		}
		
	}
}


void initialExploration(void){
  return;
}



void moveTo(int targX, int targY){
	pause(200);
	copy_objects();
	gyro_set_degrees(game.coords[0].theta*THETA_TO_DEGREES);
	float currX = game.coords[0].x/VPS_PER_FOOT_M2;
	float currY = game.coords[0].y/VPS_PER_FOOT_M2;
	float diffX = targX - currX;
	float diffY = targY - currY;
	int distance = sqrt(diffX*diffX + diffY*diffY)*12; //in inches
	float angle = atan2f(diffY,diffX)*180/PI;
	
	if(angle < 0){
		angle += 360;
	}
	
	
	faceDirection(angle);
	
	driveStraight(distance, angle);
	
	return;
}
#endif