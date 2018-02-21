 /*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <stdio.h>
#include <time.h>

int GetDaysPassed(time_t currentTime, time_t lastTime)
{
    int days = (lastTime - currentTime)/24/3600*-1;
    
    return days;
}

int GetHoursPassed(time_t currentTime, time_t lastTime)
{
    int hours = (lastTime - currentTime)/3600*-1;
    
    return hours;
}

int GetMinutesPassed(time_t currentTime, time_t lastTime)
{
    int minutes = (lastTime - currentTime)/60*-1;
    
    return minutes;
}

int SleepNeed(int seconds, int state)
{
    int value = 0;
    if(state != 0) value += seconds*0.75f;
    else value -= seconds*0.75f;
    return value;
}

int LifeLost(int seconds, int sleep)
{
    int value = 0;
    if(sleep == 0) value -= seconds*0.55f;
    return value;
}

typedef enum { Idle = 0, Sleeping } PetState; //0 Idle, 1 Sleeping


int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;
    
    static PetState state;
    
    
    int mayWidth = 450;
    int mayHeight = 450;
        
    int spriteRowsX = 0;
    int spriteRowsY = 0;
    int frameX = 0;
    int frameY = 0;
    int framesCounter = 0;
    
    Vector2 position = { 0, 0 };
    
    int life = 100;
    int sleep = 100;
    time_t currentTime;
    time_t lastTime;
    
    int daysPassed;
    int hoursPassed;
    int minutesPassed;
    int secondsPassed;
    
    
    FILE *vPetFile = fopen("data/basic.pet", "rb+");
    

    InitWindow(screenWidth, screenHeight, "vParrot");
    
    Texture2D may = LoadTexture("resources/quaker_parrot.png");
    spriteRowsX = 4;
    spriteRowsY = 7;
    mayWidth = (may.width);
    mayHeight = (may.height);
    Rectangle sprite = (Rectangle){ 0, 0, mayWidth / spriteRowsX, mayHeight / spriteRowsY };
    Rectangle bounds = (Rectangle){ 0, 0, mayWidth, mayHeight };
    
    fread(&position.x, 1, 4, vPetFile);
    fread(&position.y, 1, 4, vPetFile);
    fread(&sleep, 1, 4, vPetFile);
    fread(&life, 1, 4, vPetFile);
    fread(&state, 1, 4, vPetFile);
    fread(&lastTime, 1, 8, vPetFile);
    currentTime = time (NULL);
    
    daysPassed = GetDaysPassed(currentTime, lastTime);
    hoursPassed = GetHoursPassed(currentTime, lastTime);
    minutesPassed = GetMinutesPassed(currentTime, lastTime);
    secondsPassed = (lastTime - currentTime)*-1;
    
    sleep += SleepNeed(secondsPassed, state);
    life += LifeLost(secondsPassed, sleep);
    
    
    
    
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
            bounds.x = position.x;
            bounds.y = position.y;
            sprite.x = mayWidth / spriteRowsX * frameX;
            sprite.y = mayHeight / spriteRowsY * frameY;
            
            if(IsMouseButtonPressed(0)) 
            {
                state++;
                frameX = 0;
            }
            if(state > 1) state = 0;
            
            if(IsKeyDown(KEY_UP)) position.y-=2;
            else if(IsKeyDown(KEY_DOWN)) position.y+=2;
            if(IsKeyDown(KEY_LEFT)) position.x-=2;
            else if(IsKeyDown(KEY_RIGHT))position.x+=2;
            
            if(position.x <= 0) position.x = 0;
            if(position.y <= 0) position.y = 0;
            if(position.x >= screenWidth - mayWidth / spriteRowsX) position.x = screenWidth - mayWidth / spriteRowsX;
            if(position.y >= screenHeight - mayHeight / spriteRowsY) position.y = screenHeight - mayHeight / spriteRowsY;
            currentTime = time (NULL);
            
            //time = cloak();
       
            /*
            framesCounter++;
            if(framesCounter > 60)
            {
                frameX++;
                framesCounter = 0;
            }                
            if(frameX > 3) frameX = 0;
            */
            
            switch(state)
            {
                case(Idle):
                {
                    frameY = 0;
                    framesCounter++;
                    if(framesCounter > 60)
                    {
                        frameX++;
                        framesCounter = 0;
                        if(frameX > 3) frameX = 0;
                    }
                    
                    if(framesCounter%45 == 0)
                    {
                        sleep--;
                        if (sleep == 0) life--;
                    }
                    if(sleep <= 0) sleep = 0;
                    
                }
                break;
                
                case(Sleeping):
                {
                    frameY = 4;
                    framesCounter++;
                    if(framesCounter > 60)
                    {
                        frameX++;
                        framesCounter = 0;
                        if(frameX > 1) frameX = 0;
                    }
                    
                    if(framesCounter%45 == 0) sleep++;
                        
                    
                    if(sleep >= 100) sleep = 100;
                }
                break;
                
                default:break;          
            }
            
            
            
            if(IsKeyPressed(KEY_ENTER))
            {
                lastTime = currentTime;
                fseek(vPetFile, 0, SEEK_SET);
                fwrite(&position.x, 1, 4, vPetFile);
                fwrite(&position.y, 1, 4, vPetFile);
                fwrite(&sleep, 1, 4, vPetFile);
                fwrite(&life, 1, 4, vPetFile);
                fwrite(&state, 1, 4, vPetFile);
                fwrite(&lastTime, 1, 8, vPetFile);
            }
            
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            //DrawTexture(may, 50, 50, WHITE);
            DrawTextureRec(may, sprite, position, WHITE);
            DrawRectangle(150,0,sleep,25, ORANGE);
            DrawText(FormatText("Sleep: %i", sleep), 155, 0, 19, BLACK);
            DrawRectangle(0,0,life,25, RED);
            DrawText(FormatText("Health: %i", life), 5, 0, 19, BLACK);
            DrawText(FormatText("%ld", currentTime), 50, 50, 20, BLACK);
            DrawText(FormatText("%ld", lastTime), 50, 100, 20, BLACK);
            DrawText(FormatText("%i", daysPassed), 50, 150, 20, BLACK);
            DrawText(FormatText("%i", hoursPassed), 100, 150, 20, BLACK);
            DrawText(FormatText("%i", minutesPassed), 150, 150, 20, BLACK);
            DrawText(FormatText("%i", secondsPassed), 200, 150, 20, BLACK);



        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    UnloadTexture(may);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}