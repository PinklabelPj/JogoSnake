#include "snake.h"
#include "maps.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
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
    jogo.num_players = 0; // inicializa contagem de players ao iniciar o jogo
    int gameOver = 1;
    int Menu = 1; // 1 -> menu, 0 -> jogo rodando
    int menuSelection = 0; // 0 -> Novo Jogo, 1 -> Ranking, 2 -> Sair

    //Cria a janela do jogo
    InitWindow(LARGURA, ALTURA, "Snake Game");
    SetTargetFPS(60);
    srand(time(NULL));
    InitAudioDevice(); 
    SetMasterVolume(1.0f);
    jogo.backgroundMusic = LoadMusicStream("Assets/BackgroundSound.mp3"); 
    PlayMusicStream(jogo.backgroundMusic);    
    SetMusicVolume(jogo.backgroundMusic, 0.1f);
    jogo.apple = LoadTexture("Assets/apple.png");
    jogo.eatSound = LoadSound("Assets/Eat_Apple.mp3");    
    SetSoundVolume(jogo.eatSound, 1.0f);
    jogo.colisaoSound = LoadSound("Assets/Colisão_GameOver.mp3");
    SetSoundVolume(jogo.colisaoSound, 0.5f); 
    

    // Inicializa
    IniciaJogo(&jogo);
    IniciaJogador(&jogo);
    

    while (!WindowShouldClose()){
        UpdateMusicStream(jogo.backgroundMusic);
        BeginDrawing();
        ClearBackground(BLACK);
        

        if (Menu){
            //menu
            if (IsKeyPressed(KEY_DOWN)){
                menuSelection = (menuSelection + 1) % 3;
            }
            if (IsKeyPressed(KEY_UP)){
                menuSelection = (menuSelection + 2) % 3; // -1 mod 3
            }

            // Desenha o menu
            DrawText("SNAKE GAME", 200, 80, 40, GREEN);
            if (menuSelection == 0) {
                DrawText("Novo Jogo", 220, 200, 30, WHITE);
            } else {
                DrawText("  Novo Jogo", 220, 200, 30, WHITE);
            }
            
            if (menuSelection == 1) {
                DrawText("Ranking", 220, 260, 30, WHITE);
            } else {
                DrawText("  Ranking", 220, 260, 30, WHITE);
            }
            
            if (menuSelection == 2) {
                DrawText("Sair", 220, 320, 30, WHITE);
            } else {
                DrawText("  Sair", 220, 320, 30, WHITE);
            }
            

            if (IsKeyPressed(KEY_ENTER)){// seleciona a opção do menu
                if (menuSelection == 0){
                    Desaloca(&jogo);
                    IniciaJogo(&jogo);
                    while(!WindowShouldClose()){
                        UpdateMusicStream(jogo.backgroundMusic);
                        BeginDrawing();
                        ClearBackground(BLACK);
                        DrawText("Nome do Jogador:", LARGURA/3.4, ALTURA/4, 30, WHITE);
                        DrawText(jogo.jogador.nickname, LARGURA/3.4, ALTURA/2.5, 25, WHITE);
                        int letra = GetCharPressed();
                            if(letra){
                            jogo.jogador.nickname[jogo.jogador.tam] = letra;
                            jogo.jogador.tam++;
                            
                        } else if (IsKeyPressed(KEY_BACKSPACE) && jogo.jogador.tam >= 0){
                            jogo.jogador.nickname[jogo.jogador.tam] = '\0';
                            jogo.jogador.tam--;
                        }                        
                        EndDrawing();
                        if(IsKeyPressed(KEY_ENTER)){
                            break;
                        } 
                    }
            
        
                        gameOver = 1;
                        Menu = 0;
                } else if (menuSelection == 1){
                    // Vai para tela de ranking até o usuário apertar ENTER
                    while(!WindowShouldClose()){
                        UpdateMusicStream(jogo.backgroundMusic);
                        BeginDrawing();
                        ClearBackground(BLACK);
                        DrawText("Ranking", 260, 20, 50, YELLOW);

                        IniciaRank(&jogo); // desenha na aba rank no menu

                        EndDrawing();
                        if (IsKeyPressed(KEY_ENTER) || WindowShouldClose()) break;
                    }
                } else if (menuSelection == 2){
                    // sai do jogo
                    break;
                }
            }

        } else {
            // jogo rodando
            if (gameOver){
                DesenhaJogo(&jogo);
                AtualizaRodada(&jogo);
                ColisaoFood(&jogo);
                if(Colisaocobra(&jogo) == 1){
                    gameOver = 0;
                }
                if(ColisaoMapa(&jogo) == 1){//se bate em barreira, perde
                    gameOver = 0;
                }
            } else {
                DrawText("Game Over", 205, 200, 50, RED);
                DrawText(TextFormat("%s", jogo.jogador.nickname), 120, 310, 20, WHITE);
                DrawText(TextFormat("Pontos: %d", jogo.jogador.pontos), 120, 350, 20, WHITE);
                DrawText("Pressione Enter para retornar ao menu", 120, 400, 20, WHITE);
                if (IsKeyPressed(KEY_ENTER)){
                    // salva ranking (arquivo + vetor em memória) antes de resetar o jogo (ajuda do miguelito)
                    SalvaRanking(&jogo, "ranking.txt");
                    Desaloca(&jogo);
                    IniciaJogo(&jogo);
                    gameOver = 1;
                    Menu = 1; // volta pro o menu
                }
            }
        }

        EndDrawing();
    }
    
    UnloadMusicStream(jogo.backgroundMusic);
    UnloadSound(jogo.eatSound);
    UnloadSound(jogo.colisaoSound);
    CloseAudioDevice();
    UnloadTexture(jogo.apple);
    Desaloca(&jogo);
    CloseWindow();
    return 0;
}
