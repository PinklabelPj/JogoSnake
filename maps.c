#include "maps.h"
#include "raylib.h"
#include <stdlib.h>
// máximo de obstáculos por mapa pra n desenhar no mapa inteiro
#define MAX_OBS 128
static Rectangle obstaculos[MAX_OBS];
static int obst_count = 0;
//static evita conflito com os outros arquivos, como criei um .c e .h separado, foi melhor usar ele

// cria um Rectangle alinhado à grade, funcao teve q ser pesquisada
static Rectangle GradeRetangulo(int gx, int gy){
    Rectangle r;
    r.width = STD_SIZE_X;
    r.height = STD_SIZE_Y;
    r.x = 10 + gx * STD_SIZE_X;
    r.y = 10 + gy * STD_SIZE_Y;
    return r;
}

// preenche obstaculos
static void Mapa1(void){
    obst_count = 0;
    //desenho de cruz no centro
    int coords[][2] = {
        {8,3},{8,4},{8,5},{8,6},{8,7},{8,8},{8,9},{8,10},
        {3,8},{4,8},{5,8},{6,8},{7,8},{9,8},{10,8},{11,8}
    };
    int n = sizeof(coords)/sizeof(coords[0]);
    for(int i=0;i<n && i<MAX_OBS;i++){
        obstaculos[obst_count++] = GradeRetangulo(coords[i][0], coords[i][1]);
    }
}

static void Mapa2(void){
    obst_count = 0;
    int coords[][2] = {
        {2,4},{3,4},{4,4},{5,4},{6,4},
        {9,5},{10,5},{11,5},
        {4,9},{5,9},{6,9},{7,9},
        {10,11},{10,10},{10,9}
    };
    int n = sizeof(coords)/sizeof(coords[0]);
    for(int i=0;i<n && i<MAX_OBS;i++){
        obstaculos[obst_count++] = GradeRetangulo(coords[i][0], coords[i][1]);
    }
}

// aleatoriedade dos mapa
void IniciaMapaAleatorio(Jogo *j){
    (void)j; 
    int mapa = rand() % 2;
    if(mapa == 0) Mapa1();
    else Mapa2();

    //se alguma barreira estiver onde a cabeça inicia vai ser tirada
    Rectangle cabecaStart = (Rectangle){ (float)(LARGURA/2 - STD_SIZE_X), (float)(ALTURA - STD_SIZE_Y - 10), (float)STD_SIZE_X, (float)STD_SIZE_Y };
    int w = 0;
    for(int i=0;i<obst_count;i++){//aqui ta contando um por um ne
        if (!CheckCollisionRecs(obstaculos[i], cabecaStart)){
            obstaculos[w++] = obstaculos[i];
        }
    }
    obst_count = w;
}

void DesenhaMapa(Jogo *j){
    (void)j;
    for(int i=0;i<obst_count;i++){
        DrawRectangleRec(obstaculos[i], PURPLE);
    }
}

int ColisaoMapa(Jogo *j){ // verifica se a cabeça da cobra nasce em um obstáculo
    if(j==NULL || j->snake==NULL) return 0;
    Rectangle cabeca = j->snake->body.pos;
    for(int i=0;i<obst_count;i++){
        if(CheckCollisionRecs(cabeca, obstaculos[i])){
            // som para colisão com a cobra
            PlaySound(j->colisaoSound);
            return 1;
        }
    }
    return 0;
}

// verifica se a cobra colide com algum obstáculo
static int CobraColisao(Rectangle r){
    for(int i=0;i<obst_count;i++){
        if(CheckCollisionRecs(r, obstaculos[i])) return 1;
    }
    return 0;
}


void IniciaFoodSeguro(Jogo *j){
    if(j==NULL) return;
    do {
        IniciaFood(j); 
    } while(CobraColisao(j->food.pos));
}
