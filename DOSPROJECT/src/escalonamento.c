#include "../include/escalonamento.h"
#include <stdio.h>
#include <stdlib.h>

#define PILHA_TAMANHO 4096



uint8_t* pilha1;
uint8_t* pilha2;

Context tarefa1_ctx;
Context tarefa2_ctx;
void tarefa1() {
    while (1) {
        printf("Tarefa 1\n");
        Sleep(1000);
        switch_context(&tarefa1_ctx, &tarefa2_ctx);
    }
}

void tarefa2() {
    while (1) {
        printf("Tarefa 2\n");
        Sleep(1000);
        switch_context(&tarefa2_ctx, &tarefa1_ctx);
    }
}
