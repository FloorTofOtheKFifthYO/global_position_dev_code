#ifndef __CONTROL_H__
#define __CONTROL_H__
#ifdef __cplusplus
 extern "C" {
#endif
#include "tim.h"
#include "mpu9250.h"
#include "math.h"
#include "ADXRS810.h"
     
extern float Xspeed;
extern float Yspeed;
extern double pos_X;
extern double pos_Y;
extern double CarAngleSigma;
extern double CarGyroVal;
extern double CarAngleSigma2;
extern double CarGyroVal2;
extern float xyintersection;
    
extern void position_dev_init() ;
extern void position_dev_cal_xy();
     
static void position_dev_get_speed();
static void position_dev_get_angle();
static float angle_subtract(float a, float b);
     
#ifdef __cplusplus
}
#endif     
#endif