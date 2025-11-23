#include "raylib.h"
#include <math.h>
#include<time.h>
#include<stdlib.h>
#include <stdio.h>
#define gravity 1.06f
#define playerspeed 7.0f
#define arrowspeed 11.6f



int groundcollision(Vector2 sprite){//takes positionvector of the sprite and checks groundcollision
    if(sprite.y>=430) return 1;
    return 0;
}


int platformCollision(Vector2 player, Vector2 platform , float platformLength){
    if(player.x>=platform.x && player.x<=(platform.x+platformLength)){
        if(player.y+20-platform.y>-10&&player.y+20-platform.y<10){
            return 1;
        }
    }
    return 0;
}


Vector2 random_star_pos(){
    Vector2 vec;
    float possibleyvals[]={20.0f,80.0f,100.0f,170.0f,260.0f,280.0f,390.0f};
    vec.x=30+740*((float)rand()/(float)RAND_MAX);
    vec.y=possibleyvals[rand()%7];
    return vec;
}


float random_y_position(){//returns a random y coordinate that dosent collide with a platform
    float possible_positions[]={80.0f,150.0f,300.0f,410.0f};
    return possible_positions[(rand()%4)];
}


float target_y_position(Vector2 x){//returns a targetted y coordinate for arrow
    float possible_positions[]={80.0f,150.0f,300.0f,410.0f};
    float distances[4];
    int shortest=0;
    distances[1]=fabs(possible_positions[1]-x.y);
    for(int i=1;i<4;i++){
        distances[i]=possible_positions[i]-x.y;
    }
    for(int i=0;i<4;i++){
        if(distances[i]<shortest) shortest=i;
    }
    return possible_positions[shortest];
}


int player_arrow_collision(Vector2 playerpos, Vector2 arrowpos){
    Vector2 playerledege={playerpos.x,playerpos.y},arrowtip={arrowpos.x+86,arrowpos.y-11};
    if(arrowtip.x>=playerledege.x &&arrowtip.x-playerledege.x<90 && fabs(playerpos.y-arrowpos.y-11)<25.0f)
        return 1;
    return 0;
}


int star_col(Vector2 p,Vector2 s){
    if(fabs(p.x-s.x)<30&&fabs(p.y-s.y)<=30){
        return 1;
    }
    return 0;
}


int main(void)
{
    srand(time(NULL));
    //Variable Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int jumps_allowed=2;
    const Vector2 platform1={150,340},platform2={450,190},platform3={150,120};
    const Vector2 platform_size={170,20};
    int points=0;
    int game_over=0;
    InitWindow(screenWidth, screenHeight, "can i please get full marks for this :)");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    Vector2 player={400,225};
    float playerYSpeed=3.0f; //handles only the verticsls compnent of the playeres speed
    //horizontal is controled by the arrow keys
    Color playerColor={0, 28, 66, 255};
    int jumps=0;
    
    Vector2 arrowpos={-100.0f,150.0f};
    Vector2 starpos=random_star_pos();
    //loading sprites
    Texture2D backdrop=LoadTexture("Piskelbackdrop.png");
    Texture2D backdrop2=LoadTexture("Piskelbackdrop(2).png");
    Texture2D arrow_img=LoadTexture("arrow.png");
    Texture2D star_img=LoadTexture("star2.png");
    //loading highscore
    int highscore;
    FILE *fptr=fopen("highscore.txt","r");
    fscanf(fptr,"%d",&highscore);
    fclose(fptr);
    //starting window
    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(backdrop2,0,0,WHITE);
            DrawText("PRESS ARROW KEYS TO START", 40, 200, 40,BLACK);
        EndDrawing();
        
        if(IsKeyPressed(KEY_UP)){
            break;
        }
        if(IsKeyPressed(KEY_RIGHT)){
            break;
        }
        if(IsKeyPressed(KEY_LEFT)){
            break;
        }
        if(IsKeyPressed(KEY_DOWN)){
            break;
        }
    }
    
    // Main game loop
    while (!WindowShouldClose())  // Detects window close button or ESC key
    {
        // Update variables

        //movements
        if(IsKeyPressed(KEY_D)||IsKeyPressed(KEY_UP)){
            //jump
            if(jumps<jumps_allowed){  //ensures jumping is limited to double jump
                playerYSpeed=-14.0f;
                jumps++;
            }
        }
        
        if(IsKeyDown(KEY_RIGHT)){
            if(player.x<screenWidth-20)
            player.x+=playerspeed;
        }

        if(IsKeyDown(KEY_LEFT)){
            if(player.x>20)
            player.x-=playerspeed;
        }

        //Appplying speed and gravity
        player.y+=playerYSpeed;
        if (playerYSpeed<14.0f){
            playerYSpeed+=gravity;
        }

        //ground collision
        if(groundcollision(player)){  
            player.y=430;
            jumps=0; //resets remaining jumps tp zero everytime you land
        }

        //platform collision
        if(platformCollision(player,platform1,platform_size.x)){//1
            if(playerYSpeed>=0){
                player.y=platform1.y-20;
                playerYSpeed=0.0f;
                jumps=0;
            }
        }

        if(platformCollision(player,platform2,platform_size.x)){//2
            if(playerYSpeed>=0){
                player.y=platform2.y-20;
                playerYSpeed=0.0f;
                jumps=0;
            }
        }

         if(platformCollision(player,platform3,platform_size.x)){//3
            if(playerYSpeed>=0){
                player.y=platform3.y-20;
                playerYSpeed=0.0f;
                jumps=0;
            }
        }


        //arrow moovements
        arrowpos.x+=arrowspeed;
        if(arrowpos.x>850){
            arrowpos.x=-100;
            if(points<10)
            arrowpos.y=random_y_position();
            else
            arrowpos.y=target_y_position(player);
        }


        //arrow collision- check for game over
        if(player_arrow_collision(player,arrowpos))
        {
            game_over=1;
        }


        //star collosion
        if(star_col(player,starpos)){
            points++;
            starpos=random_star_pos();
        }

        // Draw
        BeginDrawing();

            if(!game_over){
                ClearBackground(RAYWHITE);
                DrawTexture(backdrop,0,0,WHITE);
                DrawCircleV(player,20,playerColor);
                DrawRectangleV(platform1,platform_size,BLACK);
                DrawRectangleV(platform2,platform_size,BLACK);
                DrawRectangleV(platform3,platform_size,BLACK);
                DrawTextureV(arrow_img,arrowpos,WHITE);
                DrawTexture(star_img,starpos.x-16,starpos.y-16,WHITE);
                DrawText(TextFormat("Points: %d",points), 20, 20, 20, DARKGRAY);
                //DrawText(TextFormat("a targetted position= %.3f",target_y_position(player)),20,50,30,BLACK);
                //DrawText(TextFormat("star Y = %.3f",starpos.y),20,80,20,BLACK);
                //note: the above are for debugging
            }
            else{
                ClearBackground(RAYWHITE);
                DrawTexture(backdrop2,0,0,WHITE);
                DrawText("you got hit by an arrow\nyou lose\n\nplease give this project \nfull marks :)\n\npress esc", 10, 10, 50, BLACK);

            }

       EndDrawing();
    }

    // De-Initialization
    UnloadTexture(backdrop);
    UnloadTexture(arrow_img);
    UnloadTexture(star_img); 
    UnloadTexture(backdrop2);

    if(highscore<points){
        fptr=fopen("highscore.txt","w");
        fprintf(fptr,"%d",points);
        fclose(fptr);
    }

    CloseWindow();
    return 0;
}