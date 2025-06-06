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
///Aqui fica a area critica do programa
/* mostrar arquitetura e processador aqui o chat salvou simplimenste por que não sei mexer via c
codigos de registro do windows*/
void mostra_arquitetura()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    tipo_terminal("Arquitetura da CPU: ");

    switch (si.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_AMD64:
        tipo_terminal("x64 (AMD ou Intel 64 bits)\n");
        break;
    case PROCESSOR_ARCHITECTURE_INTEL:
        tipo_terminal("x86 (32 bits)\n");
        break;
    case PROCESSOR_ARCHITECTURE_ARM:
        tipo_terminal("ARM\n");
        break;
    case PROCESSOR_ARCHITECTURE_IA64:
        tipo_terminal("Intel Itanium (64 bits)\n");
        break;
    default:
        tipo_terminal("Desconhecida\n");
        break;
    }
}

void mostra_nome_cpu()
{
    HKEY hKey;
    char cpuName[0x100];
    DWORD bufSize = sizeof(cpuName);

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {

        if (RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)cpuName, &bufSize) == ERROR_SUCCESS)
        {
            printf("Processador: %s\n", cpuName);
        }
        else
        {
            printf("Erro ao obter nome do processador\n");
        }

        RegCloseKey(hKey);
    }
    else
    {
        printf("Erro ao acessar o registro do Windows\n");
    }
}

void get_system_info()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    printf("Detectando CPU...\n");
    Sleep(100);
    mostra_arquitetura();
    Sleep(100);
    printf("Numero de nucleos: %u\n", sysInfo.dwNumberOfProcessors);
    Sleep(100);
    mostra_nome_cpu();
    Sleep(100);

    printf("\nDetectando memoria RAM...\n");

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    unsigned long long total_mb = memInfo.ullTotalPhys / (1024 * 1024);
    unsigned long long extended_mb = total_mb - 1; // simulando 1MB como convencional

    // Parte fixa estilo BIOS
    /*printf("640 KB OK\n");
    Sleep(100);
    printf("Upper Memory: 384 KB OK\n");
    Sleep(100);
    */

    // Contador visual até extended memory em MB
    printf("Extended Memory: ");
    fflush(stdout);

    for (unsigned long long i = 0; i <= extended_mb; i += 1000)   // contador contando os mb de memoria
    {
        printf("\rExtended Memory: %llu MB", i);
        fflush(stdout);
        Sleep(300);
    }

    printf(" OK\n");

    // Total e disponível
    printf("Total: %llu MB\n", total_mb);
    printf("Disponivel: %llu MB\n", memInfo.ullAvailPhys / (1024 * 1024));
    printf("Memoria e limitada em 1 Mb \n");

}


void pega_gpu()
{
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);
    printf("Placa de video:\n");
    if (EnumDisplayDevices(NULL, 0, &dd, 0))
    {
        printf("Nome: %s\n", dd.DeviceString);
    }
    else
    {
        printf("Não foi possível obter informações da GPU.\n");
    }

}
void bios_screen() {
    mudar_cor(10, 0);
    //char tecla;
    system("cls");
    printf("Phoenix BIOS v6.00PG\n");
    Sleep(2000);
    printf("Copyright (C) 2025 China Technologies, Ltd.\n");
    Sleep(2000);
    printf("Main Processor:");
    Sleep(2000);
    mostra_nome_cpu();
    printf("Memory Testing : OK\n");
    Sleep(2000);
   // printf("Press DEL to enter SETUP\n");
    //tecla = getchar();
    //Sleep(2000);

    Sleep(2000);  // Espera 2 segundos

    // Simular detecção rápida
    Sleep(2000);
    printf("\nDetecting IDE Primary Master ... ");
    Sleep(500);
    printf("ST500DM002\n");
    Sleep(500);
    Sleep(2000);
    printf("Verificando instalacao...");
    Sleep(2000);
    verificainstall();
    Sleep(2000);

    printf("Detecting IDE Primary Slave  ... ");
    Sleep(500);
    printf("None\n");

    printf("Detecting IDE Secondary Master ... ");
    Sleep(500);
    printf("HL-DT-ST DVDRAM\n");

    printf("Detecting IDE Secondary Slave  ... ");
    Sleep(500);
    printf("None\n");

    Sleep(1500);  // Pausa final
    resetar_cor();
    limpa_tela();
}


void boot_screen()
{
    bios_screen();
    Beep(750, 300);
    system("cls");
    printf("Pegando informacoes do sistema\n");
    get_system_info();
    Beep(750, 300);
    Sleep(100);;
    pega_gpu();
    Beep(750, 300);
    Sleep(200);
    printf("Carregando o sistema...\n\n");

    printf("[          ] 0%%");
    fflush(stdout);
    Sleep(1000);

    for (int i = 0; i <= 10; i++)
    {
        printf("\r[");
        for (int j = 0; j < i; j++) printf("|"); // Preenchendo a barra
        for (int j = i; j < 10; j++) printf(" ");
        printf("] %d%%", i * 10);
        fflush(stdout);
        Sleep(500); // Meio segundo de espera (0.5s)
    }

    printf("\n\nSistema carregado com sucesso!\n");
    Sleep(100);
    tipo_terminal("  _____   ____  \n");
    tipo_terminal(" / ____| / __ \\ \n");
    tipo_terminal("| (___  | |  | |\n");
    tipo_terminal(" \\___ \\ | |  | |\n");
    tipo_terminal(" ____) || |__| |\n");
    tipo_terminal("|_____/  \\____/ \n");
    tipo_terminal("S  Y  S  T  E  M\n\n");
    //tipo_terminal("\n\n Gambiarra System!\n");
    Sleep(100);
    system("cls"); // Apaga a tela antes de iniciar o programa principal
    printf("gambiarra system 1.022\n");
}

void encerrar_programa()
{
    exit(0);
}
void verificainstall() {
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


    printf("abrindo: %s\n", finalPath);  // DEBUG: Mostra o caminho usado

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

    }else{
        if(a == 0 && b == 0 && c==0){
        printf("Verificao de boot concluida...\n");
        printf("Linux nao esta disponivel");
        Sleep(1000);
        printf("encerrando...");
        } else {
        printf("Condicao nao atendida. Encerrando.\n");
        exit(1);
        return;
    }
    }
}


