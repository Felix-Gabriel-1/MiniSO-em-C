#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>  // Para kbhit() e getch()

///==========================
///Area de include
#include "include/verificainstall.h"
#include "include/geren_mem.h"
#include "include/funcgraficas.h"
#include "include/jogos.h"
#include "include/programas.h"
#include "include/system.h"
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
//Muda grafico talvez só funcine no MSDOS em modo 16 bits

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
            resetar_cor();
        }
        break;
        case 23:
            mudar_cor(7,1);
            memoria_cache_hash();
            resetar_cor();
            break;
        case 24:
            mudar_cor(7,1);
            listar_cache();
            resetar_cor();
            break;
        resetar_cor();
        break;
        default:
            //printf("Comando desconhecido, use ajuda caso nao saiba os comandos\n");
            mudar_cor(12, 0);  // Vermelho
            printf("Erro: comando nao reconhecido!\n");
            resetar_cor();
        }
    }


    return 0;
}

