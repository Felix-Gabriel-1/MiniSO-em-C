#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>  // Para kbhit() e getch()
//#include "include/verificainstall.h"
#define MAX_PATH 256

#define RAM_TOTAL 1024 * 1024  // 1MB


///==========================
///Area de include
#include "../include/verificainstall.h"
#include "../include/geren_mem.h"
#include "../include/funcgraficas.h"
#include "../include/jogos.h"
#include "../include/programas.h"
#include "../include/system.h"
///==========================

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

//Abrir arquivo usando o buffer
void xopen(const char *filename)
{
    /*if (tamanho +) > (0x9fff-0x05000)){
        printf("Arquivo muito grande para caber na RAM simulada.\n");
        fclose(file);
        return;
    }
    */
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Erro ao abrir arquivo: %s\n", filename);
        return;
    }

    // Descobrir tamanho do arquivo
    fseek(file, 0, SEEK_END);
    long tamanho = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Alocar memória na RAM virtual
    char *buffer = (char *) alocar_memoria(0x05000, 0x9FFFF, tamanho + 1);
    if (!buffer)
    {
        printf("Erro: Memória insuficiente para carregar o arquivo\n");
        fclose(file);
        return;
    }

    fread(buffer, 1, tamanho, file);
    buffer[tamanho] = '\0';
    printf("Conteúdo de %s:\n%s\n", filename, buffer);

    fclose(file);
    liberar_memoria(buffer);
}

void calculadora()
{
    int *n1 = (int *) alocar_memoria(0x00000, 0x9FFFF,sizeof(int));
    int *n2 = (int *) alocar_memoria(0x00000, 0x9FFFF,sizeof(int));
    int *res = (int *) alocar_memoria(0x00000, 0x9FFFF,sizeof(int));

    if (!n1 || !n2 || !res)
    {
        printf("Erro: Memória insuficiente na RAM simulada.\n");
        return;
    }

    printf("Escolha um valor: ");
    scanf("%d", n1);
    printf("Escolha outro valor: ");
    scanf("%d", n2);

    *res = *n1 + *n2;

    printf("%d + %d = %d\n", *n1, *n2, *res);

    liberar_memoria(n1);
    liberar_memoria(n2);
    liberar_memoria(res);
}
int calcasm(int a, int b, char op)
{
    int resultado1 = 0;
    __asm__ (
        "movl %[a], %%eax;"
        "movl %[b], %%ebx;"
        "movb %[op], %%cl;"

        "cmpb $'+', %%cl;"
        "je soma;"
        "cmpb $'-', %%cl;"
        "je sub;"
        "cmpb $'*', %%cl;"
        "je mult;"
        "cmpb $'/', %%cl;"
        "je div;"
        "jmp fim;"

        "soma:"
        "addl %%ebx, %%eax;"
        "jmp fim;"

        "sub:"
        "subl %%ebx, %%eax;"
        "jmp fim;"

        "mult:"
        "imull %%ebx, %%eax;"
        "jmp fim;"

        "div:"
        "cdq;"
        "idivl %%ebx;"
        "jmp fim;"

        "fim:"
        "movl %%eax, %[res];"

        : [res] "=r" (resultado1)
        : [a] "r" (a), [b] "r" (b), [op] "r" (op)
        : "%eax", "%ebx", "%ecx", "%edx"
    );

    return resultado1;
}
void ajuda()
{
    printf("--------------------------------------------------------------------\n");
    printf("Sair        | Encerra o programa\n");
    printf("--------------------------------------------------------------------\n");
    printf("Calc        | Abre a calculadora\n");
    printf("--------------------------------------------------------------------\n");
    printf("Calcasm     | Abre a calculadora (multiplicando em modo ASSEMBLY)\n");
    printf("--------------------------------------------------------------------\n");
    printf("Diretorio   | Mostra o diretorio atual\n");
    printf("--------------------------------------------------------------------\n");
    printf("xopen       | Abre algum arquivo\n");
    printf("--------------------------------------------------------------------\n");
    printf("xgfx        | Muda o visual (SO FUNCIONA EM MS-DOS EM MODO 16BITS\n");
    printf("--------------------------------------------------------------------\n");
    printf("md          | (entra nodiretorio desejado)\n");
    printf("--------------------------------------------------------------------\n");
    printf("ed          | (exibe os diretorios)\n");
    printf("--------------------------------------------------------------------\n");
    printf("md..        | (voltar no diretorio)\n");
    printf("--------------------------------------------------------------------\n");
    printf("versao      |(exibe a versao atual do sistema)\n");
    printf("--------------------------------------------------------------------\n");
    printf("mem.exe     |(Visualizador de memoria ram)\n");
    printf("--------------------------------------------------------------------\n");
    printf("free.exe    |(Limpa a memoria ram)\n");
    printf("--------------------------------------------------------------------\n");
    printf("dump.mem    |(Salva o estado atual da memoria ram)\n");
    printf("--------------------------------------------------------------------\n");
    system("pause");
    printf("--------------------------------------------------------------------\n");
    printf("adivinha.exe|(jogo de adivinhar o numero)\n");
    printf("--------------------------------------------------------------------\n");
    printf("editor      |(Abre o notpad do windows)\n");
    printf("--------------------------------------------------------------------\n");
    printf("memmaker/otimizarmem  |(Otimiza a memoria ram jogando para upper)\n");
    printf("--------------------------------------------------------------------\n");
    printf("memex       |(visualizador da memoria comum e upper)\n");
    printf("--------------------------------------------------------------------\n");
    printf("jogocobra.exe/cobra.exe|(visualizador da memoria comum e upper)\n");
    printf("--------------------------------------------------------------------\n");
    printf("cache.exe/memcache.exe |(utiliza a memoria cache)\n");
    printf("--------------------------------------------------------------------\n");
    printf("listacache.exe/cachelista.exe |(lista a memoria cache)\n");
    printf("--------------------------------------------------------------------\n");




}

