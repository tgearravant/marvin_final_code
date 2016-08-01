#ifndef ROBOT_STRATEGY_CODE

#define ROBOT_STRATEGY_CODE

#include <math.h>
#include "basket.h"
#include "spinner.h"
#include "lever.h"

#define TERRITORY_0_X 1000
#define TERRITORY_0_Y 0

#define TERRITORY_1_X 623
#define TERRITORY_1_Y 900

#define TERRITORY_2_X -600
#define TERRITORY_2_Y 900

#define TERRITORY_3_X -1000
#define TERRITORY_3_Y  0

#define TERRITORY_4_X -600
#define TERRITORY_4_Y -900

#define TERRITORY_5_X 600
#define TERRITORY_5_Y -900

#define RAD_TO_DEG 57.29577951

int clearTerritory(int territory, bool direction, int entryPoint){
  int captureErrorCode=0;
  int mineErrorCode=0;
  int mineBallCount=0;
 
  if (entryPoint <= 0){  
    captureErrorCode=capture(territory, direction);
    if (captureErrorCode==1){
      capture(territory, direction);
    }
	if (captureErrorCode==2)
      pause(10);
    if (captureErrorCode==3)
      pause(10); 
	if(captureErrorCode == 4)
		return 0;
  }
  if(entryPoint<=1){
    copy_objects();
    mineBallCount = game.territories[territory].remaining;
    mineErrorCode=mine2(territory);
	
    if (mineErrorCode==1){
		motor_set_vel(LEFT_MOTOR, -100);
		motor_set_vel(RIGHT_MOTOR, -100);
		pause(1300);
		stop();
		if(mine2(territory) != 0){
			motor_set_vel(LEFT_MOTOR, -100);
			motor_set_vel(RIGHT_MOTOR, -100);
			pause(1300);
			stop();
		}
    }if (mineErrorCode==2)
      pause(10);
    if (mineErrorCode==3)
      pause(10);
    if (mineErrorCode==4)
      pause(10);
	if(mineErrorCode == 5)
		return 0;
  }
  copy_objects();
  caughtBalls = caughtBalls + (mineBallCount - game.territories[territory].remaining);
  
  
  return 0;
}



int getCurrentTerritory(void){
	copy_objects();
	int x = game.coords[0].x;
	int y = game.coords[0].y;
	
	int currTerritory = 0;
	
	float theta = atan2f(y,x)*RAD_TO_DEG;
	
	if(theta < 0)
		theta +=360;
	
	if(theta >= 30 && theta < 90){
		currTerritory = 1;
	}else if(theta >= 90 && theta < 150){
		currTerritory = 2;
	}else if(theta >= 150 && theta < 210){
		currTerritory = 3;
	}else if(theta >= 210 && theta < 270){
		currTerritory = 4;
	}else if(theta >= 270 && theta < 330){
		currTerritory = 5;
	}else{
		currTerritory = 0;
	}
	
	return currTerritory;
}

int getGoalTerritory(int x, int y){

	int goalTerritory = 0;
	
	
	float theta = atan2f(y,x)*RAD_TO_DEG;
	
	if(theta < 0)
		theta +=360;
		
	if(theta >=30 && theta < 90){
		goalTerritory = 1;
	}else if(theta >= 90 && theta < 150){
		goalTerritory = 2;
	}else if(theta >= 150 && theta < 210){
		goalTerritory = 3;
	}else if(theta >= 210 && theta < 270){
		goalTerritory = 4;
	}else if(theta >= 270 && theta < 330){
		goalTerritory = 5;
	}else{
		goalTerritory = 0;
	}
	
	return goalTerritory;
}

int territoryDistance(int x1, int y1, int x2, int y2){
	int territory1 = getGoalTerritory(x1, y1);
	int territory2 = getGoalTerritory(x2,y2);
	
	int terrDistance = abs(territory2 - territory1);
	
	if(terrDistance == 5){
		terrDistance = 1;
	}else if(terrDistance == 4){
		terrDistance = 2;
	}
	return terrDistance;
}

