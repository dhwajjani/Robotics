//DHWAJ VANDANKUMAR JANI

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

# include <ev3.h>

#define LENGTHX 16

#define LENGTHY 10

#define Tile_Size 0.305

#define MAX_OBSTACLES 25    /* maximum number of obstacles */

enum{right,bottom,left,top};

int Current_Orientation = right;

typedef struct node

{
    
    int x;
    
    int y;
    
    int distance;bool isGoal;bool isStart;
    
} node;

typedef struct record

{
    
    int x;
    
    int y;
    
    bool valid;
    
} record;


void fillGrid(node grid[LENGTHX+2][LENGTHY+2], double obstacle[MAX_OBSTACLES][2],double start[2], int goal[2])

{
    
    int i,j,x,y;
    
    for (i = 0; i < LENGTHX + 2; i++)
        
    {
        
        for (j = 0; j < LENGTHY+ 2; j++)
            
        {
            
            grid[i][j].x = i;
            
            grid[i][j].y = j;
            
            if((i == 0) || (i == LENGTHX +1) || (j == 0) || (j == LENGTHY +1) )
                
                grid[i][j].distance = -1;
            
            else
                
                grid[i][j].distance = 55;
            
            grid[i][j].isGoal = false;
            
            grid[i][j].isStart = false;
            
        }
        
    }
    
    for (int i = 0; i < MAX_OBSTACLES && obstacle[i][0] != -1; i++)
        
    {
        
        x = obstacle[i][0] + 1;
        
        y = obstacle[i][1] + 1;
        
        grid[x][y].distance = -1;
        
        grid[x-1][y].distance = -1;
        
        grid[x][y-1].distance = -1;
        
        grid[x-1][y-1].distance = -1;
        
        
        
    }
    
    if(grid[goal[0]][goal[1]].distance >0)
    {
        grid[goal[0]][goal[1]].isGoal = true;
        
        grid[goal[0]][goal[1]].distance = 0;
        
        int temp_distance = 0;
        
        while(temp_distance <= 25)
            
        {
            
            for(i=1; i<LENGTHX + 1; i++)
                
            {
                
                for(j=1; j<LENGTHY + 1; j++)
                    
                {
                    
                    if(grid[i][j].distance == temp_distance)
                        
                    {
                        
                        if((grid[i-1][j].distance > 0) && (grid[i-1][j].distance > grid[i][j].distance))
                            
                            grid[i-1][j].distance = temp_distance + 1;
                        
                        if((grid[i+1][j].distance > 0) && (grid[i+1][j].distance > grid[i][j].distance))
                            
                            grid[i+1][j].distance = temp_distance + 1;
                        
                        if((grid[i][j-1].distance > 0) && (grid[i][j-1].distance > grid[i][j].distance))
                            
                            grid[i][j-1].distance = temp_distance + 1;
                        
                        if((grid[i][j+1].distance > 0) && (grid[i][j+1].distance > grid[i][j].distance))
                            
                            grid[i][j+1].distance = temp_distance + 1;
                        
                    }
                    
                }
                
            }
            
            temp_distance++;
            
        }
    }
    else
    {
        LcdPrintf(1, "invalid Goal!!\n");
        Wait(2000);
    }
    for (j = LENGTHY + 1; j >= 0; j--)
        
    {
        
        
        
        for (i = 0; i < LENGTHX + 2; i++)
            
        {
            
            if(i == 0)
                
                printf("\n");
            
            printf("%2d\t", grid[i][j].distance);
            
            
            
        }
        
    }
    
}



bool check_plus_x(node grid[LENGTHX+2][LENGTHY+2], int current_pos[2])

{
    
    return ((grid[current_pos[0] + 1][current_pos[1]].distance < grid[current_pos[0]][current_pos[1]].distance) && (grid[current_pos[0] + 1][current_pos[1]].distance != -1) );
    
    
    
}


bool check_plus_y(node grid[LENGTHX+2][LENGTHY+2], int current_pos[2])

{
    
    return ((grid[current_pos[0]][current_pos[1]+1].distance < grid[current_pos[0]][current_pos[1]].distance) && (grid[current_pos[0]][current_pos[1]+1].distance != -1) );
    
}

bool check_minus_y(node grid[LENGTHX+2][LENGTHY+2], int current_pos[2])

{
    
    return ((grid[current_pos[0]][current_pos[1]-1].distance < grid[current_pos[0]][current_pos[1]].distance) && (grid[current_pos[0]][current_pos[1]-1].distance != -1) );
    
}

bool check_minus_x(node grid[LENGTHX+2][LENGTHY+2], int current_pos[2])

