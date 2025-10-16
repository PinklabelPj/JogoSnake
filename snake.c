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
#define SNAKE_COLOR YELLOW
#define FOOD_COLOR BLUE




// renomeado a variaveis para receber o corpo da cobra como lista j->snake->body
void IniciaBody(Jogo *j){
    j->snake = (Snake)malloc(sizeof(Celula));
    if(j->snake == NULL){

    }
    j->snake->body.pos = (Rectangle) {LARGURA/2 - STD_SIZE_X, ALTURA - STD_SIZE_Y -10, STD_SIZE_X, STD_SIZE_Y};
    j->snake->body.direcao = 0;
    j->snake->body.color = SNAKE_COLOR;
    }

void IniciaBordas(Jogo *j){
    //Borda de cima
    j->bordas[0].pos = (Rectangle) {0, 0, LARGURA, 10};
    //Borda da direita
    j->bordas[1].pos = (Rectangle) {LARGURA - 10, 0, 10, ALTURA};
    //Borda de baixo
    j->bordas[2].pos = (Rectangle) {0, ALTURA - 10, LARGURA, 10};
    //Borda da esquerda
    j->bordas[3].pos = (Rectangle) {0, 0, 10, ALTURA};
}

void IniciaFood(Jogo *j){
    j->food.pos = (Rectangle) {(float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), (float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), STD_SIZE_X, STD_SIZE_Y};
    j->food.color = FOOD_COLOR;
}

void IniciaJogo(Jogo *j){
    IniciaBordas(j);
    IniciaBody(j);
    IniciaFood(j);

    j->tempo = GetTime();
}

// renomeado a variaveis para receber o corpo da cobra como lista j->snake->body
void DesenhaBody(Jogo *j){
    DrawRectangleRec(j->snake->body.pos, j->snake->body.color);

}

void DesenhaFood(Jogo *j){
    DrawRectangleRec(j->food.pos, j->food.color);
}

void DesenhaBordas(Jogo *j){
    //Desenha as barreiras nas bordas
    for (int i = 0; i < 4; i++){
        DrawRectangleRec(j->bordas[i].pos, LIGHTGRAY);
    }
}

void DesenhaJogo(Jogo *j){
    DesenhaBordas(j);
    DesenhaBody(j);
    DesenhaFood(j);
}

// renomeado a variaveis para receber o corpo da cobra como lista j->snake->body
void AtualizaDirecao(Jogo *j){
    //Atualiza para qual direção a cobra vai  
    if(IsKeyDown(KEY_UP) && j->snake->body.direcao != 2 && GetTime() - j->cooldown > COOLDOWN){
        j->snake->body.direcao = 0;
        j->cooldown = GetTime();
    }
    if(IsKeyDown(KEY_RIGHT) && j->snake->body.direcao != 3 && GetTime() - j->cooldown > COOLDOWN){
        j->snake->body.direcao = 1;
        j->cooldown = GetTime();
    }
    if(IsKeyDown(KEY_DOWN) && j->snake->body.direcao != 0 && GetTime() - j->cooldown > COOLDOWN){
        j->snake->body.direcao = 2;
        j->cooldown = GetTime();
    }
    if(IsKeyDown(KEY_LEFT) && j->snake->body.direcao != 1 && GetTime() - j->cooldown > COOLDOWN){
        j->snake->body.direcao = 3;
        j->cooldown = GetTime();
    }
}

// renomeado a variaveis para receber o corpo da cobra como lista j->snake->body
void AtualizaPosBody(Jogo *j){
// Precisa Implementar a lista para inserir no primeiro e apagar no ultimo

    /*if (j->snake->body.direcao == 0){
        j->snake->body.pos. -= STD_SIZE_Y;
    }
    if (j->snake->body.direcao == 1){
        j->snake->body.pos.x += STD_SIZE_X;
    }
    if (j->snake->body.direcao == 2){
        j->snake->body.pos.y += STD_SIZE_Y;
    }
    if (j->snake->body.direcao == 3){
        j->snake->body.pos.x -= STD_SIZE_X;
    } */
}

void AtualizaRodada(Jogo *j){
    AtualizaDirecao(j);
    if (GetTime() - j->tempo > TEMPO){
        AtualizaPosBody(j);
        j->tempo = GetTime();
        j->cooldown = COOLDOWN;
    }
}

// função para resetar comidas quando for comida check
// renomeado a variaveis para receber o corpo da cobra como lista j->snake->body
int ColisaoFood(Jogo *j){
    if (CheckCollisionRecs(j->snake->body.pos, j->food.pos)){
        IniciaFood(j);
        AumentaBody(j);
    }
    return 0;
}

// função para colidir com a borda check
// renomeado a variaveis para receber o corpo da cobra como lista j->snake->body
int ColisaoBorda(Jogo *j){
    if(CheckCollisionRecs(j->bordas[1].pos, j->snake->body.pos) || CheckCollisionRecs(j->bordas[2].pos, j->snake->body.pos) || CheckCollisionRecs(j->bordas[3].pos, j->snake->body.pos) || CheckCollisionRecs(j->bordas[0].pos, j->snake->body.pos) ){
        return 1;
    }
    return 0;
}

// função para aumentar o tamanho da cobra ao contrario do que ela come a maçã
void AumentaBody(Jogo *j){
    Celula* NovoPos = (Celula*)malloc(sizeof(Celula));
    NovoPos->body.color = SNAKE_COLOR;

    if (j->snake->body.direcao == 0){
        NovoPos->body.pos.y += STD_SIZE_Y;
    }

    if (j->snake->body.direcao == 1){
        NovoPos->body.pos.x -= STD_SIZE_X;
    }

    if (j->snake->body.direcao == 2){
        NovoPos->body.pos.y -= STD_SIZE_Y;
    }
    
    if (j->snake->body.direcao == 3){
        NovoPos->body.pos.x += STD_SIZE_X;
    }
    NovoPos->body.prox = NULL;
}