/*
void win2(bool spinnerDirection){
  const long timeThreshold = 85000;
  const int ballThreshold = 26;
  long startTime=get_time();
  int caughtBalls=0;
  int dumpErrorCode=0;
  bool canDump = true;
  bool canMine = true;
  bool canSpin = true;
  
  encourage();
  //explore(spinnerDirection);
  
  int i=-getCurrentTerritory();
  
  while (canSpin && canMine){
    for (; (i>=-5 && canSpin && canMine) ;i++){
	
      //clearTerritory(-i, spinnerDirection, 0, &caughtBalls);
	  
	  clearTerritory(2, spinnerDirection, 0, &caughtBalls);
	  clearTerritory(1, spinnerDirection, 0, &caughtBalls);
	  clearTerritory(0, spinnerDirection, 0, &caughtBalls);
	  dump(0, spinnerDirection);
	  clearTerritory(5, spinnerDirection, 0, &caughtBalls);
	  clearTerritory(4, spinnerDirection, 0, &caughtBalls);
	  clearTerritory(3, spinnerDirection, 0, &caughtBalls);
	  
      if ((caughtBalls>ballThreshold||get_time()-startTime>timeThreshold) && canDump){
        //dumpErrorCode=dump(getCurrentTerritory());
		dumpErrorCode = dump(3, spinnerDirection);
        if (dumpErrorCode==1)
          pause(10);
        if (dumpErrorCode==2)
          pause(10);
        if (dumpErrorCode==3)
          pause(10);           
        if (dumpErrorCode==0)
          caughtBalls=0;
      }
      if ( (get_time()-startTime>timeThreshold) && caughtBalls==0 )
        break;
    }
    i=0;
  }
  if(canSpin){
    for (int i=0; i<=5; i++){
	  copy_objects();
      if (game.territories[i].owner!=3){
        capture(i, spinnerDirection);
      }
    }
  }
}
*/

void win(bool team){
	const long timeThreshold = 90000;
	const int ballThreshold = 26;
	long startTime = get_time();
	int dumpErrorCode = 0;
	
	int Score[6] = {0,0,0,0,0,0};
	
	int bestTerritory;
	
	pause(300);
	copy_objects();
	
	gyro_set_degrees(game.coords[0].theta*THETA_TO_DEGREES);
	
	if(team == true){
		clearTerritory(5, team, 0);
	}else{
		clearTerritory(2, team, 0);
	}
	
	while(1){
		
		bestTerritory = 0;
		
		
		
		
		//calculate the score of each territory
		for(int territory = 0; territory <= 5; territory++){
		
			Score[territory] = 0;
			
			//if we don't own it, plus 100
			if(game.territories[territory].owner !=3){
				Score[territory] += 100;
			}
			
			//plus 60 for each ball that can be mined, or plus 300 if the counter is near 0
			if(game.territories[territory].remaining > 0 && game.territories[territory].rate_limit > 5){
				Score[territory] += game.territories[territory].remaining*60;
			}else if(game.territories[territory].rate_limit < 5){
				Score[territory] += 300;
			}
			
			copy_objects();
			//minus 70 for each territory away
			Score[territory] -= territoryDistance(game.coords[0].x, game.coords[0].y, getMidX(territory), getMidY(territory))*70;
			
			
			
			if(game.territories[territory].owner == 3 && game.territories[territory].remaining == 0 && game.territories[territory].rate_limit > 5){
				Score[territory] -=10000;
			}
			
			//minus 399 if the other robot is in the territory
			if(getGoalTerritory(game.coords[1].x, game.coords[1].y) == territory){
				Score[territory] -= 399;
			}
			
			if(territoryDistance(getMidX(territory), getMidY(territory), game.coords[1].x, game.coords[1].y) == 1){
				Score[territory] -= 80;
			}
			
//			printf("\nTerritory: %i Score: %i\n", territory, Score[territory]);
		}
		
		//find the maximum score
		
		if(Score[1] > Score[bestTerritory])
			bestTerritory = 1;
		if(Score[2] > Score[bestTerritory])
			bestTerritory = 2;
		if(Score[3] > Score[bestTerritory])
			bestTerritory = 3;
		if(Score[4] > Score[bestTerritory])
			bestTerritory = 4;
		if(Score[5] > Score[bestTerritory])
			bestTerritory = 5;
		
//		printf("Best Territory: %i\n", bestTerritory);
		
		
		//clear that territory - change entry point?
		clearTerritory(bestTerritory, team, 0);
		
		//check if we need to dump - after timeThreshold, we will dump after each territory
		if(caughtBalls > ballThreshold || (get_time() - startTime > timeThreshold && caughtBalls > 0)){
			
			dumpErrorCode = preDump(team);
			
			if(dumpErrorCode == 0){
				caughtBalls = 0;
			}else{
				digital_write(0,1);
				dump(getCurrentTerritory(), team);
				
			}
		}
	}
}
#endif