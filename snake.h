#ifndef SNAKE_H
#define SNAKE_H
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

typedef struct Bordas{
    Rectangle pos;
}Bordas;

typedef struct Body{
    Rectangle pos;
    Color color;
    int direcao;
}Body;

// Criado struct de celula para corpo da cobra
typedef struct Celula{
    Body body;
    struct Celula* prox; // att pra funcionar as funções
}Celula;

// renomeando um vetor de celula para snake
typedef Celula* Snake;

typedef struct Food{
    Rectangle pos;
    Color color;
}Food;

typedef struct Jogo{
    Snake snake;
    Food food;
    Bordas bordas[4];
    double tempo;
    double cooldown;
}Jogo;

void IniciaBody(Jogo *j);
void IniciaBordas(Jogo *j);
void IniciaFood(Jogo *j);
void IniciaJogo(Jogo *j);
void DesenhaBody(Jogo *j);
void DesenhaFood(Jogo *j);
void DesenhaBordas(Jogo *j);
void DesenhaJogo(Jogo *j);
void AtualizaDirecao(Jogo *j);
void AtualizaPosBody(Jogo *j);
void AtualizaRodada(Jogo *j);
int ColisaoFood(Jogo *j);
int ColisaoBorda(Jogo *j);
void AumentaBody(Jogo *j);
int Colisaocobra(Jogo *j);
void Desaloca(Jogo *j);


#endif