void limpa_tela()
{
    system("cls");
}
int identificar_comando(const char* comando)
{
    if (strcmp(comando, "xopen") == 0) return 1;
    if (strcmp(comando, "xgfx") == 0) return 2;
    if (strcmp(comando, "diretorio") == 0 || strcmp(comando,"dir") == 0) return 3;
    if (strcmp(comando, "sair") == 0 ) return 4;
    if (strcmp(comando, "md..") == 0) return 5;
    if (strcmp(comando, "calc") == 0 || strcmp(comando,"calculadora") == 0) return 6;
    if (strcmp(comando, "ajuda") == 0) return 7;
    if (strcmp(comando, "calcasm") == 0 || strcmp(comando,"calculadora_assembly") == 0) return 8;
    if (strcmp(comando, "limpar") == 0) return 9;
    if (strcmp(comando, "versao") == 0) return 10;
    if (strcmp(comando, "md") == 0) return 11;
    if (strcmp(comando, "ed") == 0) return 12;
    if (strcmp(comando, "hora") == 0) return 13;
    if (strcmp(comando, "mem.exe") == 0) return 14;
    if (strcmp(comando, "free.exe") == 0) return 15;
    if (strcmp(comando, "alloc.exe") == 0) return 16;
    if (strcmp(comando, "dump.mem") == 0) return 17;
    if (strcmp(comando, "adivinha.exe") == 0) return 18;
    if (strcmp(comando, "editor") == 0) return 19;
    if (strcmp(comando, "memmaker") == 0 || strcmp(comando,"otimizarmem") == 0) return 20;
    if (strcmp(comando, "memex") == 0 || strcmp(comando,"exibirmem") == 0) return 21;
    if (strcmp(comando, "cobra.exe") == 0 || strcmp(comando,"jogocobra.exe") == 0) return 22;
    if (strcmp(comando, "memcache.exe") == 0 || strcmp(comando,"cache.exe") == 0) return 23;
    if (strcmp(comando, "listacache.exe") == 0 || strcmp(comando,"cachelista.exe") == 0) return 24;
    //if (strcmp(comando, "hash") == 0) return 13;
    return 0; // Comando desconhecido
}
