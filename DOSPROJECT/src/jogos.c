#include<stdio.h>
#include<stdlib.h> // Para fun��o srand() e system()
#include<windows.h> // Para fun��o SetConsoleCursorPosition()
#include<conio.h> // Para fun��o getch() e kbhit()
#include<time.h> // Para fun��o rand()

///==========================
///Area de include
#include "../include/verificainstall.h"
#include "../include/geren_mem.h"
#include "../include/funcgraficas.h"
#include "../include/jogos.h"
#include "../include/programas.h"
#include "../include/system.h"
///==========================

void som_game_over_8bit() {
    Beep(880, 100);
    Beep(830, 100);
    Beep(784, 100);
    Beep(740, 200);
    Beep(698, 300);
    Beep(622, 400);
}
void som_menu_fliperama() {
    Beep(1500, 40);
    Beep(1300, 40);
}
void som_acerto_j() {
    Beep(1000, 100);
    Beep(1300, 100);
    Beep(1600, 100);
}
void som_menu()
{
    Beep(700, 40);
}
void beeps_boot() {
    Beep(400, 100);
    Beep(600, 100);
    Beep(800, 100);
    Beep(1000, 100);
}

// Vari�veis Globais
int c[300][2], pontos=1, cx=2, cy=2;
int comida[2], velo=150;

// FUN��ES
void gotoxy(int x, int y){ // Fun��o
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}

void desenha(){ // Desenha a cobrinha
    int i;
    for(i=0; i<pontos; i++){
        gotoxy(c[i][0],c[i][1]);
        printf("%c",219);
    }
}

void atualiza(){ // Atualiza a posi��o da cobrinha
    int i;
    gotoxy(c[pontos][0],c[pontos][1]);
    printf(" ");
    for(i=pontos; i>=0; i--){
        c[i+1][0] = c[i][0];
        c[i+1][1] = c[i][1];
    }
}

int analiza(){ // V� se a cobrinha enconstou em seu pr�prio corpo
    int i, retorno=0;
    for(i=1; i<pontos; i++){
        if(cx==c[i][0] && cy==c[i][1]){
            retorno=1;
        }
    }
    return retorno;
}

void geraComida(){ // Gera comida em local aleat�rio
    gotoxy(comida[0],comida[1]);
    printf(" ");
    srand(time(NULL));
    comida[0] = (rand() % 48) +1;
    comida[1] = (rand() % 18) +1;
    gotoxy(comida[0],comida[1]);
    printf("%c",4);
}
void resetar_jogo(){
    pontos = 1;
    cx = 2;
    cy = 2;
    velo = 150;
    for (int i = 0; i<300; i++){
        c[i][0] = 0;
        c[i][1] = 0;
    }
}
