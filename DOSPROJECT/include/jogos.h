#ifndef JOGOS_H_INCLUDED
#define JOGOS_H_INCLUDED
//Sons de jogo
void som_game_over_8bit();
void som_menu_fliperama();
void som_acerto_j();
void som_menu();
void beeps_boot();
//
void gotoxy(int x, int y);
void atualiza();
int analiza();
void geraComida();
void desenha();
void gotoxy(int x, int y);
extern int c[300][2], pontos, cx, cy;
extern int comida[2], velo;
void resetar_jogo();


#endif // JOGOS_H_INCLUDED
