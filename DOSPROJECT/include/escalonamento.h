#ifndef ESCALONAMENTO_H
#define ESCALONAMENTO_H
#include <stdint.h>

typedef struct Context {
    void* esp;  // ponteiro de pilha (64 bits)
    void* ebp;
} Context;

void switch_context(Context* atual, Context* proxima);
void start_task(void (*func)(), void* pilha_topo);
void tarefa1();
void tarefa2();

#endif
