//Dhwaj Vandankumar Jani

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ev3.h>
#include <ev3_sound.h>
#define distance 80
#define speed 10
volatile int last_angle;
int turned = 0;

void goal()
{
    Off(OUT_BC);
    PlayTone(NOTE_WHOLE, 2000);
    while(1)
    {
        OnFwdReg(OUT_A,12);
        Wait(SEC_1);
        OnRevReg(OUT_A,12);
        Wait(SEC_1);
        if(readSensor(IN_4))
            break;
    }
}


void turnleft()
{
    OnRevSync(OUT_BC,7);
    Wait(2000);
    if(readSensor(IN_3) > 1)
        goal();
    while((readSensor(IN_1) != (last_angle-90)))
    {
        if(readSensor(IN_1) < (last_angle-90))
        {
            OnFwdReg(OUT_B,25);
            OnRevReg(OUT_C,25);
            Wait(100);
            Off(OUT_BC);
        }
        if(readSensor(IN_1) > (last_angle-90))
        {
            OnFwdReg(OUT_C,25);
            OnRevReg(OUT_B,25);
            Wait(50);
            Off(OUT_BC);
        }
    }
    last_angle = last_angle - 90;
    Off(OUT_BC);
    if(readSensor(IN_3) > 1)
        goal();
    LcdClean();
    LcdPrintf(1, "\r angle = %d",readSensor(IN_1));
    Wait(300);
}

void turnright()
{
    OnFwdSync(OUT_BC,11);
    Wait(2800);
    if(readSensor(IN_3) > 1)
        goal();
    while((readSensor(IN_1) != (last_angle+90)))
    {
        if(readSensor(IN_1) < (last_angle+90))
        {
            OnFwdReg(OUT_B,25);
            OnRevReg(OUT_C,25);
            Wait(50);
            Off(OUT_BC);
        }
        if(readSensor(IN_1) > (last_angle+90))
        {
            OnFwdReg(OUT_C,25);
            OnRevReg(OUT_B,25);
            Wait(50);
            Off(OUT_BC);
        }
    }
    OnFwdSync(OUT_BC,10);
    Wait(3000);
    if(readSensor(IN_3) > 1)
        goal();
    
    last_angle = last_angle + 90;
    Off(OUT_BC);
    LcdClean();
    LcdPrintf(1, "\r angle = %d",readSensor(IN_1));
    Wait(300);
}

int main(void)
{
    InitEV3();
    SoundInit();
    setAllSensorMode(GYRO_ANG,US_DIST_MM,COL_COLOR,TOUCH_PRESS);
    int init_angle = readSensor(IN_1);
    last_angle = init_angle;
    LcdClean();
    LcdPrintf(1, "\r angle = %d",readSensor(IN_1));
    Wait(100);
    while(1)
    {
        if(readSensor(IN_4))
            turnleft();
        if(readSensor(IN_3) > 1)
            goal();
        while((readSensor(IN_2) < distance-10) || (readSensor(IN_2) > distance+10))
        {
            if(readSensor(IN_2) < distance-10)
            {
                OnFwdReg(OUT_C,speed+1);
                OnFwdReg(OUT_B,speed);
                Wait(50);
            }
            if(readSensor(IN_2) > 250)
            {
                turnright();
                turned = 1;
                break;
            }
            else if((readSensor(IN_2) > distance+10) && readSensor(IN_2) < 250)
            {
                OnFwdReg(OUT_C,speed-1);
                OnFwdReg(OUT_B,speed);
                Wait(50);
                
            }
            if(readSensor(IN_4))
            {
                turnleft();
                break;
            }
            if(readSensor(IN_3) > 1)
                goal();
        }
        Off(OUT_BC);
        while((readSensor(IN_1) < (last_angle-3)) ||  (readSensor(IN_1) > (last_angle+3)))
        {
            if(readSensor(IN_1) < (last_angle-2))
            {
                OnFwdReg(OUT_B,20);
                OnRevReg(OUT_C,20);
                Wait(50);
                Off(OUT_BC);
            }
            if(readSensor(IN_1) > (last_angle+2))
            {
                OnFwdReg(OUT_C,20);
                OnRevReg(OUT_B,20);
                Wait(50);
                Off(OUT_BC);
            }
            if(readSensor(IN_4))
            {
                turnleft();
                break;
            }
            if(readSensor(IN_3) > 1)
                goal();
        }
        OnFwdSync(OUT_BC,10);
        Wait(1000);
    }
    FreeEV3();
    return 0;
}
