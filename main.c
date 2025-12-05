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
    jogo.num_players = 0; // inicializa contagem de players ao iniciar o programa
    int gameOver = 1;
    int Menu = 1; // 1 -> menu, 0 -> jogo rodando
    int menuSelection = 0; // 0: Start, 1: Instructions, 2: Exit

    //Cria a janela;
    InitWindow(LARGURA, ALTURA, "Snake Game");
    SetTargetFPS(60);
    srand(time(NULL));

    // Inicializa uma vez; quando o jogador começar, chama IniciaJogo novamente
    IniciaJogo(&jogo);
    IniciaJogador(&jogo);
    

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        

        if (Menu){
            // Navegação do menu
            if (IsKeyPressed(KEY_DOWN)){
                menuSelection = (menuSelection + 1) % 3;
            }
            if (IsKeyPressed(KEY_UP)){
                menuSelection = (menuSelection + 2) % 3; // -1 mod 3
            }

            // Desenha o menu
            DrawText("SNAKE GAME", 200, 80, 40, GREEN);
            DrawText(menuSelection == 0 ? "Novo Jogo" : "  Novo Jogo", 220, 200, 30, WHITE);
            DrawText(menuSelection == 1 ? "Ranking" : "  Ranking", 220, 260, 30, WHITE);
            DrawText(menuSelection == 2 ? "Sair" : "  Sair", 220, 320, 30, WHITE);
            

            if (IsKeyPressed(KEY_ENTER)){
                if (menuSelection == 0){
                    Desaloca(&jogo);
                    IniciaJogo(&jogo);
                    while(!WindowShouldClose()){
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
                        BeginDrawing();
                        ClearBackground(BLACK);
                        DrawText("Ranking", 200, 60, 40, LIGHTGRAY);
                        EndDrawing();
                        if (IsKeyPressed(KEY_ENTER) || WindowShouldClose()) break;
                    }
                } else if (menuSelection == 2){
                    // Sair do jogo
                    break;
                }
            }

        } else {
            // Jogo em execução
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
                if(ColisaoMapa(&jogo) == 1){//se bate em barreira, perde
                    gameOver = 0;
                }
            } else {
                DrawText("Game Over", 195, 300, 40, WHITE);
                DrawText(TextFormat("Pontos: %d", jogo.jogador.pontos), 230, 350, 20, WHITE);
                DrawText("Press ENTER to return to menu", 120, 400, 20, WHITE);
                if (IsKeyPressed(KEY_ENTER)){
                    /* salva ranking (arquivo + vetor em memória) antes de resetar o jogo */
                    SalvaRanking(&jogo, "ranking.txt");
                    Desaloca(&jogo);
                    IniciaJogo(&jogo);
                    gameOver = 1;
                    Menu = 1; // volta para o menu
                }
            }
        }

        EndDrawing();
    }

    Desaloca(&jogo);
    CloseWindow();
    Desaloca(&jogo);
    return 0;
}
