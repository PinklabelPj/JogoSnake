#ifndef MAPS_H
#define MAPS_H

#include "snake.h"

void IniciaMapaAleatorio(Jogo *j);   // randomiza e inicializa o mapa
void DesenhaMapa(Jogo *j);           // desenha as barreira
int ColisaoMapa(Jogo *j);            // ccolisão com as barreira
void IniciaFoodSeguro(Jogo *j);     


#endif
//diretorio proprio pra gente saber as funcoes mais rapido ja q ficou em cima do tempo pra ficar procurando 
