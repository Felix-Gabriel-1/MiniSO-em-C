#ifndef GEREN_MEM_H_INCLUDED
#define GEREN_MEM_H_INCLUDED
void liberar_memoria();
void* alocar_memoria(unsigned int inicio, unsigned int fim, size_t tamanho);
void comando_mem();
void salvar_memoria_em_arquivo();
size_t memoria_restante();
void bsod(const char* mensagem);
void limpar_buffer();
void jogo_adivinha();
void memmaker();
void exibir_mapa_memoria();
void comparar_blocos(const void* a, const void* b);
//int login_hash(const *char chave, int primo);

#endif // GEREN_MEM_H_INCLUDED