{
    
    return ((grid[current_pos[0] - 1][current_pos[1]].distance < grid[current_pos[0]][current_pos[1]].distance) && (grid[current_pos[0] - 1][current_pos[1]].distance != -1) );
    
    
    
}



bool findPath(node grid[LENGTHX+2][LENGTHY+2] , record *record, int start[2])

{
    
    int s = 1;
    
    bool path_found = true;
    
    int current_pos[2];
    
    current_pos[0] = start[0];
    
    current_pos[1] = start[1];
    
    while (grid[current_pos[0]][current_pos[1]].distance != 0)
        
    {
        
        if(check_plus_x(grid,&current_pos[0]))
            
        {
            
            current_pos[0] = current_pos[0] + 1;
            
            record[s].x = current_pos[0];
            
            record[s].y = current_pos[1];
            
            record[s].valid = true;   //used as valid record
            
            s++;
            
        }
        
        else if(check_plus_y(grid,&current_pos[0]))
            
        {
            
            current_pos[1] = current_pos[1] + 1;
            
            record[s].x = current_pos[0];
            
            record[s].y = current_pos[1];
            
            record[s].valid = true;   //used as valid record
            
            s++;
            
        }
        
        else if(check_minus_y(grid,&current_pos[0]))
            
        {
            
            current_pos[1] = current_pos[1] - 1;
            
            record[s].x = current_pos[0];
            
            record[s].y = current_pos[1];
            
            record[s].valid = true;   //used as valid record
            
            s++;
            
        }
        
        else if(check_minus_x(grid,&current_pos[0]))
            
        {
            
            current_pos[0] = current_pos[0] - 1;
            
            record[s].x = current_pos[0];
            
            record[s].y = current_pos[1];
            
            record[s].valid = true;   //used to validate record
            
            s++;
            
        }
        
        else
        {
            printf("\nNo Path Found!!!");
            path_found = false;
            break;
        }
        
        
        
    }
    return path_found;
    
}


void rotate_plus90()

{
    Off(OUT_BC);
    Wait(500);
    //ResetRotationCount(OUT_BC);
    int countC, countB;
    OnFwdReg(OUT_C,3);
    OnRevReg(OUT_B,3);
    Wait(5200);
    Off(OUT_BC);
    countB = MotorRotationCount(OUT_B);
    countC = MotorRotationCount(OUT_C);
    float average1 = ((abs(countB)+abs(countC))/2);
    if(average1 < 176 )
    {
        OnFwdReg(OUT_B,3);
        OnRevReg(OUT_C,3);
        Wait(400);
        Off(OUT_BC);
    }
    else if(average1 > 181)
    {
        OnFwdReg(OUT_C,3);
        OnRevReg(OUT_B,3);
        Wait(400);
        Off(OUT_BC);
    }
}


void rotate_minus90()

{
    Off(OUT_BC);
    Wait(500);
    //ResetRotationCount(OUT_BC);
    int countC, countB;
    OnFwdReg(OUT_B,3);
    OnRevReg(OUT_C,3);
    Wait(5200);
    Off(OUT_BC);
    countB = MotorRotationCount(OUT_B);
    countC = MotorRotationCount(OUT_C);
    float average1 = ((abs(countB)+abs(countC))/2);
    if(average1 < 176.5 )
    {
        OnFwdReg(OUT_B,3);
        OnRevReg(OUT_C,3);
        Wait(500);
        Off(OUT_BC);
    }
    else if(average1 > 179 )
    {
        OnFwdReg(OUT_C,3);
        OnRevReg(OUT_B,3);
        Wait(500);
        Off(OUT_BC);
    }
    
}


void move_forward_tile()

{
    OnFwdSync(OUT_BC,20);
    
    Wait(3000);
    
}


void execute_path(record *record, int max_n)

