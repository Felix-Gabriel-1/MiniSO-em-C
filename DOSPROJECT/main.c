#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>  // Para kbhit() e getch()
#include "include/funcoes_diretorio.h"
#include "include/geren_mem.h"
#include "include/jogos.h"
#define MAX_PATH 256

#define RAM_TOTAL 1024 * 1024  // 1MB

/*Area de memoria do programma
*/
#define INICIO_CONV   0x00000
#define FIM_CONV      0x9FFFF

#define INICIO_UPPER  0xA0000
#define FIM_UPPER     0xBFFFF

#define INICIO_ROM    0xC0000
#define FIM_ROM       0xFFFFF


// Função para mudar a cor do texto no console
void mudar_cor(int texto, int fundo)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (fundo << 4) | texto);
}
void resetar_cor()
{
    mudar_cor(7, 0);
}

void versao ()
{
    printf("\n Versao 1.021 \n ");
    printf("\n por Gabriel Felix \n ");
}
//Abrir arquivo usando o buffer
void xopen(const char *filename)
{
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

//Muda grafico talvez só funcine no MSDOS em modo 16 bits
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
void encerrar_programa()
{
    exit(0);
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
        "movl %[a], %%eax;"         // eax = a
        "movl %[b], %%ebx;"         // ebx = b
        "movb %[op], %%cl;"         // cl = operador

        "cmpb $'+', %%cl;"          // Verifica se operador é '+'
        "je soma;"                  // Se sim, pular para soma
        "cmpb $'-', %%cl;"          // Verifica se operador é '-'
        "je sub;"                   // Se sim, pular para subtração
        "cmpb $'*', %%cl;"          // Verifica se operador é '*'
        "je mult;"                  // Se sim, pular para multiplicação
        "cmpb $'/', %%cl;"          // Verifica se operador é '/'
        "je div;"                   // Se sim, pular para divisão
        "jmp fim;"                  // Caso não seja nenhum dos acima, ir para fim

        "soma:"                     // soma: eax = eax + ebx
        "add %%ebx, %%eax;"         //Chama funcção soma para os registradores ebx e eax
        "jmp fim;"                  // Pular para fim

        "sub:"                      // subtração: eax = eax - ebx
        "subl %%ebx, %%eax;"
        "jmp fim;"                  // Pular para fim

        "mult:"                     // multiplicação: eax = eax * ebx
        "imull %%ebx, %%eax;"
        "jmp fim;"                  // Pular para fim

        "div:"                      // divisão: eax = eax / ebx
        "cdq;"                      // Extensão de sinal (para divisão de números negativos)
        "idivl %%ebx;"              // idivl para dividir eax por ebx
        "jmp fim;"                  // Pular para fim

        "fim:"                      // Aqui o valor de eax é movido para resultado1
        "movl %%eax, %[res];"       // Salva o resultado de eax em resultado1

        : [res] "=r" (resultado1)   // Saída: resultado final
        : [a] "r" (a), [b] "r" (b), [op] "r" (op)  // Entradas: a, b, op
        : "%eax", "%ebx", "%ecx", "%edx"  // Registradores usados
    );

    return resultado1;
}

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
void boot_screen()
{
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
        for (int j = 0; j < i; j++) printf("="); // Preenchendo a barra
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
    tipo_terminal("\n\n Gambiarra System!\n");
    Sleep(100);
    system("cls"); // Apaga a tela antes de iniciar o programa principal
    printf("gambiarra system 1.022\n");
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
    //if (strcmp(comando, "hash") == 0) return 13;
    return 0; // Comando desconhecido
}
//depois remover esse monte de if
int main()
{
    boot_screen();
    char command[30], filename[20];
    char cwd[MAX_PATH];
    while(1)
    {
        if(getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s>", cwd);
        }
        else
        {
            printf("ERRO EM EXIBIR O DIRETORIO");
            encerrar_programa();
        }
        scanf("%29s", command);
        registrar_log(command);
        int cmd = identificar_comando(command);
        //Sai cadeia de if-else entra switch case

        switch (cmd)
        {
        case 1: // xopen
            scanf("%19s", filename);
            xopen(filename);
            break;
        case 2: // xgfx
            //change_graphics();
            break;
        case 3: // diretorio
            mostar_diretorio();
            break;
        case 4: // sair
            encerrar_programa();
            break;
        case 5: // voltar
            voltar_diretorio();
            break;
        case 6: // calc
            //calculadora();
            break;
        case 7: // ajuda
            ajuda();
            break;
        case 8:   // calcasm
        {
            int a = 0,b = 0,r = 0;
            char op;
            printf("numero1:");
            scanf("%d",&a);
            printf("numero2:");
            scanf("%d",&b);
            printf("op:");
            scanf(" %c", &op);  // Espaço antes do %c ignora whitespace



            r = calcasm(a,b,op);
            printf("Resultado: %d\n",r);
            break;
        }
        case 9: // limpar
            limpa_tela();
            break;
        case 10:
            versao();
            break;
        case 11: // md = mudar de diretório
        {
            char comando[256];
            printf("md ");
            fflush(stdin); // limpa o buffer
            fgets(comando, sizeof(comando), stdin);
            comando[strcspn(comando, "\n")] = '\0';
            while (comando[0] == ' ')
            {
                memmove(comando, comando + 1, strlen(comando));
            }
            entrar_diretorio(comando);  // aqui usa chdir()
            break;
        }
        case 12:
            listar_diretorio();
            break;
        case 13:
            relogio();
            break;
        case 14:
            comando_mem();
            break;
        case 15:
            liberar_memoria();
            break;
        case 16:
        {
            char entrada[64];
            printf("Tamanho a alocar (em bytes): ");
            scanf("%63s", entrada);
            size_t tamanho = atoi(entrada);


            void* bloco = alocar_memoria(0x00000, 0x9FFFF, tamanho);  // Memória convencional

            if (bloco)
            {
                printf("Memoria alocada em %p (%zu bytes)\n", bloco, tamanho);
            }
            else
            {
                printf("Erro: nao foi possivel alocar memoria\n");
            }
            break;
        }
        case 17:
            salvar_memoria_em_arquivo();
            break;
        case 18:
            mudar_cor(7, 1);  // texto branco (7), fundo azul (1)
            jogo_adivinha();
            resetar_cor();
            break;
        case 19:
            system("start notpad");
            break;
        case 20: //remmaker
        {
            mudar_cor(7, 1);  // texto branco (7), fundo azul (1)
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            memmaker();
            resetar_cor();
        }
        break;
        case 21:
            mudar_cor(7, 1);
            exibir_mapa_memoria();
            resetar_cor();
            break;
        case 22: //jogo cobra
        {
            mudar_cor(7, 1);
            printf("-------------------------------------------------------------------------\n");
            som_menu();
            printf("                        JOGO DA COBRA \n");
            som_menu();
            printf("-------------------------------------------------------------------------\n");
            som_menu();
            printf("link de quem ensinou https://www.youtube.com/watch?v=jTzQ9s3-ThU \n");
            som_menu();
            printf("                                                                 ver 0.03\n");
            som_menu();
            printf("-------------------------------------------------------------------------\n");
            Sleep(300);
            printf("Carregando...");
            Sleep(2000);
            system("cls");
            //Para resetar o jogo caso inciasse denovo dava bug
            resetar_jogo();
            int i, gameover=0;
            int tecla;

            for(i=0; i<50; i++)
            {
                gotoxy(i,0);
                printf("%c",219);
                Sleep(5);
            }
            for(i=0; i<20; i++)
            {
                gotoxy(50,i);
                printf("%c",219);
                Sleep(5);
            }
            for(i=50; i>=0; i--)
            {
                gotoxy(i,20);
                printf("%c",219);
                Sleep(05);
            }
            for(i=20; i>0; i--)
            {
                gotoxy(0,i);
                printf("%c",219);
                Sleep(5);
            }
            geraComida();
            desenha();
            tecla='d';
            while(gameover==0)
            {
                gotoxy(52,2);
                printf("Pontos: %d\t",pontos);
                gotoxy(52,4);
                printf("Velocidade: %.2f caracteres/s",1000/velo);
                c[0][0]=cx;
                c[0][1]=cy;
                if(kbhit()) // Se alguma tecla for apertada, o valor
                    tecla=getch(); // vai para a variável 'tecla'

                if(tecla=='w' || tecla=='W' || tecla==72)
                {
                    cy--;
                    if(cy==0) { som_game_over_8bit(); break; } // Se a cabeça da cobra estiver na parede superior,
                }                    // O jogo acaba
                if(tecla=='a' || tecla=='A' || tecla==75)
                {
                    cx--;
                    if(cx==0) { som_game_over_8bit(); break; } // Se a cabeça da cobra estiver na parede da esquerda,
                }                    // O Jogo acaba
                if(tecla=='s' || tecla=='S' || tecla==80)
                {
                    cy++;
                    if(cy==20) { som_game_over_8bit(); break; } // Se a cabeça da cobra estiver na parede de baixo,
                }                     // O jogo acaba
                if(tecla=='d' || tecla=='D' || tecla==77)
                {
                    cx++;
                    if(cx>=50) { som_game_over_8bit(); break; } // Se a a cabeça da cobra estiver na parede da direida,
                }                     // O jogo acaba

                if(cx==comida[0] && cy==comida[1])  // Se a cobra comer a comida
                {
                    pontos++;
                    if(velo>50) velo-=10; // Velocidade em milissegundos abaixa
                    geraComida();
                    som_acerto_j();
                }
                gameover=analiza();
                atualiza(); // Atualiza a cobra
                desenha(); // Desenha a cobra
                gotoxy(50,20);
                Sleep(velo);
            }
            system("cls"); // Quando o usuário perder, limpa a tela e exibe uma mensagem final
            printf("Voce perdeu! Fez %d pontos.\n",pontos);
            som_game_over_8bit();
            system("pause");
        }
        resetar_cor();
        break;
        default:
            printf("Comando desconhecido, use ajuda caso nao saiba os comandos\n");
        }
    }


    return 0;
}

