; -------------------------------------------------------
    ; ARQUIVO LIMPO: tx_initialize_low_level.S
    ; -------------------------------------------------------
    
    PRESERVE8
    THUMB

    ; Importações necessárias
    IMPORT  _tx_thread_system_stack_ptr
    IMPORT  _tx_initialize_unused_memory
    IMPORT  _tx_thread_context_save
    IMPORT  _tx_thread_context_restore
    IMPORT  _tx_timer_interrupt
    IMPORT  |Image$$ZI$$Limit|

    ; Área de Código
    AREA    |.text|, CODE, READONLY
    
; -------------------------------------------------------
; Função: _tx_initialize_low_level
; Descrição: Prepara a memória e ponteiros para o ThreadX
; -------------------------------------------------------
    EXPORT  _tx_initialize_low_level
_tx_initialize_low_level

    ; 1. Desabilitar interrupções (ThreadX habilita depois)
    CPSID   i

    ; 2. Configurar ponteiro de memória livre
    ; Usa o símbolo do linker do Keil para saber onde acaba a RAM usada
    LDR     r0, =_tx_initialize_unused_memory
    LDR     r1, =|Image$$ZI$$Limit|
    ADD     r1, r1, #4
    STR     r1, [r0]

    ; 3. Configurar Stack do Sistema (MSP)
    ; O startup_TM4C129.s já configurou o MSP inicial. 
    ; Apenas salvamos o valor atual para o ThreadX saber onde voltar.
    LDR     r0, =_tx_thread_system_stack_ptr
    MRS     r1, MSP                     ; Ler o Stack Pointer atual
    STR     r1, [r0]

    ; OBS: Não configuramos VTOR nem SysTick aqui.
    ; O VTOR é cuidado pelo startup padrão.
    ; O SysTick deve ser configurado no main.c via TivaWare.

    BX      lr

; -------------------------------------------------------
; Handlers do ThreadX
; -------------------------------------------------------

    EXPORT  __tx_BadHandler
__tx_BadHandler
    B       __tx_BadHandler

    EXPORT  __tx_SVCallHandler
__tx_SVCallHandler
    B       __tx_SVCallHandler

    EXPORT  __tx_IntHandler
__tx_IntHandler
    ; Este é um handler genérico, não costuma ser usado no Cortex-M 
    ; se você usa a tabela de vetores nativa.
    PUSH    {r0, lr}
    POP     {r0, lr}
    BX      LR

    EXPORT  __tx_SysTickHandler
__tx_SysTickHandler
    PUSH    {r0, lr}
    BL      _tx_timer_interrupt
    POP     {r0, lr}
    BX      LR

    ; Nota: _tx_thread_context_save vai pular para 
    ; _tx_thread_context_restore, que fará o retorno da interrupção.

    EXPORT  __tx_NMIHandler
__tx_NMIHandler
    B       __tx_NMIHandler

    EXPORT  __tx_DBGHandler
__tx_DBGHandler
    B       __tx_DBGHandler

    ALIGN
    END