{
    
    int n = 0;
    while((n < max_n) && record[n].valid)
        
    {
        
        switch(Current_Orientation)
        
        {
                
            case right:
                
                if(record[n].y < record[n+1].y)
                    
                {
                    
                    rotate_plus90();
                    
                    move_forward_tile();
                    
                    Current_Orientation = top;
                    
                }
                
                else if(record[n].y > record[n+1].y)
                    
                {
                    
                    rotate_minus90();
                    
                    move_forward_tile();
                    
                    Current_Orientation = bottom;
                    
                }
                
                else if(record[n].x > record[n+1].x)
                {
                    rotate_minus90();
                    rotate_minus90();
                    move_forward_tile();
                    Current_Orientation = left;
                    
                }
                
                else
                {
                    
                    move_forward_tile();
                    
                }
                
                break;
                
            case bottom:
                
                if(record[n].x < record[n+1].x)
                    
                {
                    
                    rotate_plus90();
                    
                    move_forward_tile();
                    
                    Current_Orientation = right;
                    
                }
                
                else if(record[n].x > record[n+1].x)
                    
                {
                    
                    rotate_minus90();
                    
                    move_forward_tile();
                    
                    Current_Orientation = left;
                    
                }
                
                else if(record[n].y < record[n+1].y)
                {
                    
                    rotate_minus90();
                    rotate_minus90();
                    move_forward_tile();
                    Current_Orientation = top;
                }
                
                else
                    
                {
                    
                    move_forward_tile();
                    
                }
                
                break;
                
            case left:
                
                if(record[n].y < record[n+1].y)
                    
                {
                    
                    rotate_minus90();
                    
                    move_forward_tile();
                    
                    Current_Orientation = top;
                    
                }
                
                else if(record[n].y > record[n+1].y)
                    
                {
                    
                    rotate_plus90();
                    
                    move_forward_tile();
                    
                    Current_Orientation = bottom;
                    
                }
                
                else if(record[n].x < record[n+1].x)
                {
                    
                    rotate_minus90();
                    rotate_minus90();
                    move_forward_tile();
                    Current_Orientation = right;
                }
                
                else
                    
                {
                    
                    move_forward_tile();
                    
                }
                
                break;
                
            case top:
                
                if(record[n].x < record[n+1].x)
                    
                {
                    
                    rotate_minus90();
                    
                    move_forward_tile();
                    
                    Current_Orientation = right;
                    
                }
                
                else if(record[n].x > record[n+1].x)
                    
                {
                    
                    rotate_plus90();
                    
                    move_forward_tile();
                    
                    Current_Orientation = left;
                    
                }
                
                else if(record[n].y > record[n+1].y)
                {
                    
                    rotate_minus90();
                    rotate_minus90();
                    move_forward_tile();
                    Current_Orientation = bottom;
                }
                
                else
                    
                {
                    
                    move_forward_tile();
                    
                }
                
                break;
                
            default:
                
                printf("\n");
                
                
                
        }
        
        n++;
        
    }
    Off(OUT_BC);
    bool LcdClean();
    LcdPrintf(1, "\n\nGoal Reached!\nManhattan Distance : \n%d",max_n);
    Wait(SEC_10);
    
}


int main()

{
    
    InitEV3();
    
    int num_obstacles = 14; /* number of obstacles */
    
    double obstacle[MAX_OBSTACLES][2] = /* obstacle locations */
    {{4,1},{4,2},{4,3},{4,4},{4,5},{4,6},{7,9},{7,8},{7,7},{7,6},{7,5},{8,5},{11,4},{12,4}, {-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    //
    //        {-1,-1},{-1,-1},{-1,-1}
    
    
    //    {{0.61, 2.743},{0.915, 2.743},{1.219, 2.743},{1.829, 1.219},
    //
    //        {1.829, 1.524},{ 1.829, 1.829}, {1.829, 2.134},{2.743, 0.305},
    //
    //        {2.743, 0.61},{2.743, 0.915},{2.743, 2.743},{3.048, 2.743},
    //
    //        {3.353, 2.743},
    //
    //        {-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},
    //
    //        {-1,-1},{-1,-1},{-1,-1}};
    
    //    double start[2] = {0.305, 1.219}; /* start location */
    //
    //    double goal[2] = {3.658, 1.829}; /* goal location */
    
    int countB, countC;
    ResetRotationCount(OUT_BC);
    
    int Start[2], Goal[2];
    
    //    Start[0] = round(start[0] + 1;
    //
    //    Start[1] = round(start[1]/Tile_Size) + 1;
    //
    //    Goal[0] = round(goal[0]/Tile_Size) + 1;
    //
    //    Goal[1] = round(goal[1]/Tile_Size) + 1;
    
    Start[0] = 2+1;
    
    Start[1] = 2+1;
    
    Goal[0] = 10+1;
    
    Goal[1] = 8+1;
    
    node grid[LENGTHX+2][LENGTHY+2];  //to include boundaries
    
    fillGrid(grid, obstacle, Start, Goal);
    
    record record[grid[Start[0]][Start[1]].distance+1];
    
    if(grid[Goal[0]][Goal[1]].isGoal)
    {
        
        record[0].x = Start[0];
        
        record[0].y = Start[1];
        
        record[0].valid = true;
        
        if(findPath(grid,record, Start))
        {
            
            execute_path(record,grid[Start[0]][Start[1]].distance);
        }
    }
    countB = MotorRotationCount(OUT_B);
    countC = MotorRotationCount(OUT_C);
    
    
    FreeEV3();
    
    return 0;
    
}
