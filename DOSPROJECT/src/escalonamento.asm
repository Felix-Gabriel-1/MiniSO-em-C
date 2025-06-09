; escalonamento.asm
BITS 64
global switch_context
global start_task

section .text

; void switch_context(Context* atual, Context* proxima)
; Parâmetros:
; RCX = atual
; RDX = proxima
switch_context:
    ; Salvar registradores não voláteis (se precisar)
    push rbx
    push rbp
    push rsi
    push rdi

    ; Salvar contexto atual
    mov [rcx], rsp      ; salva esp (stack pointer)
    mov [rcx + 8], rbp  ; salva rbp

    ; Restaurar contexto próximo
    mov rsp, [rdx]
    mov rbp, [rdx + 8]

    ; Restaurar registradores
    pop rdi
    pop rsi
    pop rbp
    pop rbx

    ret

; void start_task(void (*func)(), void* pilha_topo)
; Parâmetros:
; RCX = func
; RDX = pilha_topo
start_task:
    mov rsp, rdx          ; seta o topo da pilha
    and rsp, -16          ; garante alinhamento 16 bytes
    sub rsp, 8            ; reserva espaço para return address (alinhado)
    push 0                ; valor falso para retorno
    jmp rcx               ; vai para a função tarefa