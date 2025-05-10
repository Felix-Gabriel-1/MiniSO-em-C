#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>  // Para WinAPI

#define MENU_OPCOES 3   // Define a quantidade de opções do menu
void abrir_novamente_executavel()
{
    char caminhoAtual[MAX_PATH];
    GetModuleFileName(NULL, caminhoAtual, MAX_PATH); // Caminho do Instalador.exe

    // Remove o nome do executável
    char *ultimoSeparador = strrchr(caminhoAtual, '\\');
    if (ultimoSeparador != NULL)
    {
        *ultimoSeparador = '\0'; // Agora estamos em ...\bin\Debug
    }

    // Subir 3 níveis até a pasta "SO"
    for (int i = 0; i < 3; i++) {
        char *sep = strrchr(caminhoAtual, '\\');
        if (sep != NULL) {
            *sep = '\0';
        }
    }

    // Agora adicionar caminho para SO.exe
    strcat(caminhoAtual, "\\SODOS\\bin\\Debug\\SO.exe");

    // Montar comando para abrir
    char comando[MAX_PATH * 2];
    sprintf(comando, "cmd /c start \"\" \"%s\"", caminhoAtual);

    system(comando);
    exit(0);
}



// Função para mudar a cor do texto no console
void set_color(int texto, int fundo)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (fundo << 4) | texto);
}


//ideia substituir printf fosorio porem ajustado
void tipo_terminal(const char *texto)
{
    while (*texto)
    {
        putchar(*texto++);
        fflush(stdout);
        Sleep(50);//ajustar velocidade
    }
    printf("\n");
}

void escreve_boot(){
    FILE *pont_arq;
    //char tipo_so[40];
    pont_arq = fopen("boot.txt","w");
    if(pont_arq == NULL){
        printf("ERRO FATAL NO BOOT");
        return;
    }
    #if defined(_WIN32) || defined(_WIN64) //não tenho a minima ideia disso mas acho que funciona
        fprintf(pont_arq, "1\n0\n0\n"); // Windows
        printf("Sistema detectado: Windows\n");

    #elif defined(__linux__)
        fprintf(pont_arq, "0\n1\n0\n"); // Linux
        printf("Sistema: Linux\n");

    #elif defined(__ANDROID__)
        fprintf(pont_arq, "0\n0\n1\n"); // Android
        printf("Sistema: Android\n");

    #else
        fprintf(pont_arq, "0\n0\n0\n"); // Desconhecido
        printf("Sistema: Desconhecido\n");
    #endif
    Sleep(9000);
    fclose(pont_arq);
    printf("\n instalado boot.txt .....\n");
}

void instala_arq()
{
    FILE *pont_arq;
    pont_arq = fopen("boot.txt","a");
    fclose(pont_arq);
    printf("\n instalando boot.txt .....\n");
    escreve_boot();
}


void mostra_arquitetura() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    tipo_terminal("Arquitetura da CPU: ");
    Sleep(300);

    switch (si.wProcessorArchitecture) {
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

void mostra_nome_cpu() {
    HKEY hKey;
    char cpuName[0x100];
    DWORD bufSize = sizeof(cpuName);

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        if (RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)cpuName, &bufSize) == ERROR_SUCCESS) {
            printf("Processador: %s\n", cpuName);
        } else {
            printf("Erro ao obter nome do processador\n");
        }

        RegCloseKey(hKey);
    } else {
        printf("Erro ao acessar o registro do Windows\n");
    }
}

void pega_gpu(){
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);
    printf("Placa de video:\n");
    if (EnumDisplayDevices(NULL, 0, &dd, 0)) {
        printf("Nome: %s\n", dd.DeviceString);
    } else {
        printf("Não foi possível obter informações da GPU.\n");
    }

}


