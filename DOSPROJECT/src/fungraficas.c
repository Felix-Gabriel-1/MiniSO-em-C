#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>  // Para kbhit() e getch()
///==========================
///Area de include
#include "../include/verificainstall.h"
#include "../include/geren_mem.h"
#include "../include/funcgraficas.h"
#include "../include/jogos.h"
#include "../include/programas.h"
#include "../include/system.h"
///==========================
//#include "include/verificainstall.h"
#define MAX_PATH 256

#define RAM_TOTAL 1024 * 1024  // 1MB

/*Area de memoria do programma
*/
#define INICIO_CONV   0x00000
#define FIM_CONV      0x9FFFF

#define INICIO_UPPER  0xA0000
#define FIM_UPPER     0xBFFFF

#define INICIO_ROM    0xC0000 //Desatinada a memoria cache
#define FIM_ROM       0xFFFFF

//Fim da area de mem
#define HASH_TAMANHO 128
void resetar_cor()
{
    mudar_cor(7, 0);
}

void versao ()
{
    printf("\n Versao 1.021 \n ");
    printf("\n por Gabriel Felix \n ");
}

void change_graphics()
{
    /*    Beep(750, 300);
        __asm__(
            "mov $0x13, %al \n"
            "mov $0x00, %ah \n"
            "int $0x10"
        );
        printf("Modo grafico alterado!\n");
        */
}
// Função para mudar a cor do texto no console
void mudar_cor(int texto, int fundo)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (fundo << 4) | texto);
}
