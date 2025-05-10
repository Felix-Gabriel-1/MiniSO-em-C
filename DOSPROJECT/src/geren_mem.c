#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Inclui o cabeçalho com os protótipos
#include <windows.h>
#define MAX_BLOCOS 100
#define RAM_TOTAL (1024 * 1024) // 1 MB de memória simulada
#include <time.h>
#include <math.h>
/*Area de memoria do programma
*/
#define INICIO_CONV   0x00000
#define FIM_CONV      0x9FFFF

#define INICIO_UPPER  0xA0000
#define FIM_UPPER     0xBFFFF

#define INICIO_ROM    0xC0000
#define FIM_ROM       0xFFFFF

void bsod(const char* mensagem);
void som_acerto() {
    Beep(1000, 100);
    Beep(1500, 100);
}
void som_erro() {
    Beep(600, 200);
    Beep(400, 200);
}
void som_suspense() {
    for (int i = 800; i <= 1200; i += 100) {
        Beep(i, 100);
        Sleep(50); // pequena pausa entre os tons
    }
}

void jogo_adivinha()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    int numero_secreto, palpite, tentativas = 0;

    // Fosorio
    srand(time(NULL));
    numero_secreto = rand() % 100 + 1;  // número entre 1 e 100

    printf("===========================================================================\n");
    printf("                    Bem-vindo ao jogo da Adivinha!    \n");
    printf("                    Tente adivinhar o numero (1-100)  \n");
    printf("                           ver 0.12\n");
    printf("===========================================================================\n");

    while (1)
    {
        printf("Seu palpite: ");
        scanf("%d", &palpite);
        tentativas++;

        if (palpite < numero_secreto)
        {
            som_erro();
            printf("Muito baixo! Tente novamente.\n");
        }
        else if (palpite > numero_secreto)
        {
            som_acerto();
            printf("Muito alto! Tente novamente.\n");
        }
        else
        {
            som_suspense();
            printf("Parabens! Voce acertou em %d tentativas!\n", tentativas);
            break;
        }
    }
}

typedef struct
{
    void* endereco;
    size_t tamanho;
    int usado;
    unsigned int endereco_simulado;
} BlocoMemoria;

BlocoMemoria memoria[MAX_BLOCOS];
int total_blocos = 0;
//Gepteto salvou em comparar blocos realmente não sabia
int comparar_blocos(const void* a, const void* b)
{
    BlocoMemoria* blocoA = (BlocoMemoria*)a;
    BlocoMemoria* blocoB = (BlocoMemoria*)b;
    return (int)(blocoA->endereco_simulado - blocoB->endereco_simulado);
}

void* alocar_memoria(unsigned int inicio, unsigned int fim, size_t tamanho)
{
    if (total_blocos >= MAX_BLOCOS)
    {
        //printf("DEBUG: Limite de blocos atingido (%d)\n", MAX_BLOCOS);
        return NULL;
    }

    // Ordena os blocos antes de tentar alocar
    qsort(memoria, total_blocos, sizeof(BlocoMemoria), comparar_blocos);

    unsigned int pos = inicio;

    for (int i = 0; i <= total_blocos; i++)
    {
        unsigned int inicio_bloco = (i < total_blocos) ? memoria[i].endereco_simulado : fim + 1;

        // Espaço suficiente entre 'pos' e 'inicio_bloco'
        if ((inicio_bloco >= pos) && ((inicio_bloco - pos) >= tamanho))
        {
            void* bloco = malloc(tamanho);
            if (bloco != NULL)
            {
                memoria[total_blocos].endereco = bloco;
                memoria[total_blocos].tamanho = tamanho;
                memoria[total_blocos].usado = 1;
                memoria[total_blocos].endereco_simulado = pos;
                total_blocos++;

                //printf("DEBUG: Alocado %zu bytes em endereco simulado 0x%05X\n", tamanho, pos);
                return bloco;
            }
            else
            {
                //printf("DEBUG: Falha em malloc() para %zu bytes\n", tamanho);
                return NULL;
            }
        }

        // Avança para depois do bloco atual
        if (i < total_blocos)
        {
            pos = memoria[i].endereco_simulado + memoria[i].tamanho;
        }
    }

    printf("DEBUG: Nenhum espaco disponivel para %zu bytes entre 0x%05X e 0x%05X\n", tamanho, inicio, fim);
    printf("MEMORIA INSUFICIENTE PARA ALOCAR %zu BYTES!\n", tamanho);
    Sleep(6000);
    bsod("MEMORIA INSUFICIENTE: excedido o limite de 1MB!");
    return NULL;
}



