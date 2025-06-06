#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
///==========================
///Area de include
#include "../include/verificainstall.h"
#include "../include/geren_mem.h"
#include "../include/funcgraficas.h"
#include "../include/jogos.h"
#include "../include/programas.h"
#include "../include/system.h"
///==========================
/* gepeto e stackoverflow salvou */
#define MAX_PATH 256


void relogio(){ //Beta depois arrumar trava sistema
      while (1) {
        time_t agora;
        struct tm *infoTempo;

        time(&agora);                  // Obtém o tempo atual
        infoTempo = localtime(&agora); // Converte para horário local

        system("cls");

        printf("Hora: %02d:%02d:%02d\n",
               infoTempo->tm_hour,
               infoTempo->tm_min,
               infoTempo->tm_sec);

        Sleep(1000); // 1 segundo
    }

}


void tipo_terminal(const char *texto)//Escreve como se fosse letra por letra
{
    while (*texto)
    {
        putchar(*texto++);
        fflush(stdout);
        Sleep(50);//ajustar velocidade
    }
    printf("\n");
}

void mostar_diretorio(){
    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        printf("Diretorio atual: %s\n", cwd);
    }
    else{
        perror("Erro ao obter diretório atual");
    }
}
void voltar_diretorio(){
    if(chdir("..")==0){
        //printf("voltando");
    }
    else{
        printf("erro diretorio inacessivel");
    }
}
void entrar_diretorio(const char* nome) {
    //printf("[DEBUG] Tentando entrar em: %s\n", nome);
    if (chdir(nome) == 0) {
        printf("Diretorio alterado para: %s\n", nome);
    } else {
        perror("Erro: diretorio nao existente");
    }
}


//esse aqui relmente não tenho a minima ideia de como replicar puro windows + algum toque meu
void listar_diretorio() {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("*", &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Erro ao listar diretório.\n");
        return;
    }

    printf("\nNome                          Tipo\n");
    printf("--------------------------------------------------\n");

    do {
        const char* tipo = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? "<DIR>" : "     ";
        printf("%-30s %s\n", findFileData.cFileName, tipo);
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void registrar_log(const char* comando) {
    FILE *log = fopen("log.sys", "a");
    if (log) {
        fprintf(log, "%s\n", comando); // salva o comando com quebra de linha
        fclose(log);
    } else {
        printf("Erro ao escrever no log.sys\n");
    }
}

