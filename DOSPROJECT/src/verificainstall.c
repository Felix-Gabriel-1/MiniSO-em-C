#include <windows.h>
#include <stdio.h>
#include <string.h>

///==========================
///Area de include
#include "../include/verificainstall.h"
#include "../include/geren_mem.h"
#include "../include/funcgraficas.h"
#include "../include/jogos.h"
#include "../include/programas.h"
#include "../include/system.h"
///==========================
/*void verificainstall() {
    char exePath[MAX_PATH];
    char basePath[MAX_PATH];
    char finalPath[MAX_PATH];

    // Obtém o caminho completo do executável
    GetModuleFileName(NULL, exePath, MAX_PATH);

    // Copia o caminho para manipular
    strcpy(basePath, exePath);

    // Remove \bin\Debug\meu_exe.exe
    char *lastSlash = strrchr(basePath, '\\');
    if (lastSlash) *lastSlash = '\0';  // Remove o nome do exe
    lastSlash = strrchr(basePath, '\\');
    if (lastSlash) *lastSlash = '\0';  // Remove Debug
    lastSlash = strrchr(basePath, '\\');
    if (lastSlash) *lastSlash = '\0';  // Remove bin

    snprintf(finalPath, MAX_PATH, "%s\\..\\Instalador\\bin\\Debug\\boot.txt", basePath);


    printf("Tentando abrir: %s\n", finalPath);  // DEBUG: Mostra o caminho usado

    FILE *arquivo = fopen(finalPath, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int a, b, c;
    if (fscanf(arquivo, "%d %d %d", &a, &b, &c) != 3) {
        printf("Erro ao ler os números.\n");
        fclose(arquivo);
        return;
    }

    fclose(arquivo);

    if (a == 1 && b == 0 && c == 0) {
        printf("Verificao de boot concluida...\n");

    } else {
        printf("Condição não atendida. Encerrando.\n");
        exit(1);
        return;
    }
}
*/
