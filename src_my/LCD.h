#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "Crystalfontz128x128_ST7735.h"
#include "grlib/grlib.h"

// --- Definições e Globais ---

typedef enum {
    TELA_PRINCIPAL,
    SELECIONAR_FILTRO,
    PARAMETRIZAR_FC1,
} EstadoIHM;

// Mapeamento para facilitar leitura (deve bater com a ordem da matriz FIR)
typedef enum {
    FILTRO_LP = 0,
    FILTRO_HP = 1,
    FILTRO_BP = 2
} FilterType_t;



void DesenharTelaParametrizacao(void);
void DesenharTelaSelecaoFiltro(void);
void DesenharTelaPrincipal(void);
void LimparTela(void);
void DisplaySetup(void);
void ProcessarTelaPrincipal(void);
void ProcessarSelecaoFiltro(void);
void ProcessarParametrizacaoFc1(void);
void IHM_Loop(void);