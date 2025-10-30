#include "snake.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define LARGURA 660
#define ALTURA 660
#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define TEMPO 0.2
#define COOLDOWN 0.2
#define SNAKE_COLOR GREEN
#define FOOD_COLOR RED

int main(){
    Jogo jogo;
    int gameOver = 1;

    //Cria a janela;
    InitWindow(LARGURA, ALTURA, "Snake Game");
    SetTargetFPS(60);
    srand(time(NULL));
    
    IniciaJogo(&jogo);
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        if (gameOver){
            DesenhaJogo(&jogo);
            AtualizaRodada(&jogo);
            ColisaoFood(&jogo); 
            if(Colisaocobra(&jogo) == 1){
                gameOver = 0;
            }
            if(ColisaoBorda(&jogo) == 1){
                gameOver = 0;
            }
        } else {
            DrawText("Game Over", 195, 300, 40, WHITE);
            if (IsKeyPressed(KEY_ENTER)){
                Desaloca(&jogo);
                IniciaJogo(&jogo);
                gameOver = 1;
            }
        }
        EndDrawing();
    }
    Desaloca(&jogo);
    CloseWindow();
    Desaloca(&jogo);
    return 0;
}
