#ifndef TEST_CODE
#define TEST_CODE
#define LEVER_SERVO_PORT 0
#define BASKET_SERVO_PORT 1
#define LEFT_MOTOR   0
#define RIGHT_MOTOR  1
#include <joyos.h>
#include "navigation2.h"
#include "navigation.h"
#include "basket.h"
#include "spinner.h"
#include "lever.h"
#include "sensors.h"

void directionTest1(void){
	faceDirection(30);
	pause(500);
	faceDirection(60);
	pause(500);
	faceDirection(90);
	pause(500);
	faceDirection(120);
	pause(500);
	faceDirection(150);
	pause(500);
	faceDirection(180);
	pause(500);
	faceDirection(210);
	pause(500);
	faceDirection(240);
	pause(500);
	faceDirection(270);
	pause(500);
	faceDirection(300);
	pause(500);
	faceDirection(330);
	pause(500);
	faceDirection(30);
	pause(500);
	
	faceDirection(0);
	pause(500);
	faceDirection(330);
	pause(500);
	faceDirection(300);
	pause(500);
	faceDirection(270);
	pause(500);
	faceDirection(240);
	pause(500);
	faceDirection(210);
	pause(500);
	faceDirection(180);
	pause(500);
	faceDirection(150);
	pause(500);
	faceDirection(120);
	pause(500);
	faceDirection(90);
	pause(500);
	faceDirection(60);
	pause(500);
	faceDirection(30);
	pause(500);
	faceDirection(330);
	pause(500);
	faceDirection(0);
	return;

}

void directionTest2(void){
	faceDirection(160);
	pause(500);
	faceDirection(0);
	pause(500);
	faceDirection(200);
	pause(500);
	faceDirection(90);
	pause(500);
	faceDirection(250);
	pause(500);
	faceDirection(90);
	pause(500);
	faceDirection(290);

	return;
}

void squareTest(void){
	driveStraight(24,0);
	faceDirection(90);
	driveStraight(24,90);
	faceDirection(180);
	driveStraight(24,180);
	faceDirection(270);
	driveStraight(24,270);
	faceDirection(90);
	driveStraight(24,90);
	faceDirection(0);
	driveStraight(24,0);
	faceDirection(270);
	driveStraight(24,270);
	faceDirection(180);
	driveStraight(24,180);
	faceDirection(0);

	return;
}

void spinnerTest(void){
  spinnerOn(true);
  pause(2000);
  spinnerOff();
  pause(1000);
  spinnerOn(false);
  pause(2000);
  spinnerOff();
}


void copyObjectsTest(void){
  copy_objects();
  printf("Initial Variables\n\nPosition X: %i\nPosition Y: %i Position Theta: %i\n\n\n", game.coords[0].x, game.coords[0].y, game.coords[0].theta);
  copy_objects();
  motor_set_vel(LEFT_MOTOR,150);
	motor_set_vel(RIGHT_MOTOR,150);
	pause(1000);
  stop();
  printf("Pre-Drive Straight Variables\n\nPosition X: %i\nPosition Y: %i Position Theta: %i\n\n\n", game.coords[0].x, game.coords[0].y, game.coords[0].theta);
  copy_objects();
  printf("Post-Drive Straight Variables\n\nPosition X: %i\nPosition Y: %i Position Theta: %i\n\n\n", game.coords[0].x, game.coords[0].y, game.coords[0].theta);
  pause(1000);
  copy_objects();
  printf("Post-Pause Variables\n\nPosition X: %i\nPosition Y: %i Position Theta: %i\n\n\n", game.coords[0].x, game.coords[0].y, game.coords[0].theta);
  int tempTime=get_time();
  copy_objects();
  int time=get_time()-tempTime;
  printf("Copy Objects Time: %i", time);
  
}

void leverServoTest(void){
  servo_set_pos(LEVER_SERVO_PORT, 15);
  pause(1000);
  servo_set_pos(LEVER_SERVO_PORT, 500);
  pause(1000);
  servo_set_pos(LEVER_SERVO_PORT, 250);
  return;
}
/*
void leverTest(void){
  leverPull();
  pause(1000);
  leverRelease();
  pause(1000);
  leverPull();
  pause(1000);
  leverRelease();
  pause(1000);
  leverPull();
  pause(1000);
  leverRelease();
  pause(1000);
  return;
 
}
*/
void frobLeverTest(void){
	frob_read_range(15,500);
	while(1){
		printf("Servo Pos: %u\n",frob_read());
		servo_set_pos(LEVER_SERVO_PORT, frob_read());
	}
}

void frobBasketTest(void){
	frob_read_range(15,500);
	while(1){
		printf("Servo Pos: %u\n",frob_read());
		servo_set_pos(BASKET_SERVO_PORT, frob_read());
	}
}

void superTest(void){
  leverFlick();
  leverFlick();
  leverFlick();
  spinnerOn(true);
  pause(1000);
  spinnerOff();
  pause(100);
  spinnerOn(false);
  pause(1000);
  spinnerOff();
  faceDirection(90);
  driveBackwards(500, 100, 90);
  basketTilt();
  emptyBasket();
  basketRaise();
  emptyBasketSlowly();
  basketRaise();
  
  stop();
  
  return;
}
#endif