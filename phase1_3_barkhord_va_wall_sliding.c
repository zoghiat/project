

#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define SCREEN_W  960
#define SCREEN_H  640

#define MAP_W 24
#define MAP_H 16
#define TILE_SIZE 32

// arry 2d
static int world_map[MAP_H][MAP_W] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1},
    {1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1},
    {1,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

typedef struct {
    Vector2 pos;   // position
    Vector2 dir;   // bilbilak
    float moveSpeed;
    float rotSpeed;
} Player;
//andaze gir radical 
static float lengthVec2(Vector2 v) {
    return sqrtf(v.x*v.x + v.y*v.y);
}


// normalayez kardan bra fovو وکتور دو برای دو بعدی
static Vector2 normalize(Vector2 v) {
    float l = lengthVec2(v);
    //bra jelogry az 0%0
    if (l == 0) return (Vector2){0,0};
    //normalayze shode ro bar gardone
    return (Vector2){ v.x / l, v.y / l };
}

int main(void)
{
    InitWindow(SCREEN_W, SCREEN_H, "Phase 1.3 - Collision & Wall Sliding (Raylib, C)");
    //show frame
    SetTargetFPS(60);

    // مرکز نقشه کوچک بالای صفحه یا میانه صفحه - ما کل پنجره را نمای بالا استفاده میکنیم
    // تمام استراکت صفر هسا در اول 
    Player player = {0};
    player.pos = (Vector2){ 3.5f, 3.5f }; // درون خانه (x,y) در واحد خانه‌ها
    player.dir = normalize((Vector2){ 1.0f, 0.0f }); // be ofogh negae kone normalayez bra mohkam kari
    player.moveSpeed = 3.5f; // soraaat
    player.rotSpeed = 2.5f; // charkhes

    while (!WindowShouldClose())
    {
        //delat time=dt, baraye raften rahat tare va softe,dt=fasle bein frame



        float dt = GetFrameTime();

        // tagherr dir
        if (IsKeyDown(KEY_RIGHT)) {
            float ang = -player.rotSpeed * dt; //جهت راست 
            float cosA = cosf(ang), sinA = sinf(ang);
            //ba estefade az matris rahnamie shode dare power point
            Vector2 nd = { player.dir.x * cosA - player.dir.y * sinA,
                           player.dir.x * sinA + player.dir.y * cosA };
            //normalyz karadan be dalayeli
            player.dir = normalize(nd);
        }
        if (IsKeyDown(KEY_LEFT)) {

            float ang = player.rotSpeed * dt; // چپ
            float cosA = cosf(ang), sinA = sinf(ang);
            Vector2 nd = { player.dir.x * cosA - player.dir.y * sinA,
                           player.dir.x * sinA + player.dir.y * cosA };
            player.dir = normalize(nd);
        }


        // w va s baraye inke to khode dir hast
        Vector2 move = {0.0f, 0.0f};
        if (IsKeyDown(KEY_W)) { move.x += player.dir.x; move.y += player.dir.y; }
        if (IsKeyDown(KEY_S)) { move.x -= player.dir.x; move.y -= player.dir.y; }
        // a va d chon ammod ast
        Vector2 perp = { -player.dir.y, player.dir.x }; // amood
        if (IsKeyDown(KEY_A)) { move.x += perp.x; move.y += perp.y; }
        if (IsKeyDown(KEY_D)) { move.x -= perp.x; move.y -= perp.y; }

        // اگر حرکت وجود دارد نرمالایز کن تا سرعت کنترلی داشته باشیم (اجتناب از سرعت بیشتر در قطری)
        if (lengthVec2(move) > 0.0f) {
            move = normalize(move);
            move.x *= player.moveSpeed * dt;
            move.y *= player.moveSpeed * dt;
        }

        // khordan be divar 
        // x ha
        float newPosX = player.pos.x + move.x;
        //cast
        int mapX = (int)newPosX;
        int mapY = (int)player.pos.y;
        if (mapX >= 0 && mapX < MAP_W && mapY >= 0 && mapY < MAP_H) {
            if (world_map[mapY][mapX] == 0) {
                player.pos.x = newPosX;
            } else {
                // برخورد در محور X -> لغزش فقط در جهت y ممکن است (x ثابت)
                // فقط صفر کردن مؤلفه x کافیست؛ چون y بعدا بررسی می‌شود
            }
        }

        // y ha
        float newPosY = player.pos.y + move.y;
        mapX = (int)player.pos.x;
        mapY = (int)newPosY;
        if (mapX >= 0 && mapX < MAP_W && mapY >= 0 && mapY < MAP_H) {
            if (world_map[mapY][mapX] == 0) {
                player.pos.y = newPosY;
            } else {
                // برخورد در محور Y -> لغزش فقط در جهت x ممکن است (y ثابت)
            }
        }

        // draw windows
        BeginDrawing();
            ClearBackground(PURPLE);

            // bardashtan az map
            for (int y = 0; y < MAP_H; y++) {
                for (int x = 0; x < MAP_W; x++) {
                    Rectangle cell = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                    if (world_map[y][x] == 1) {
                        DrawRectangleRec(cell, DARKGRAY);
                    } else {
                        DrawRectangleLines(cell.x, cell.y, cell.width, cell.height, LIGHTGRAY);
                    }
                }
            }

            // رسم بازیکن (دایره در مرکز خانه به مختصات پیکسلی)
            Vector2 pixPos = { player.pos.x * TILE_SIZE, player.pos.y * TILE_SIZE };
            DrawCircleV(pixPos, TILE_SIZE * 0.25f, BLUE);

            // رسم جهت نگاه بازیکن (خط)
            Vector2 dirEnd = { pixPos.x + player.dir.x * TILE_SIZE * 0.9f,
                               pixPos.y + player.dir.y * TILE_SIZE * 0.9f };
            DrawLineEx(pixPos, dirEnd, 3.0f, RED);

            

           

        EndDrawing();
    }

    CloseWindow();
    return 0;
}