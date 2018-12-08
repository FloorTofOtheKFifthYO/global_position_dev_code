#include "control.h"
#define PI 3.14159265358979
#define  PERIMETER (50.5 * PI) //定位轮周长 = D * PI，单位mm
float Xspeed = 0;
float Yspeed = 0;
float pos_5_circle = 30.00;
float neg_5_circle = 30.26;
float xyintersection = 1.5707963 - 0.09;
double Xlength = 0;
double Ylength = 0;
double pos_X = 0;
double pos_Y = 0;
double CarAngleSigma = 0;
float CarAngle = 0;
double CarGyroVal = 0;
double CarAngleSigma2 = 0;
double CarGyroVal2 = 0;
float gyro_offset_cnt = 0;
float adxrs_offset_cnt = 0;
int16_t gy[3];   //三轴陀螺仪的值，分别为 X　Y　Z的角速度
int16_t ac[3];  //XYZ三轴的加速度

static float angle_subtract(float a, float b)
{
    float out = a - b;
    while(out > PI)
	{
		out -= 2 * PI;
	}
	while(out < - PI)
	{
		out += 2 * PI;
	}
    return out;
}

void position_dev_init()
{
    HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
    TIM2 -> CNT = 0x7FFF;
    TIM3 -> CNT = 0x7FFF;
    TIM4 -> CNT = 0x7FFF;
    MPU9250_Init(&MPU9250);
    ADXRS810_Init();
    HAL_Delay(2000);
  
    for(int i = 0; i < 200; i++)
    {
      MPU_Read6500(&MPU9250,ac,gy);
      gyro_offset_cnt+=gy[2]*500/32768;
      HAL_Delay(2);
    }
    
    for(int i = 0; i < 500; i++)
    {
      adxrs_offset_cnt+=ADXRS810_Data();
      HAL_Delay(2);
    }
}

static void position_dev_get_speed()
{    
    int tmp = TIM2 -> CNT - 0x7FFF;
    Ylength = (double)tmp * PERIMETER /2000.0;
    TIM2 -> CNT = 0x7FFF;
    Yspeed = Ylength /10;//换算成米每秒,调节周期是10ms
    
    tmp = TIM4 -> CNT - 0x7FFF; 
    tmp = -tmp;//X相是反的
    Xlength = (double)tmp * PERIMETER /2000.0;
    Xspeed = Xlength / 10;//换算成米每秒
    TIM4 -> CNT = 0x7FFF;
}

static void position_dev_get_angle()
{
    
    MPU_Read6500(&MPU9250,ac,gy);
    CarGyroVal = gy[2]*500/32768;
    CarGyroVal -= gyro_offset_cnt /200;
    CarGyroVal *= (PI/180)*0.01;
    if(CarGyroVal > 0) CarGyroVal *= 10 * PI / pos_5_circle;
    if(CarGyroVal < 0) CarGyroVal *= 10 * PI / neg_5_circle;
    CarAngleSigma = CarAngleSigma + CarGyroVal;

    CarGyroVal2 = ADXRS810_Data();
    CarGyroVal2 -= adxrs_offset_cnt /500;
    CarGyroVal2 /= 80.0;
    CarGyroVal2 *= (PI/180)*0.01;
           //if(CarGyroVal > 0) CarGyroVal *= 10 * PI / pos_5_circle;
             //if(CarGyroVal < 0) CarGyroVal *= 10 * PI / neg_5_circle;
    CarAngleSigma2 = CarAngleSigma2 + CarGyroVal2;
}

void position_dev_cal_xy()
{
    position_dev_get_speed();
    position_dev_get_angle();
    pos_X += Xlength * cos(CarAngleSigma) + Ylength * cos(xyintersection + CarAngleSigma);
    pos_Y += Ylength * cos(CarAngleSigma) + Xlength * cos(xyintersection - CarAngleSigma);
}
