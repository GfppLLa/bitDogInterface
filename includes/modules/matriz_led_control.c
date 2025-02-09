#include "matriz_led_control.h"
#include "pride_of_flag.h"
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/structs/systick.h"  // Necessário para a função de reset no SDK do Pico
#include "pico/bootrom.h"  //
#include <stdio.h>
// #include "buzzer_functions.h"
#define NUM_PIXELS 25



//imprimir valor binário
void imprimir_binario(int num) 
{
 int i;
 for (i = 31; i >= 0; i--) {
  (num & (1 << i)) ? printf("1") : printf("0");
 }
}



void desenho_pio(double *desenho, pio_t * meu_pio)
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(desenho[24-i], desenho[24-i], desenho[24-i]);
        pio_sm_put_blocking(pio0, 0, valor_led);
        //imprimir_binario(valor_led);
    }
    printf("clock set to %ld\n", clock_get_hz(clk_sys));
}

void desenho_pio_rgb(double *desenho, pio_t * meu_pio)
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(desenho[24-i]*meu_pio->b, desenho[24-i]*meu_pio->r, desenho[24-i]*meu_pio->g);
        pio_sm_put_blocking(meu_pio->pio, meu_pio->sm, valor_led);
        //imprimir_binario(valor_led);
    }
    printf("clock set to %ld\n", clock_get_hz(clk_sys));
}

void desenho_pio_vermelho(double *desenho, pio_t * meu_pio) 
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(0.0, desenho[24 - i], 0.0); // Corrigido para usar o canal vermelho corretamente
        pio_sm_put_blocking(pio0, 0, valor_led);
    }
    printf("Frame desenhado na matriz de LEDs com cor vermelha.\n");
}

void desenho_pio_azul_100(pio_t * meu_pio)
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(1.0, 0.0, 0.0);
        pio_sm_put_blocking(pio0, 0, valor_led);
        //imprimir_binario(valor_led);
    }
    printf("clock set to %ld\n", clock_get_hz(clk_sys));
}

void desenho_pio_vermelho_80(pio_t * meu_pio)
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(0.0, 0.8, 0.0);
        pio_sm_put_blocking(pio0, 0, valor_led);
        //imprimir_binario(valor_led);
    }
    printf("clock set to %ld\n", clock_get_hz(clk_sys));
}

void desenho_pio_verde_50(pio_t * meu_pio)
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(0.0, 0.0, 0.5);
        pio_sm_put_blocking(pio0, 0, valor_led);
        //imprimir_binario(valor_led);
    }
    printf("clock set to %ld\n", clock_get_hz(clk_sys));
}

void ligar_todos_os_leds_20_p(pio_t * meu_pio)
{
    uint32_t valor_led = 0;
    double ligar_todos_os_leds_20[25] =   {0.2, 0.2, 0.2, 0.2, 0.2,
                                        0.2, 0.2, 0.2, 0.2, 0.2, 
                                        0.2, 0.2, 0.2, 0.2, 0.2,
                                        0.2, 0.2, 0.2, 0.2, 0.2,
                                        0.2, 0.2, 0.2, 0.2, 0.2};

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(ligar_todos_os_leds_20[24-i], ligar_todos_os_leds_20[24-i], ligar_todos_os_leds_20[24-i]);
        pio_sm_put_blocking(pio0, 0, valor_led);
        //imprimir_binario(valor_led);
    }
    printf("clock set to %ld\n", clock_get_hz(clk_sys));
}

void desliga_tudo(pio_t * meu_pio)
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(0.0, 0.0, 0.0);
        pio_sm_put_blocking(pio0, 0, valor_led);
        //imprimir_binario(valor_led);
    }
    printf("clock set to %ld\n", clock_get_hz(clk_sys));
}

void entrarModoBootloader() {

    tocar_tom_buzzer(1000, 200); 
    sleep_ms(100);              
    tocar_tom_buzzer(1500, 200); 
    sleep_ms(100);               
    tocar_tom_buzzer(2000, 300);
    sleep_ms(100);               
    tocar_tom_buzzer(1000, 200);
    sleep_ms(50);                
    tocar_tom_buzzer(1500, 200); 
    sleep_ms(50);                
    tocar_tom_buzzer(2000, 400);
    reset_usb_boot(0, 0); // Reinicia no modo bootloader
}
void tocarecantar() {

 tocar_tom_buzzer(392, 500); // Sol
 sleep_ms(100);
  tocar_tom_buzzer(330, 500); // Mi
  sleep_ms(100);
  tocar_tom_buzzer(261, 500); // Do
  sleep_ms(100);
  tocar_tom_buzzer(330, 500); // Mi
  sleep_ms(100);
  tocar_tom_buzzer(392, 500); // Sol
  sleep_ms(100);
  tocar_tom_buzzer(523, 500); // ^Do
  sleep_ms(100);

tocar_tom_buzzer(659, 500); // ^Mi
  sleep_ms(100);

  tocar_tom_buzzer(587, 500); // ^Re
  sleep_ms(100);

  tocar_tom_buzzer(523, 500); // ^Do
  sleep_ms(100);

  tocar_tom_buzzer(330, 250); // Mi
  sleep_ms(100);

  tocar_tom_buzzer(370, 250); // Fa#
  sleep_ms(100);

  tocar_tom_buzzer(392, 500); // Sol
}
//desenho pio
uint32_t matrix_rgbFlag(double b, double r, double g)
{
    r=fmax(0.0, fmin(1.0, r));
    g=fmax(0.0, fmin(1.0, g));
    b=fmax(0.0, fmin(1.0, b));
  unsigned char R, G, B;
  R = (unsigned char)(r * 255.0);
  G = (unsigned char)(g * 255.0);
  B = (unsigned char)(b * 255.0);
  return  (G << 24)| (R << 16) | (B << 8)|0xFF;
}
void desenho_pioFlag(double (*flagB)[25],double (*flagG)[25],double (*flagR)[25], pio_t *meu_pio)
{
    uint32_t valor_led = 0;
    int frames=10;
    tocar_tom_buzzer(1000, 200); 
    sleep_ms(100);              

    for(int16_t j=0; j<frames; j++)
    {//frames ou linhas
        for (int16_t i = 0; i <= NUM_PIXELS; i++) //laço pixels ou colnas
        {

            valor_led = matrix_rgbFlag(flagB[j][24-i], flagR[j][24-i], flagG[j][24-i]);
          
            pio_sm_put_blocking(pio0, 0, valor_led);
            sleep_us(50);
   
 }
        sleep_ms(500);
    }
}

void star_spangled_gleison(pio_t *meu_pio)
{
tocarecantar();
//desenho_pioFlag(flagB, flagG, flagR, &meu_pio);
}