/* antigo com kb */
/* void get_system_info() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    printf("Detectando CPU...\n");
    Sleep(1); //dar paradinha
    printf("Numero de nucleos: %u\n", sysInfo.dwNumberOfProcessors);
    Sleep(1);

    printf("\nDetectando memoria RAM...\n");

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    unsigned long long total_kb = memInfo.ullTotalPhys / 1024;
    unsigned long long extend_kb = total_kb - 1024; // simulando extended

    // Parte fixa estilo BIOS
    printf("640 KB OK\n");
    Sleep(1);
    printf("Upper Memory: 384 KB OK\n");
    Sleep(1);

    // Contador visual até extended memory
    printf("Extended Memory: ");
    fflush(stdout);

    for (unsigned long long i = 0; i <= extend_kb; i += 2048) { // sobe de 2MB em 2MB
        printf("\rExtended Memory: %llu KB", i);
        fflush(stdout);
        uSleep(30000); // 0.03s por "tick"
    }

    printf(" OK\n");

    // Total e disponível
    printf("Total: %llu MB\n", total_kb / 1024);
    printf("Disponivel: %llu MB\n", memInfo.ullAvailPhys / (1024 * 1024));
}
*/

void get_system_info() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    printf("Detectando CPU...\n");
    Sleep(1000);
    mostra_arquitetura();
    Sleep(1000);
    printf("Numero de nucleos: %u\n", sysInfo.dwNumberOfProcessors);
    Sleep(1000);
    mostra_nome_cpu();
    Sleep(1000);

    printf("\nDetectando memoria RAM...\n");

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    unsigned long long total_mb = memInfo.ullTotalPhys / (1024 * 1024);
    unsigned long long extended_mb = total_mb - 1; // simulando 1MB como convencional

    // Parte fixa estilo BIOS
    printf("640 KB OK\n");
    Sleep(1);
    printf("Upper Memory: 384 KB OK\n");
    Sleep(1);

    // Contador visual até extended memory em MB
    printf("Extended Memory: ");
    fflush(stdout);

    for (unsigned long long i = 0; i <= extended_mb; i += 50) { // contador contando os mb de memoria
        printf("\rExtended Memory: %llu MB", i);
        fflush(stdout);
        Sleep(300);
    }

    printf(" OK\n");

    // Total e disponível
    printf("Total: %llu MB\n", total_mb);
    printf("Disponivel: %llu MB\n", memInfo.ullAvailPhys / (1024 * 1024));
}


void menu_formatar()
{
    set_color(7, 1);  // texto branco (7), fundo azul (1)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");

    // Fundo azul, texto branco
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    printf("======================================================\n");
    printf("         INSTALACAO DO SISTEMA DA GAMBIARRA           \n");
    printf("======================================================\n\n");

    printf("E necessario formatar para prosseguir.\n");
    printf("Deseja continuar?\n\n");

    // Cor para opção destacada (cinza claro)
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("  [S] Sim\n");

    // Cinza sem intensidade (dá um aspecto de "desabilitado" ou menos destaque)
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("  [N] Nao\n");

    // Volta para padrão
    SetConsoleTextAttribute(hConsole, 7);
}

