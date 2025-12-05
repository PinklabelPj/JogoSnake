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
#define MAX_PLAYERS 21



// renomeado a variaveis para receber o corpo da cobra como lista j->snake->body
void IniciaBody(Jogo *j){
    j->snake = (Snake)malloc(sizeof(Celula));
    if(j->snake == NULL){
        return;
    }
    j->snake->body.pos = (Rectangle) {LARGURA/2 - STD_SIZE_X, ALTURA - STD_SIZE_Y -10, STD_SIZE_X, STD_SIZE_Y};
    j->snake->body.direcao = 0;
    j->snake->body.color = SNAKE_COLOR;
    j->snake->prox = NULL; // cabeça da cobra 
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
    IniciaMapaAleatorio(j); // escolhe e inicializa um mapa aleatório
    IniciaFoodSeguro(j);    // pra n ficar comida em cima da barreira
    IniciaJogador(j);

    j->tempo = GetTime();
}

// renomeado a variaveis para receber o corpo da cobra como lista j->snake->body
void DesenhaBody(Jogo *j){
    Celula* atual = j->snake;
    while(atual != NULL){
        DrawRectangleRec(atual->body.pos, atual->body.color);
        atual = atual->prox; // move pro prox no da lista
    }
    

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
    DesenhaMapa(j);
    DesenhaBody(j);
    DesenhaFood(j);
    DrawText(TextFormat("Pontuação: %d", j->jogador.pontos), 10, 10, 20, PINK);
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
    if(j->snake == NULL){
        return;
    }
    // salvando posições antigas da cabeça (usando tipo rectangle da raylib para criar variavel com xy)
    Rectangle Posicao_Anterior = j->snake->body.pos; // posicao da cabeça
    int Direcao_Anterior = j->snake->body.direcao; // direcao da cabeça
    // aux pra salvar posições e direções antigas
    Rectangle Aux_Pos_Ant;
    int Aux_Direcao_Ant;

    // move a cabeça de acordo com a direção
    if (j->snake->body.direcao == 0){
        j->snake->body.pos.y -= STD_SIZE_Y;
    }
    if (j->snake->body.direcao == 1){
        j->snake->body.pos.x += STD_SIZE_X;
    }
    if (j->snake->body.direcao == 2){
        j->snake->body.pos.y += STD_SIZE_Y;
    }
    if (j->snake->body.direcao == 3){
        j->snake->body.pos.x -= STD_SIZE_X;
    } 

    if(j->snake->prox != NULL){
        Celula* atual = j->snake->prox; // apontando pro corpo (prox nó dps da cabeça)
        while(atual != NULL){
            Aux_Pos_Ant = atual->body.pos; // salvando pos do corpo
            Aux_Direcao_Ant = atual->body.direcao; // save direcao do corpo

            atual->body.pos = Posicao_Anterior; 
            atual->body.direcao = Direcao_Anterior;

            Posicao_Anterior = Aux_Pos_Ant;
            Direcao_Anterior = Aux_Direcao_Ant;

            atual = atual->prox; // att o rabo da cobra 
        }
    }
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
        j->jogador.pontos += 1;
        AumentaBody(j);
        IniciaFoodSeguro(j); // gera nova comida sem colisão com barreiras
        return 1;
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
    Celula* NovoPos =(Celula*)malloc(sizeof(Celula));
    if(NovoPos == NULL){
        return;
    }
    
    
    NovoPos->body.color = SNAKE_COLOR;
    NovoPos->prox = NULL; // final da lista
    Celula* atual = j->snake;
    while(atual->prox != NULL){
        atual = atual->prox; // percorre ate o rabo
    }

    NovoPos->body.pos = atual->body.pos; 
    NovoPos->body.direcao = atual->body.direcao; 
    int direcao_rabo = atual->body.direcao;
    if(direcao_rabo == 0){
        NovoPos->body.pos.y += STD_SIZE_Y;
    } 
    else if(direcao_rabo == 1){
        NovoPos->body.pos.x -= STD_SIZE_X;
    } 
    else if(direcao_rabo == 2){
        NovoPos->body.pos.y -= STD_SIZE_Y;
    } 
    else if(direcao_rabo == 3){
        NovoPos->body.pos.x += STD_SIZE_X;
    } 
    atual->prox = NovoPos;

    }

    int Colisaocobra(Jogo *j){
        Celula* cabeca = j->snake;
        Celula* atual = j->snake->prox; // começa do corpo
        while(atual != NULL){
            if(CheckCollisionRecs(cabeca->body.pos, atual->body.pos)){
                return 1;
            }
            atual = atual->prox;
        }
        return 0;
    }

