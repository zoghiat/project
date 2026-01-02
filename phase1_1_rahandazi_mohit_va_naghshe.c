

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
    InitWindow(SCREEN_W, SCREEN_H, "Phase 1.1 - Environment & Map (Raylib, C)");
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