void boot_screen()
{
    char op[11];
    Beep(750, 300);

    system("cls"); // No Windows, use "clear" no Linux/macOS
    printf("Pegando informacoes do sistema\n");
    get_system_info();
    Beep(750, 300);
    Sleep(1);

    pega_gpu();
    Beep(750, 300);
    Sleep(2);

    /*printf("\n\nE necessario formatar para prosseguir.\n");
    printf("Deseja continuar?\n\n");
    printf("[S] Sim\n[N] Nao\n");
    */
    set_color(7, 1);  // texto branco (7), fundo azul (1)
    menu_formatar();

    //Tratamento de Stings
    while (getchar() != '\n' && !feof(stdin)); // limpa buffer mas tá bugado por que precisa colcar dois S S
    fgets(op,sizeof(op),stdin); //sintaxe o nome da variavel, o numero de casas, sinaliz de onde esta vindo stdin é do teclado
    op[strcspn(op, "\n")] = 0; //remover o barra n

    if(strcmp(op,"S") == 0 || strcmp(op,"s") == 0 ){ //não esqueca do 0, quando 0 o strcmp retora que são iguais
        printf("Carregando o instalador\n\n");
        printf("[          ] 0%%");
        fflush(stdout);
        Sleep(1);

        for (int i = 0; i <= 10; i++)
        {
            printf("\r["); //Chatgpt salvou aqui
            for (int j = 0; j < i; j++) printf("*"); // Preenchendo a barra
            for (int j = i; j < 10; j++) printf(" "); // Espaços vazios
            printf("] %d%%", i * 10);
            fflush(stdout);
            Sleep(500); // Meio segundo de espera (0.5s)
        }
        const char *instalacao[] = { //ideia stack over
            "\n Instalando SO.c...",
            "Copiando driver.sys...",
            "Configurando bios.bin...",
            "Finalizando setup...",
            "Limpando registros temporários...",
            "Sistema instalado com sucesso!"
        };
        instala_arq();

        for (int i = 0; i < 6; i++){
            printf("%s\n", instalacao[i]);
            Beep(700 + i * 50, 150);
            Sleep(700);
        }
         Sleep(1000);

        tipo_terminal("\n\n carregando o sistema!\n");
        Sleep(1);
        Beep(750, 300);
        Sleep(1);
        Beep(750, 300);
        abrir_novamente_executavel();
        exit(0);


        //system("cls"); // Apaga a tela antes de iniciar o programa principal
    }else{
        exit(0);
    }

}
// Função que exibe o menu, destacando a opção atualmente selecionada
void mostrar_menu(int selecionado)
{
    set_color(7, 1);  // texto branco (7), fundo azul (1)
    system("cls"); // Limpa a tela
    char *opcoes[MENU_OPCOES] =
    {
        "Iniciar sistema",
        "Sobre o sistema",
        "Sair"
    };

    printf("===== MENU PRINCIPAL =====\n\n");

    // Loop para mostrar cada opção do menu
    for (int i = 0; i < MENU_OPCOES; i++)
    {
        if (i == selecionado)
        {
            set_color(14,0); // Cor amarela para destacar
            printf("  %s\n", opcoes[i]); // Mostra a opção com destaque
            set_color(7,0);  // Restaura a cor para branco padrão
        }
        else
        {
            printf("   %s\n", opcoes[i]); // Mostra a opção normalmente
        }
    }
}

int main()
{
    set_color(15, 1); // Texto branco brilhante, fundo azul (clássico do DOS)
    int selecionado = 0; // Índice da opção selecionada (inicialmente a primeira)
    mostrar_menu(selecionado); // Exibe o menu pela primeira vez

    while (1)   // Loop infinito para capturar entrada do usuário
    {
        // Verifica se a seta para cima foi pressionada
        if (GetAsyncKeyState(VK_UP) & 0x8000)
        {
            selecionado = (selecionado - 1 + MENU_OPCOES) % MENU_OPCOES; // Move para cima no menu
            mostrar_menu(selecionado); // Atualiza o menu
            Sleep(200); // Pequeno atraso para evitar múltiplos registros da mesma tecla
        }

        // Verifica se a seta para baixo foi pressionada
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        {
            selecionado = (selecionado + 1) % MENU_OPCOES; // Move para baixo no menu
            mostrar_menu(selecionado); // Atualiza o menu
            Sleep(200);
        }

        // Verifica se a tecla Enter foi pressionada
        if (GetAsyncKeyState(VK_RETURN) & 0x8000)
        {
            system("cls"); // Limpa a tela
            switch (selecionado)
            {
            case 0:
                printf("Iniciando sistema...\n");
                boot_screen();// Mensagem ao escolher "Iniciar sistema"
                break;
            case 1:
                printf("Gambiarra System 1.0\nFeito em C puro!\n"); // Informações do sistema
                break;
            case 2:
                printf("Saindo...\n"); // Mensagem de saída
                Sleep(1000); // Espera 1 segundo
                exit(0); // Encerra o programa
            }
            Sleep(1000); // Espera 1 segundo antes de continuar
            system("pause"); // Pausa para o usuário ver a mensagem
            mostrar_menu(selecionado); // Exibe novamente o menu
        }

        Sleep(50); // Alívio de CPU, para não consumir muito processamento
    }

    return 0;
}
