#include "raylib.h" 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void GameIsPlaying(
    int *PlayerX,
    Vector2* circles,
    Vector2* BadCircles,
    int *Score,
    int *Lives,
    Sound Pickup,
    Sound Hurt,
    int *PlaySFX,
    int *KeyboardInput,
    int *FPSDraw,
    int *GameOver,
    int *PauseGame) {
        
    float Delta = GetFrameTime();
    int ScreenWidth = GetScreenWidth();
    int ScreenHeigth = GetScreenHeight();
    int PlayerY = (GetScreenHeight() / 10) * 9;
    char ScoreText[30];
    char LivesText[15];
    SetSoundVolume(Pickup, 0.5);
    SetSoundVolume(Hurt, 0.5);

    if (*PauseGame == 0) {    
        if (IsKeyPressed(KEY_T)) {
            if (*KeyboardInput == 1) *KeyboardInput = 0;
            if (*KeyboardInput == 0) *KeyboardInput = 1;
        }

        for (int i = 0; i < 15; i++) {
            if (circles[i].y < ScreenHeigth && *GameOver == 0) {
                circles[i].y += 250 * Delta;
            }
            else {
                float CircleX = GetRandomValue(0, ScreenWidth + 25);
                circles[i].x = CircleX;
                circles[i].y = GetRandomValue(-25, 0);
            }
        }

        for (int i = 0; i < 5; i++) {
            if (BadCircles[i].y < ScreenHeigth && *GameOver == 0) {
                BadCircles[i].y += 250 * Delta;
            }
            else {
                BadCircles[i].x = GetRandomValue(0, ScreenWidth);
                BadCircles[i].y = GetRandomValue(-25, 0);
            }
        }

        for (int i = 0; i < 15; i++) {
            if (CheckCollisionCircles(circles[i], 25, (Vector2) {(float) *PlayerX, (float) PlayerY}, 25)) {
                *Score += 1;
                circles[i].x = GetRandomValue(0, ScreenWidth);
                circles[i].y = GetRandomValue(-25, 0);
                if (*PlaySFX == 1) PlaySound(Pickup);
            }
        }
            
        for (int i = 0; i < 5; i++) {
            if (CheckCollisionCircles(BadCircles[i], 25, (Vector2) {(float) *PlayerX, (float) PlayerY}, 25)) {
                *Lives -= 1;
                BadCircles[i].x = GetRandomValue(0, ScreenWidth);
                BadCircles[i].x = GetRandomValue(-25, 0);
                if (*PlaySFX == 1) PlaySound(Hurt);
            }
        }
        if (*KeyboardInput == 1) {
            if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && *PlayerX - 25 > 0) *PlayerX -= 250 * Delta;
            if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && *PlayerX + 25 < ScreenWidth) *PlayerX += 250 * Delta;
        }
        else if (*KeyboardInput == 0) {
            Vector2 MousePosVec = GetMousePosition();
            if (MousePosVec.x > *PlayerX) *PlayerX += 250 * Delta;
            if (MousePosVec.x < *PlayerX) *PlayerX -= 250 *Delta;
        }
        }
    
    *GameOver = 0;

    BeginDrawing();

    ClearBackground(WHITE);

    for (int i = 0; i < 15; i++) {
        DrawCircleV(circles[i], 25, RED);
    }

    for (int i = 0; i < 5; i++) {
        DrawCircleV(BadCircles[i], 25, ORANGE);
    }

    DrawCircle(*PlayerX, PlayerY, 25, BLACK);

    sprintf(ScoreText, "Score %d", *Score);
    DrawText(ScoreText, ScreenWidth / 9, ScreenHeigth / 9, 35, BLACK);

    sprintf(LivesText, "Lives Left %d", *Lives);
    DrawText(LivesText, (ScreenWidth / 9) * 6, ScreenHeigth / 9, 35, BLACK);

    if (*FPSDraw == 1){
        DrawFPS(ScreenWidth / 10, ScreenHeigth / 10 * 9);
    }
    if (*PauseGame == 1) {
        DrawText("PAUSED", ScreenWidth / 2, ScreenHeigth / 2, 35, BLACK);
    }

    EndDrawing();
}

void MainMenu() {
    BeginDrawing();
    ClearBackground(WHITE);

    DrawCircle(GetScreenWidth() / 2 -150, GetScreenHeight() / 2 - 60, 25, RED);
    DrawCircle(GetScreenWidth() / 2 - 50, GetScreenHeight() / 2 - 70, 25, ORANGE);
    DrawCircle(GetScreenWidth() / 2 + 60, GetScreenHeight() / 2 - 120, 25, RED);

    DrawText("Catch the Circles", GetScreenWidth() / 2 - 250, GetScreenHeight() / 2, 60, BLACK);

    DrawText("Press P To Start", GetScreenWidth() / 2 - 130, GetScreenHeight() / 2 + 100, 35, BLACK);

    EndDrawing();
}

