#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "common/inc/tx_api.h"
#include "arm_math.h"

#define NVIC_SYS_PRI3_R  (*((volatile uint32_t *)0xE000ED20))
#define NVIC_SYS_PRI3_PENDSV_MASK  0x00FF0000 // Bits 23:16
#define NVIC_SYS_PRI3_SYSTICK_MASK 0xFF000000 // Bits 31:24

#define NUM_AMOSTRAS 512
#define FIR_ORDER 64
#define NUM_TAPS (FIR_ORDER+1)
#define BLOCK_SIZE 64
#define NUM_FILTERS 3
#define NUM_FREQ 10
#define JOYSTICK_SELECT_PORT    GPIO_PORTE_BASE
#define JOYSTICK_SELECT_PIN     GPIO_PIN_4
#define BUTTON_S1_PORT          GPIO_PORTM_BASE
#define BUTTON_S1_PIN           GPIO_PIN_3
#define BUTTON_S2_PORT          GPIO_PORTL_BASE
#define BUTTON_S2_PIN           GPIO_PIN_6
#define DEBOUNCE 200 //200 ms de debounce

// --- Variáveis Globais (Compartilhadas) ---

// Variáveis de Estado
const uint16_t freqs[10] = {250, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500};
volatile uint8_t filter = 0; // 0=LP, 1=HP, 2=BP
volatile uint8_t freq = 0;   // Índice 0 a 9
volatile bool update_filter_needed = true; 

// Inputs (vindos das interrupções)
volatile bool button_1=false;
volatile bool button_2=false;
volatile bool button_joystick=false;

volatile uint8_t modo_filter=0;




//inicialização
volatile uint32_t SystemClock;
void init_hw();