void memmaker()
{
    printf("MEMMAKER - Otimizando memoria...\n");
    Sleep(1000);

    // 1. Listar blocos fora de ordem
    // 2. Compactar blocos da memoria convencional
    // 3. Simular carregamento de drivers na upper memory
    unsigned int proximo_end_conv = INICIO_CONV;

    for (int i = 0; i < total_blocos; i++)
    {
        if (memoria[i].usado && memoria[i].endereco_simulado < FIM_CONV)
        {
            memoria[i].endereco_simulado = proximo_end_conv;
            proximo_end_conv += memoria[i].tamanho;
        }
    }
    Sleep(1000);

    // Simula carregamento de drivers na upper memory
    unsigned int end_upper = INICIO_UPPER;
    for (int i = 0; i < 2; i++)
    {
        void* driver = alocar_memoria(INICIO_UPPER, FIM_UPPER, 8192); // 8 KB cada
        if(driver){
            printf("Driver carregado em upper memory (0x%05X)\n", memoria[total_blocos - 1].endereco_simulado);
            Sleep(1000);
            printf("Compactacao completa. Memoria otimizada!\n");
        }else{
            printf("Falha ao carregar driver %d na upper memory\n", i + 1);
            bsod("MEMORIA INSUFICIENTE PARA UPPER: excedido o limite de 1MB!");
}
    }


}

void exibir_mapa_memoria()
{
    printf("\nMAPA DA MEMORIA virtual (Convencional e Upper):\n");

    for (int i = 0; i < total_blocos; i++)
    {
        if (memoria[i].usado)
        {
            printf("Bloco %d | %05X-%05X | %llu bytes\n", i,
                   memoria[i].endereco_simulado,
                   memoria[i].endereco_simulado + (unsigned int)memoria[i].tamanho - 1,
                   (unsigned long long)memoria[i].tamanho);
        }
    }

}



void liberar_memoria()
{
    printf("DEBUG: Liberando memoria...\n");

    for (int i = 0; i < total_blocos; i++)
    {
        if (memoria[i].usado)
        {
            printf("DEBUG: Liberando bloco %d | Endereco simulado: 0x%05X | %zu bytes\n",
            i,memoria[i].endereco_simulado,memoria[i].tamanho);

            free(memoria[i].endereco);
            memoria[i].endereco = NULL;
            memoria[i].tamanho = 0;
            memoria[i].usado = 0;
            memoria[i].endereco_simulado = 0;
        }
    }

    memset(memoria, 0, sizeof(memoria));  // Zera tudo
    total_blocos = 0;

    printf("DEBUG: Memoria completamente liberada.\n");
}



size_t memoria_restante()
{
    size_t usado = 0;
    for (int i = 0; i < total_blocos; i++)
    {
        if (memoria[i].usado)
        {
            usado += memoria[i].tamanho;
        }
    }
    return RAM_TOTAL - usado;
}

void comando_mem()
{
    size_t total_usado = 0;

    printf("Blocos alocados:\n");
    for (int i = 0; i < total_blocos; i++)
    {
        if (memoria[i].usado)
        {
            printf(" - Endereco de memoria: %p | Tamanho: %llu bytes\n",
                   memoria[i].endereco,
                   (unsigned long long)memoria[i].tamanho);
            total_usado += memoria[i].tamanho;
        }
    }

    printf("\nTotal alocado: %llu bytes\n", (unsigned long long)total_usado);
    printf("RAM restante: %llu bytes\n", (unsigned long long)memoria_restante());
}

void salvar_memoria_em_arquivo()
{
    FILE* f = fopen("dump.mem", "wb");
    if (!f)
    {
        printf("Erro ao salvar memoria em arquivo.\n");
        return;
    }

    for (int i = 0; i < total_blocos; i++)
    {
        if (memoria[i].usado)
        {
            fwrite(memoria[i].endereco, 1, memoria[i].tamanho, f);
        }
    }

    fclose(f);
    printf("Estado da memoria salvo em dump.mem\n");
}

void bsod(const char* mensagem)
{
    unsigned long long total_usado = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    system("cls"); // limpa a tela

    printf("\n\n\n");
    Beep(600, 300); // tom grave
    Beep(400, 500); // tom mais grave ainda
    printf("  *** SISTEMA ENCONTROU UM ERRO FATAL ***\n\n");
    Beep(600, 300); // tom grave
    Beep(400, 500); // tom mais grave ainda
    printf("  %s\n\n", mensagem);
    Beep(600, 300); // tom grave
    Beep(400, 500); // tom mais grave ainda
    for (int i = 0; i < total_blocos; i++)
    {
        if (memoria[i].usado)
        {
            printf("%p |%llu bytes\n",
                   memoria[i].endereco,
                   (unsigned long long)memoria[i].tamanho);
            total_usado += memoria[i].tamanho;
        }
    }
    printf("  O sistema sera encerrado para evitar danos.\n");
    printf("  Pressione qualquer tecla para desligar...\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // reset cores

    system("pause > nul");
    exit(1); // encerra o sistema
}

/*int login_hash(const *char chave, int primo){
    int tam = strlen(chave);
    int resultado = 0;
    for (int i = 0; i< tam; i++){
            char c = tolower(chave[i]);
            int posicaoAlfa = C - 'a';
            resultado =  resultado + posicaoAlfa * 26* pow(26,i);

    }


}*/
//Só funciona no MS-DOS
/*void limpar_buffer() {
    __asm__ (
        "limpar:"
            "mov ah, 01h    ;"
            "int 21h"
            "jz done        ;"
            "mov ah, 08h    ;"
            "int 21h"
            "cmp al, 13     ;"
            "jne limpar     ;"
        "done:"
    );

}
*/







//Parte refrente as opções ajuda

