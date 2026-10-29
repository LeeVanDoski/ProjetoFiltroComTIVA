#include "LCD.h"

tContext grContext;
EstadoIHM estadoAtual = TELA_PRINCIPAL;

// Globais compartilhadas com o sistema de filtro
extern const uint16_t freqs[10];
extern volatile uint8_t filter;      // Filtro ATIVO
extern volatile uint8_t freq;        // Frequência ATIVA (índice)
extern volatile bool update_filter_needed;

// Globais de Input (ISRs)
extern volatile bool button_1;
extern volatile bool button_2;
extern volatile bool button_joystick;

// Navegação
uint8_t menu_temp_index = 0; 

// --- Funções Auxiliares ---

void LimparTela(void) {
    tRectangle rect = {0, 0, 127, 127};
    GrContextForegroundSet(&grContext, ClrBlack);
    GrRectFill(&grContext, &rect);
    GrContextForegroundSet(&grContext, ClrWhite);
}

void DisplaySetup(void) {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    
    GrContextInit(&grContext, &g_sCrystalfontz128x128);
    GrFlush(&grContext);
    GrContextFontSet(&grContext, &g_sFontFixed6x8);
    
    GrContextForegroundSet(&grContext, ClrWhite);
    GrContextBackgroundSet(&grContext, ClrBlack);
}

// --- Funções de Desenho ---

void DesenharTelaPrincipal(void) {
    char str[30];
    LimparTela();
    
    // Título
    GrStringDraw(&grContext, "SISTEMA DSP", -1, 20, 10, true);
    GrStringDraw(&grContext, "==============", -1, 10, 20, true);
    
    // Mostra o Filtro ATIVO (variável global 'filter')
    const char* nomeFiltro;
    switch(filter) {
        case FILTRO_LP: nomeFiltro = "Passa-Baixa"; break;
        case FILTRO_HP: nomeFiltro = "Passa-Alta"; break;
        case FILTRO_BP: nomeFiltro = "Passa-Faixa"; break;
        default: nomeFiltro = "???"; break;
    }
    
    sprintf(str, "Tipo: %s", nomeFiltro);
    GrStringDraw(&grContext, str, -1, 5, 45, true);
    
    sprintf(str, "Freq: %d Hz", freqs[freq]);
    GrStringDraw(&grContext, str, -1, 5, 60, true);
    
    // Rodapé
    GrStringDraw(&grContext, "S1: Configurar", -1, 5, 110, true);
    
    GrFlush(&grContext);
}

void DesenharTelaSelecaoFiltro(void) {
    char str[30];
    LimparTela();
    
    GrStringDraw(&grContext, "TIPO DE FILTRO", -1, 15, 10, true);
    GrStringDraw(&grContext, "=================", -1, 5, 20, true);
    
    int y = 40;
    for (int i = 0; i < 3; i++) {
        const char* nome;
        switch(i) {
            case FILTRO_LP: nome = "Passa-Baixa"; break;
            case FILTRO_HP: nome = "Passa-Alta"; break;
            case FILTRO_BP: nome = "Passa-Faixa"; break;
        }
        
        // Verifica se 'i' é o item que o usuário está navegando (menu_temp_index)
        if (i == menu_temp_index) {
            sprintf(str, "> %s", nome); // Seta
        } else {
            sprintf(str, "  %s", nome);
        }
        GrStringDraw(&grContext, str, -1, 5, y, true);
        y += 15;
    }
    
    GrStringDraw(&grContext, "S1: Mover", -1, 5, 100, true);
    GrStringDraw(&grContext, "Joy: OK   S2: Voltar", -1, 5, 110, true);
    
    GrFlush(&grContext);
}

void DesenharTelaParametrizacao(void) {
    char str[30];
    LimparTela();
    
    GrStringDraw(&grContext, "FREQUENCIA", -1, 30, 10, true);
    GrStringDraw(&grContext, "================", -1, 10, 20, true);
    
    uint16_t valDisplay = freqs[menu_temp_index];
    
    sprintf(str, "   <%d Hz>   ", valDisplay);
    GrStringDraw(&grContext, str, -1, 15, 55, true);
    
    GrStringDraw(&grContext, "S1: Ajustar", -1, 5, 90, true);
    GrStringDraw(&grContext, "Joy: Confirmar", -1, 5, 100, true);
    GrStringDraw(&grContext, "S2: Cancelar", -1, 5, 110, true);
    
    GrFlush(&grContext);
}

// --- Funções de Processamento (Lógica) ---

void ProcessarTelaPrincipal(void) {
    // S1: Entrar no Menu
    if (button_1) {
        button_1 = false;
        
        estadoAtual = SELECIONAR_FILTRO;
        // Inicializa o cursor na opção que já está ativa
        menu_temp_index = filter; 
        
        DesenharTelaSelecaoFiltro(); 
    }
    
    // Limpa eventos não usados
    if (button_2) button_2 = false;
    if (button_joystick) button_joystick = false;
}

void ProcessarSelecaoFiltro(void) {
    // S1: Navegar (Ciclo 0->1->2->0)
    if (button_1) {
        button_1 = false;
        menu_temp_index++;
        if (menu_temp_index > 2) menu_temp_index = 0;
        
        DesenharTelaSelecaoFiltro(); // Redesenha com novo cursor
    }

    // Joystick: Confirmar Tipo -> Ir para Frequência
    if (button_joystick) {
        button_joystick = false;

        filter = menu_temp_index; 
        
        estadoAtual = PARAMETRIZAR_FC1;
        menu_temp_index = freq; // Começa seleção de freq onde estava
        
        DesenharTelaParametrizacao();
    }
    
    // S2: Voltar
    if (button_2) {
        button_2 = false;
        estadoAtual = TELA_PRINCIPAL;
        DesenharTelaPrincipal();
    }
}

void ProcessarParametrizacaoFc1(void) {
    // S1: Navegar Frequências (0 a 9)
    if (button_1) {
        button_1 = false;
        menu_temp_index++;
        if (menu_temp_index > 9) menu_temp_index = 0;
        
        DesenharTelaParametrizacao();
    }

    // Joystick: Confirmar TUDO e Aplicar
    if (button_joystick) {
        button_joystick = false;
        
        // 1. Salva a frequência escolhida
        freq = menu_temp_index;
        
        update_filter_needed = true; 
        
        // 3. Volta para home
        estadoAtual = TELA_PRINCIPAL;
        DesenharTelaPrincipal();
    }
    
    // S2: Cancelar
    if (button_2) {
        button_2 = false;

        estadoAtual = TELA_PRINCIPAL;
        DesenharTelaPrincipal();
    }
}

// --- Loop da Interface ---
// Chame esta função periodicamente (ex: em uma thread de GUI dedicada de baixa prioridade)
void IHM_Loop(void) {
    switch(estadoAtual) {
        case TELA_PRINCIPAL:
            ProcessarTelaPrincipal();
            break;
        case SELECIONAR_FILTRO:
            ProcessarSelecaoFiltro();
            break;
        case PARAMETRIZAR_FC1:
            ProcessarParametrizacaoFc1();
            break;
    }
}