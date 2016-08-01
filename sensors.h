#ifndef ROBOT_SENSOR_CODE

#define ROBOT_SENSOR_CODE

#include <joyos.h>
#include <math.h>

#define GYRO_PORT       20
#define LSB_US_PER_DEG  1453135
#define VPS_PER_FOOT_M1 682.6666
#define VPS_PER_FOOT_M2 443.4
#define THETA_TO_DEGREES 0.087890625

struct pose{
	int x;
	int y;
	int theta;
};
typedef struct pose Pose;

void gyroInit(void){
  printf("\nPlace robot,    press go.");
  go_click ();
  printf ("\nStabilizing...");
  pause (1000);
  printf ("\nCalibrating     offset...\n");
  gyro_init (GYRO_PORT, LSB_US_PER_DEG, 2000);
  return;
}




/*
void getLocation(){
  const float encGain = 1;
  
  float x = vps_data.x;
  float y = vps_data.y:
  float dist = 0;

//Update position based on sensors
  dist = encoder_read(ENC_PORT)*encGain; //read encoder since last check
  encoder_reset(ENC_PORT); //reset encoder before next check
  //use old heading and encoder to update
  x = x + dist*cos(theta);
  y = y + dist*sin(theta);
  theta = gyro_get_degrees() % 360; //update heading
  return;
}

 */
/*void biasGyro(){
  
  int startTime = get_time();

  float times[6] = [0,0,0,0,0,0];
  float x_coords[6] = [0,0,0,0,0,0];
  float y_coords[6] = [0,0,0,0,0,0];

  while((get_time()-startTime)<2000){
    int i = 5;
    while(i>0){
      times[i] = times[(i-1)];
      x_coords[i] = x_coords[(i-1)];
      y_coords[i] = y_coords[(i-1)];
      i = i-1;
    }
    x_coords[0] = vps_data.x;
    y_coords[0] = vps_data.y;
    times[0] = vps_data.timestamp;
    pause(200);
  }
  


*/

float gyroToDegrees(){
  float normalized=0;
  float gyroRead = gyro_get_degrees();
  float gyroDec = gyroRead - (int)gyroRead;
  if (gyroRead>0) {
    float normalized = (int)gyroRead%360;
    return (normalized+gyroDec);
  } if (gyroRead<0){
    float normalized= -(int)gyroRead%360;
    return 360-(normalized-gyroDec);
  }
  return (normalized+gyroDec);
}


float degreesToGyro(float degrees){
  int gyroMultiplier = 0;
  bool gyroSign = true;
  float gyroRead = gyro_get_degrees();
  printf("Gyro Read: %f\n", gyroRead);
  //put the sign of the value in a bool and remove the sign 
  if (degrees < 0) {
    gyroSign=false;  
    gyroRead*=-1;
  }
  //Find out how many times the gyro has gone around
  gyroMultiplier = (int)gyroRead/360;
  printf("Gyro Multiplier: %d\n\n", gyroMultiplier);
  if (!gyroSign){
    return 360*(-1*gyroMultiplier-1)+degrees;
  }else{
    return 360*gyroMultiplier+degrees;
  }
}


float vpsToDegrees(int vpsAngle){
  float vpsTheta = vpsAngle*THETA_TO_DEGREES;
  if (vpsTheta<0){
    vpsTheta+=360;
  }
  return vpsTheta;
}


Pose getVPSData(){
	Pose pose;
	copy_objects();
	pose.x = game.coords[0].x;
	pose.y = game.coords[0].y;
	
	int VPSheight_in = 129; //in inches
	int VPSheight = VPSheight_in/12*VPS_PER_FOOT_M2;
	
	int robot_height_in = 10; //in inches
	int robot_height = robot_height_in/12*VPS_PER_FOOT_M2;
	
	float proportion = robot_height / VPSheight;
	
	pose.x = pose.x - pose.x*proportion;
	pose.y = pose.y - pose.y*proportion;
	
	return pose;
}




#endif