void GameOverScreen(int *Score, int *HighScore) {
    char HighScoreText[30];
    char ScoreText[30];
    sprintf(HighScoreText, "High Score %d", *HighScore);
    sprintf(ScoreText, "Score: %d", *Score);

    BeginDrawing();
    ClearBackground(WHITE);

    DrawText("GAME OVER", GetScreenWidth() / 2 - 130, GetScreenHeight() / 2, 35, BLACK);

    DrawText("Press P to Restart", GetScreenWidth() / 2 - 130, GetScreenHeight() / 2 + 50, 35, BLACK);

    DrawText(ScoreText, 100, 25, 35, BLACK);
    DrawText(HighScoreText, GetScreenWidth() / 20 * 12, 25, 35, BLACK);

    EndDrawing(); 
}

void PauseScreen() {
    BeginDrawing();
    ClearBackground(WHITE);

    DrawText("PAUSED", GetScreenWidth() / 2 -100, GetScreenHeight() / 2, 35, BLACK);
    
    EndDrawing();
}

int main(void) {
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 500, "Catch the Circles");

    int PlayerX = 250;
    Vector2 circles[15];
    for (int i = 0; i < 15; i++) {
        float circleX = GetRandomValue(0, GetScreenWidth() + 25);
        float CircleY = GetRandomValue(-25, 0);
        circles[i] = (Vector2){circleX, CircleY};
    }
    Vector2 BadCircles[5];
    for (int i = 0; i < 5; i++) {
        BadCircles[i] = (Vector2){(float) GetRandomValue(0, GetScreenWidth()), (float) GetRandomValue(-25, 0)};
    }
    bool GamePlaying = false;
    int GameOver = 0;
    bool PlayMus = true;
    int PlaySFX = 1;
    int HighScore;

    FILE *scorefiler = fopen("score.txt", "r");
    if (scorefiler) {fscanf(scorefiler, "%d", &HighScore); fclose(scorefiler);}
    else {HighScore = 0;}

    int Score = 0;
    int Lives = 5;
    int PauseGame = 1;
    int KeyboardInput = 0;
    int FPSDraw = 0;

    InitAudioDevice();

    Sound Pickup = LoadSound("apple.wav");
    Sound Hurt = LoadSound("hurt.wav");
    Music Music1 = LoadMusicStream("music1.wav");
    Music Music2 = LoadMusicStream("music2.ogg");

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_M)) {
            if (PlayMus == true) {
                PlayMus = false;
            }
            else {
                PlayMus = true;
            }
        }
        if (IsKeyPressed(KEY_N)) {
            if (PlaySFX == 1) PlaySFX = 0;
            else PlaySFX = 1;
        }
        
        if (PlayMus == true) {
            int MusicPick = GetRandomValue(0, 100);
            if (MusicPick < 51) {
                UpdateMusicStream(Music1);
                PlayMusicStream(Music1);
            }
            else {
                UpdateMusicStream(Music2);
                PlayMusicStream(Music2);
            }
            SetMusicVolume(Music1, 0.25);
            SetMusicVolume(Music2, 0.25);
        }

        if (IsKeyPressed(KEY_F)) {
            
            if (PauseGame == 0) {
                PauseGame = 1;
            }
            else {
                PauseGame = 0;
            }
        }

        if (IsKeyPressed(KEY_X)) {
            if (FPSDraw == 0) FPSDraw = 1;
            if (FPSDraw == 1) FPSDraw = 0;
        }

        if (Lives == 0) GameOver = 1;
        if (GamePlaying == false) {
            if (IsKeyPressed(KEY_P)) GamePlaying = true;
            MainMenu();
        }

        else if (GameOver == 1) {
            if (HighScore == 0 || Score > HighScore) HighScore = Score;
            GameOverScreen(&Score, &HighScore);
            if (IsKeyDown(KEY_P)) {
                Score = 0;
                Lives = 5;
                GameIsPlaying(
                &PlayerX,
                circles,
                BadCircles,
                &Score,
                &Lives,
                Pickup,
                Hurt,
                &PlaySFX,
                &KeyboardInput,
                &FPSDraw,
                &GameOver,
                &PauseGame
            );
            }
        }
        else if (GamePlaying == true) {
            GameIsPlaying(
                &PlayerX,
                circles,
                BadCircles,
                &Score,
                &Lives,
                Pickup,
                Hurt,
                &PlaySFX,
                &KeyboardInput,
                &FPSDraw,
                &GameOver,
                &PauseGame
            );
        }
        
    }

    FILE *scorefilew = fopen("score.txt", "w");
    fprintf(scorefilew, "%d", HighScore);
    fclose(scorefilew);
    CloseWindow();

    return 0;
}