void Desaloca(Jogo *j){
    Celula* atual = j->snake;
    Celula* prox;
    while(atual != NULL){
        prox = atual->prox;
        free(atual);
        atual = prox;
    }
}

void IniciaJogador(Jogo *j){
    j->jogador.pontos = 0;
    j->jogador.tam = 0;
    for(int i=0; i < 32; i++){
        j->jogador.nickname[i] = '\0';
    }
}

void QuickSortPlayers(Jogo *j, int left, int right){
    if (j == NULL || left >= right) return;

    int i = left;
    int k = right;
    int pivot = j->players[(left + right) / 2].pontos;

    while (i <= k) {
        while (j->players[i].pontos > pivot) i++;
        while (j->players[k].pontos < pivot) k--;

        if (i <= k) {
            Jogador tmp = j->players[i];
            j->players[i] = j->players[k];
            j->players[k] = tmp;
            i++; k--;
        }
    }

    if (left < k) QuickSortPlayers(j, left, k);
    if (i < right) QuickSortPlayers(j, i, right);
}

void IniciaRank(Jogo *j){
    FILE *pf=fopen("ranking.txt","r");
    char linha[25];
    for(int i=0; i<20;i++){
        fgets(linha,25,pf);
        strcpy(j->players[i].nickname, strtok(linha, " "));
        j->players[i].pontos = atoi(strtok(NULL, " "));

    }
    fclose(pf);
}

void SalvaRanking(Jogo *j, const char *ranking){
    if (j == NULL) return;

    FILE *fr = fopen("ranking.txt", "r");
    char linha[128]; // quantidade de caracteres por linha
    int count = 0;

    /* Carrega até MAX_PLAYERS-1 jogadores do arquivo */
    if (fr != NULL){
        // lendo linha por linha do arquivo
        while (count < MAX_PLAYERS - 1 && fgets(linha, sizeof(linha), fr) != NULL){ 
            char *nick = strtok(linha, " "); 
            char *pts  = strtok(NULL, " ");
            if (nick != NULL && pts != NULL){
                strcpy(j->players[count].nickname, nick);
                j->players[count].nickname[sizeof(j->players[count].nickname) - 1] = '\0';
                j->players[count].pontos = atoi(pts);
                j->players[count].tam = 0;
                count++;
            }
        }
        fclose(fr);
    }

    /* limpa slots não usados */
    for (int i = count; i < MAX_PLAYERS - 1; i++){
        j->players[i].nickname[0] = '\0';
        j->players[i].pontos = 0;
        j->players[i].tam = 0;
    }


    /* Coloca o jogador atual sempre na posição 21 (índice 20) */
    int ultimo = MAX_PLAYERS - 1;
    strncpy(j->players[ultimo].nickname, j->jogador.nickname, sizeof(j->players[ultimo].nickname) - 1);
    j->players[ultimo].nickname[sizeof(j->players[ultimo].nickname) - 1] = '\0';
    j->players[ultimo].pontos = j->jogador.pontos;
    j->players[ultimo].tam = j->jogador.tam;

    /* Ordena os MAX_PLAYERS por pontos decrescentes */
    QuickSortPlayers(j, 0, ultimo);

    /* Atualiza número de players em memória (até MAX_PLAYERS) */
    j->num_players = MAX_PLAYERS;

    /* Regrava o arquivo com os top MAX_PLAYERS-1 (mantendo arquivo com os melhores 20) */
    FILE *fw = fopen("ranking.txt", "w");
    if (fw != NULL){
        int gravacao = 0; // para contar quantas linhas foram gravadas no arquivo
        for (int i = 0; i < MAX_PLAYERS && gravacao < MAX_PLAYERS - 1; i++){
            if (j->players[i].nickname[0] == '\0'){ // pula nicknames vazios
                continue;
            }
            fprintf(fw, "%s %d\n", j->players[i].nickname, j->players[i].pontos);
            gravacao++;
        }
        fclose(fw);
    }
}







