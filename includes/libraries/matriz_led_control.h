#ifndef MATRIZ_LED_CONTROL
#define MATRIZ_LED_CONTROL

#include <stdio.h>
#include <stdint.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25
#define FRAMES_FLAGS 10
#define MAX_LEDS 25

typedef struct {
    PIO pio;
    bool ok;
    uint16_t i;
    double r;
    double g;
    double b;
    uint sm;
} pio_t;

void init_pio_routine(pio_t * meu_pio);
void imprimir_binario(int num) ;
uint32_t matrix_rgb(double b, double r, double g);
void desenho_pio(double *desenho, pio_t * meu_pio);
void desenho_pio_rgb(double *desenho, pio_t * meu_pio);
void desenho_pio_vermelho(double *desenho, pio_t * meu_pio);
void desenho_pio_azul_100(pio_t * meu_pio);
void desenho_pio_vermelho_80(pio_t * meu_pio);
void desenho_pio_verde_50(pio_t * meu_piom);
void desliga_tudo(pio_t * meu_pio);
void ligar_todos_os_leds_20_p(pio_t * meu_pio);
void entrarModoBootloader();
void desenho_pioFlag(double flagB[FRAMES_FLAGS][MAX_LEDS],double flagG[FRAMES_FLAGS][MAX_LEDS],double flagR[FRAMES_FLAGS][MAX_LEDS], pio_t * meu_pio);
void star_spangled_gleison(pio_t * meu_pio);


#endif  // MATRIZ_LED_CONTROL