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
        Vector2 dir;   // bilbilak jahat
        float moveSpeed;
        // sorat chaekhesh
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

    static void SaveMapBinary(const char *fname) {
        //WB== WRITE BINARY
        FILE *f = fopen(fname, "wb");
        if (!f) return;
        fwrite(world_map, sizeof(world_map), 1, f);
        fclose(f);
    }

    static void LoadMapBinary(const char *fname) {
        // RB=== READ BINERY
        FILE*f = fopen(fname,"rb");
        if(!f) return;
        fread(world_map, sizeof(world_map),1,f);
        fclose(f);
    }

    static void DrawTopDownMap(Player player, int showMouse) {
        // bardashtan az map
        for (int y = 0; y < MAP_H; y++) {
            for (int x = 0; x < MAP_W; x++) {
                Rectangle cell = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                if (world_map[y][x] == 1) {
                    DrawRectangleRec(cell, DARKGRAY);
                } else {
                    DrawRectangleLines((int)cell.x,(int)cell.y,(int)cell.width,(int)cell.height,LIGHTGRAY);
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
            //بفهمه موس کجاست روی کدوم بلاک هست 
        if (showMouse) {
            Vector2 m = GetMousePosition();
            int mx = (int)(m.x / TILE_SIZE);
            int my = (int)(m.y / TILE_SIZE);
            if (mx>=0&&mx<MAP_W&&my>=0&&my<MAP_H) {
                DrawRectangleLines(mx*TILE_SIZE, my*TILE_SIZE,TILE_SIZE,TILE_SIZE,YELLOW);
            }
        }
    }

    static void Render3D(Player player) {
        // plane (FOV) - از روی dir ساخته میشه (بدون تغییر Player)
        // مقدار 0.66 تقریبا FOV = 66 درجه مثل 
        Vector2 plane = { -player.dir.y*0.66f, player.dir.x*0.66f };

        // sky/floor طبق هشدار power point اول سقف و اسمون بعد دیوار ها
        // اسمون
        DrawRectangle(0, 0, SCREEN_W, SCREEN_H/2, (Color){60, 60, 80, 255});
        // زمین 
        DrawRectangle(0, SCREEN_H/2, SCREEN_W, SCREEN_H/2, (Color){40, 40, 40, 255});

        // Raycasting Loop
            // به اندازه ی تعداد پیکسل های عرض صفحه
            // در هر فریم به تعداد عرض صفحه تکرار می شود 
        for (int x = 0; x < SCREEN_W; x++) {
            // camera برای این دو است که از بین -1 تا 1 باشد میدان دید 
               // راست صفحه 1و چپ -1
            float cameraX = 2.0f * x / (float)SCREEN_W - 1.0f;
            // محاسبه ی حهت پرتو
            float rayDirX = player.dir.x + plane.x * cameraX;
            float rayDirY = player.dir.y + plane.y * cameraX;

            int mapX = (int)player.pos.x;
            int mapY = (int)player.pos.y;

            // 1e30of برای یک عدد بزرگ
            // برای اینکه تقسیم صقر بر صفر اتفاق نیوفتد
            float deltaDistX = (rayDirX == 0) ? 1e30f : fabsf(1.0f / rayDirX);
            float deltaDistY = (rayDirY == 0) ? 1e30f : fabsf(1.0f / rayDirY);
            // مقایسه ی موقیعت ها ابتدایی
            float sideDistX;
            float sideDistY;

            int stepX;
            int stepY;

            int hit = 0;
            int side = 0; // 0: x-side, 1: y-side
            // حرکت به طرف چپ
            if (rayDirX < 0) {
                // چون چپ هست step منفس می شود
                stepX = -1;
                sideDistX = (player.pos.x - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0f - player.pos.x) * deltaDistX;
            }
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (player.pos.y - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0f - player.pos.y) * deltaDistY;
            }

            // DDA
                // hit به عونوان پرچم
            
            while (!hit) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                if (mapX < 0 || mapX >= MAP_W || mapY < 0 || mapY >= MAP_H) {
                    hit = 1;
                    break;
                }
                if (world_map[mapY][mapX] > 0) hit = 1;
            }

            // perpWallDist (Fisheye fix)
            float perpWallDist;
            if (side == 0) perpWallDist = (sideDistX - deltaDistX);
            else           perpWallDist = (sideDistY - deltaDistY);

            if (perpWallDist <= 0.0001f) perpWallDist = 0.0001f;

            int lineHeight = (int)(SCREEN_H / perpWallDist);

            int drawStart = -lineHeight / 2 + SCREEN_H / 2;
            int drawEnd   =  lineHeight / 2 + SCREEN_H / 2;

            if (drawStart < 0) drawStart = 0;
            if (drawEnd >= SCREEN_H) drawEnd = SCREEN_H - 1;

            // Shading برای حس عمق و فرق دیوار افقی/عمودی
            Color wallColor = (Color){200, 200, 200, 255};
            if (side == 1) wallColor = (Color){140, 140, 140, 255};

            // رسم دیوار یک ستون
            DrawLine(x, drawStart, x, drawEnd, wallColor);
        }
    }

    int main(void)
    {
        InitWindow(SCREEN_W, SCREEN_H, "Raycasting engine-Phase 1 to 3 (Raylib,C)");
        SetTargetFPS(60);

        // تمام استراکت صفر هسا در اول
        Player player = (Player){0};
        player.pos = (Vector2){ 3.5f, 3.5f }; // درون خانه (x,y) در واحد خانه‌ها
        player.dir = normalize((Vector2){ 1.0f, 0.0f });
        player.moveSpeed = 3.5f; // soraaat
        player.rotSpeed = 2.5f;  // charkhes

        // Phase 3: View Toggling
        int editMode = 0; // 0: Play Mode(3D), 1: Edit Mode(2D)

        while (!WindowShouldClose())
        {
            float dt = GetFrameTime();

            // سوئیچ بین حالت‌ها
            if(IsKeyPressed(KEY_M)) {
                editMode=!editMode;
            }

            // =========================
            // Phase 1: movement/rotation (فقط در Play Mode)
            // =========================
            if (!editMode) {
                // tagherr dir
                if (IsKeyDown(KEY_RIGHT)) {
                    float ang=player.rotSpeed*dt;   
                    float cosA=cosf(ang),sinA=sinf(ang);
                    Vector2 nd ={player.dir.x*cosA -player.dir.y* sinA,
                    player.dir.x * sinA +player.dir.y *cosA };
                    player.dir = normalize(nd);
                }
                if (IsKeyDown(KEY_LEFT)) {
                    float ang = -player.rotSpeed * dt;  
                    float cosA = cosf(ang), sinA = sinf(ang);
                    Vector2 nd ={player.dir.x *cosA -player.dir.y * sinA,
                    player.dir.x * sinA + player.dir.y * cosA };
                    player.dir = normalize(nd);
                }


                // w va s baraye inke to khode dir hast
                Vector2 move = (Vector2){0.0f, 0.0f};
                if (IsKeyDown(KEY_W)) { move.x += player.dir.x;move.y+=player.dir.y; }
                if (IsKeyDown(KEY_S)) { move.x -= player.dir.x;move.y -= player.dir.y; }

                // a va d chon ammod ast
                Vector2 perp = (Vector2){ -player.dir.y, player.dir.x }; // amood
                if (IsKeyDown(KEY_A)) { move.x -= perp.x;move.y -=  perp.y; }  // A->right
                if (IsKeyDown(KEY_D)) { move.x += perp.x;move.y +=perp.y; }  // D->left


                // اگر حرکت وجود دارد نرمالایز کن تا سرعت کنترلی داشته باشیم
                if (lengthVec2(move)> 0.0f) {
                    move = normalize(move);
                    move.x*=player.moveSpeed*dt;
                    move.y*=player.moveSpeed*dt;
                }

                // Wall Sliding 
                float newPosX=player.pos.x+move.x;
                int mapX=(int)newPosX;
                int mapY= (int)player.pos.y;
                if (mapX>=0&&mapX<MAP_W&&mapY>=0&&mapY<MAP_H) {
                    if (world_map[mapY][mapX] == 0) {
                        player.pos.x=newPosX;
                    } else {
                        // این الس و الس بعدی مهم است چون اگه خورد به دیوار قفل نکند
                        // برخورد در محور X -> لغزش فقط در جهت y ممکن است
                    }
                }

                float newPosY = player.pos.y + move.y;
                mapX=(int)player.pos.x;
                mapY=(int)newPosY;
                if (mapX>=0&&mapX<MAP_W&&mapY>=0 &&mapY<MAP_H) {
                    if (world_map[mapY][mapX] == 0) {
                        player.pos.y = newPosY;
                    } else {
                        // برخورد در محور Y -> لغزش فقط در جهت x ممکن است
                    }
                }
            }

            // =========================
            // Phase 3: قابلیت دست کاری  + سیو کردن و لود کردن 
            // =========================
            if (editMode) {
                Vector2 m = GetMousePosition();
                int mx = (int)(m.x / TILE_SIZE);
                int my = (int)(m.y / TILE_SIZE);

                if (mx >= 0 && mx < MAP_W && my >= 0 && my < MAP_H) {
                    // left click: 1 (کشیدن دیوار دلخواه)
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        world_map[my][mx] = 1;
                    }
                    // right click: 0 (خالی کردن دیوار ها )
                    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                        world_map[my][mx]= 0;
                    }
                }

                // Save & Load فقط در Edit Mode تا با حرکت KEY_S تداخل نداشته باشه
                if (IsKeyPressed(KEY_S)) {
                    SaveMapBinary("map.bin");
                }
                if (IsKeyPressed(KEY_L)) {
                    LoadMapBinary("map.bin");
                }
            }

            // =========================
            // Drawing
            // =========================
            BeginDrawing();
            ClearBackground(BLACK);

            if(!editMode){
                // Play Mode: 3D Rendering
                Render3D(player);

                // Minimap کوچک گوشه (دیباگ)
                int miniScale= 8;
                int offX = 10, offY = 10;
                for (int y = 0; y<  MAP_H; y++) {
                    for (int x = 0; x < MAP_W; x++) {
                        Rectangle r = { offX + x * miniScale, offY + y * miniScale, miniScale, miniScale };
                        if (world_map[y][x] == 1) DrawRectangleRec(r, GRAY);
                        else DrawRectangleLines((int)r.x, (int)r.y, (int)r.width, (int)r.height, (Color){80,80,80,255});
                    }
                }
                Vector2 pp = { offX+player.pos.x*miniScale,offY+player.pos.y * miniScale };
                DrawCircleV(pp, 3, BLUE);
                DrawLine((int)pp.x, (int)pp.y, (int)(pp.x + player.dir.x * 10), (int)(pp.y + player.dir.y * 10), RED);

                DrawText("Play Mode (3D) - Press M to Edit Mode",10,SCREEN_H-24,18,RAYWHITE);
            } else {
                // Edit Mode: 2D Top-down + mouse edit
                ClearBackground(PURPLE);
                DrawTopDownMap(player,1);
                DrawText("Edit Mode (2D) - M:Play | Mouse L:1 R:0 | S:Save | L:Load",10,SCREEN_H-24,18,RAYWHITE);
            }

            EndDrawing();
        }

        CloseWindow();
        return 0;
    }
