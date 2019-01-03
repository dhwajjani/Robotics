/*
CSE -5364 Fall 2018
Project- Drawing Robot C Code
DHWAJ VANDANKUMAR JANI   1001583008
*/

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <math.h>
    #include <ev3.h>
    #include <ev3_sound.h>
    #define PI 3.14
    #define l1 9.5 //Link length one
    #define l2 10  //Link length two
    float Theta_1=0, Theta_2=0, Theta_3=0, Last_Theta_1=0, Last_Theta_2=0;


    void Inverse_Kin(float x1, float y1)
    {
        float alpha=0, beta=0, a=0, raw_Theta_1, raw_Theta_2;
        alpha = atan2(y1,x1);
        a = sqrt(pow(x1,2)+pow(y1,2));
        beta = acos((pow(l1,2)+pow(a,2)-pow(l2,2))/(2*l1*a));
        raw_Theta_1 = alpha + beta;
        raw_Theta_2 = -acos((-pow(l1,2)+pow(a,2)-pow(l2,2))/(2*l1*l2));
        Theta_1 = raw_Theta_1 - Last_Theta_1;   //angular difference between desired position & last position
        Theta_2 = raw_Theta_2 - Last_Theta_2;
        Last_Theta_1 = raw_Theta_1;
        Last_Theta_2 = raw_Theta_2;

    }

    void Forward_Kin(float* x, float* y)
    {
     float b,c;
     b = MotorRotationCount(OUT_B);
     c = MotorRotationCount(OUT_C);
     b = (b*22)/(180*7);
     c = (c*22)/(180*7);
     *x = (l1*cos(b))+ (l2*cos(b+c));
     *y = (l1*sin(b))+ (l2*sin(b+c));
    }

    void Move_To_Jump(float theta1, float theta2)
    {
            int speed = 1;
            bool  reverse = false;
            ResetRotationCount(OUT_C);
            ResetRotationCount(OUT_B);
            while(MotorRotationCount(OUT_B) != theta1)
            {
           if(MotorRotationCount(OUT_B) > theta1)
                    OnRevReg(OUT_B,10);
           if(MotorRotationCount(OUT_B) < theta1)
                     OnFwdReg(OUT_B,10);
            Wait(1);
            if(speed<10 && !reverse)
            speed = speed + 1;
            else if(speed>=10 && !reverse)
            reverse = true;
            if(reverse && speed > 10)
                speed--;
            }
             Off(OUT_B);
             speed = 1;
             reverse = false;
             while(MotorRotationCount(OUT_C) != theta2)
            {
            if(MotorRotationCount(OUT_C) > theta2)
              OnRevReg(OUT_C,10);
             if(MotorRotationCount(OUT_C) < theta2)
               OnFwdReg(OUT_C,10);
            Wait(1);
            if(speed<10 && !reverse)
            speed = speed + 1;
            else if(speed>=10 && !reverse)
            reverse = true;
            if(reverse && speed > 10)
                speed--;
            }
            Off(OUT_C);
    }

    void Move_To_Draw(float Theta1, float Theta2)
    {
         int speed1 = 1, speed2 = 1;
        int max1, max2;
        max1 = abs(ceil(Theta1/2));
        max2 = abs(ceil(Theta2/2));
        ResetRotationCount(OUT_C);
        ResetRotationCount(OUT_B);
        while(MotorRotationCount(OUT_B) != Theta1 || MotorRotationCount(OUT_C) != Theta2)
        {
       if(MotorRotationCount(OUT_B) > Theta1)
                OnRevReg(OUT_B,speed1);
       else if(MotorRotationCount(OUT_B) < Theta1)
                 OnFwdReg(OUT_B,speed1);
       else
          Off(OUT_B);
        if(MotorRotationCount(OUT_C) > Theta2)
          OnRevReg(OUT_C,speed2);
        else if(MotorRotationCount(OUT_C) < Theta2)
           OnFwdReg(OUT_C,speed2);
        else
           Off(OUT_C);
        Wait(1);
        if(speed1<max1)
        speed1 = speed1 + 1;
       if(speed2<max2)
         speed2 = speed2 + 1;
        }
         Off(OUT_BC);
    }


    void Draw_Segment(float x1, float y1, float x2, float y2)  // Bresenham's line algorithm
    {
     float m, c, step_x, step_y, da, db, Current_x, Current_y;
     int No_of_Steps;
     Current_x = x1;
     Current_y = y1;
     if(x2 != x1)
     m = (y2-y1)/(x2-x1);
     else
     m = (y2-y1)/0.0001;
     c = y1 - (m*x1);
     if(x2 > x1)
     step_x = 0.1;
     else
     step_x = -0.1;
     if(y2 > y1)
     step_y = 0.1;
     else
     step_y = -0.1;
     accumulate_theta1 = 0; accumulate_theta2 = 0;
     extra_1 = 0; extra_2 = 0;j = 0; k = 0; sign_change1 = false; sign_change2 = false;
     if(fabsf(x2 - x1) >= fabsf(y2 - y1))   for slope < 1
     {
     No_of_Steps = abs(round((x2-x1)/0.1));   //Number of steps of size 1 millimeter on X axis
     for(int i=0; i < round(No_of_Steps/2) ; i++)  //find the midpoint of the segment
     {
     da = fabsf((m*(Current_x + step_x)) + c - Current_y);
     db = fabsf(Current_y + step_y - (m*(Current_x + step_x)) - c);
     if(da < db)
     {
     Current_x = Current_x + step_x;
     }
     else
     {
     Current_x = Current_x + step_x;
     Current_y = Current_y + step_y;
     }
     }
     }
     else                                     //for slope >= 1
     {
     No_of_Steps = abs(round((y2-y1)/0.1));   //Number of steps of size 1 millimeter on Y axis
     for(int i=0; i < round(No_of_Steps/2) ; i++)  //find the midpoint of the segment
     {
     da = fabsf((((Current_y + step_y) - c) / m) - Current_x);
     db = fabsf((Current_x + step_x ) - (((Current_y + step_y) - c) / m));
     if(da < db)
     {
     Current_y = Current_y + step_y;
     }
     else
     {
     Current_x = Current_x + step_x;
     Current_y = Current_y + step_y;
     }
     }
     }
     Inverse_Kin(Current_x,Current_y);
     Move_To_Draw(round((Theta_1*180*7)/22), round((Theta_2*180*7)/22));
     Inverse_Kin(x2,y2);
     Move_To_Draw(round((Theta_1*180*7)/22), round((Theta_2*180*7)/22));
    }

    void jump(float x, float y)  //jump to start point without drawing
    {
     Inverse_Kin(x,y);
     Move_To_Jump(round((Theta_1*180*7)/22), round((Theta_2*180*7)/22));
    }

    void pen_up()
    {
       OnFwdReg(OUT_A,2); 
       Wait(6000);
       Off(OUT_A);
    }

    void pen_down()
    {
    OnRevReg(OUT_A,2);
     Wait(6000);
     Off(OUT_A);
    } 
    
    int main(void)
    {
        InitEV3();
        float vertex[][2] = {{4,5},{8,6},{10,2},{8,1},{2,7},{4,5}};//coordinates of vertices of polygon
        int vertices = 6; //No of vertices in the polygon
        jump(vertex[0][0],vertex[0][1]); //jump from rest position to start position
        pen_down();
        for(int i = 0; i<vertices-1; i++)
        {
            Draw_Segment(vertex[i][0],vertex[i][1],vertex[i+1][0],vertex[i+1][1]);
        }
        pen_up();
        FreeEV3();
        return 0;
    }
