#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/uart.h"

#include "hardware/timer.h"
#include "pico/bootrom.h" //
#include "hardware/pwm.h"
#include "pico/time.h"

#include "include/libraries/ssd1306.h"
#include "include/libraries/numbers_n_array.h"
#include "include/libraries/font.h"
#include "include/libraries/func_generals.h"

#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12
#define BOTAO_A 5
#define BOTAO_B 6
#define BOTAO_C 22
#define OUT_PIN 7 //gpio pio 5x5
#define BUZZER_A_PIN 10
#define BUZZER_B_PIN 21
#define NUM_PIXELS 25
#define DEBOUNCING_TIME_US 220
#define NUMLEDS 3


#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// variaveis
volatile _Atomic uint estado; // 0 - vermelho; 1- amarelho; 2 - verde
static uint8_t leds[3] = {13, 11, 12};
volatile _Atomic uint8_t outroContador = 0;
volatile uint32_t hora_presente;
static volatile uint32_t ultimo_pressionamento = 0;
struct repeating_timer timer;
volatile _Atomic uint saida_teste = 0;
volatile bool isRunning = false;
//


volatile _Atomic uint numero_display=0;
volatile _Atomic uint cor_atual =1;//apenas 1, 2 e 3 
volatile _Atomic uint brilho =1; // 1 a 4
volatile _Atomic uint tempo =5; // 1 a 4
volatile uint32_t hora_presente;
volatile _Atomic uint var_teste=0;
volatile _Atomic uint saida_teste=0;
volatile _Atomic uint sobe_um=0;//botao a
volatile _Atomic uint desce_um=0; //botao b
int randon=0;
static bool randonTwo=0;
//
volatile _Atomic char leitura_UART;
absolute_time_t debounce_do_serial;

//objetivo nao muito claro
static volatile uint32_t ultimo_pressionamento = 0;
static uint8_t leds[3] = {13, 11, 12};
PIO pio_controlador = pio0;
bool status_init_pio = false;
uint state_machine =0;
bool isSerial=true;
ssd1306_t ssd;
bool cor =true;


// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments


//trata as interrupções
void tratar_botoes(uint gpio, uint32_t events)
{
    //debounce();
    
      hora_presente = to_us_since_boot(get_absolute_time());
    if(gpio==BOTAO_A)
    {
        if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US*1000)
        {
            bool botao_pressionado=!gpio_get(BOTAO_A);
            if(botao_pressionado){
                    if(sobe_um==0){sobe_um=1;return;}
                    if(sobe_um==1){sobe_um=0;return;}
            }
        }
    
    //

    } else if( gpio ==BOTAO_B)
    {
         if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US*1000)
        {
            bool botao_pressionado=!gpio_get(BOTAO_B);
            if(botao_pressionado){
                    if(desce_um==0){desce_um=1;return;}
                    if(desce_um==1){desce_um=0;return;}
            }
        }
     


    //
    
    } else if(gpio==BOTAO_C)
    {
        if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US*1000)
        {
            bool botao_pressionado=!gpio_get(BOTAO_C);
            if(botao_pressionado)
            {saida_teste=1;}
        }

    }
            ultimo_pressionamento=hora_presente;

}
void inicia_hardware()
{
    //inicializa pio
    init_pio_routine();
    //inicializa leds 11 
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    //inicializa botoes
        //c
    gpio_init(BOTAO_C);
    gpio_set_dir(BOTAO_C, GPIO_IN);
    gpio_pull_up(BOTAO_C);
    gpio_set_irq_enabled_with_callback(BOTAO_C, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
    //a
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
    //b
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);

    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, 0, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    //iniciar repeating timer para por em tela caso o seria n seja acionado
    add_repeating_timer_ms(1000, verificador_serial, false, &timer);

}
bool verificador_serial()//roda a cada 1000 ms
{
    if(stdio_usb_connected()==false)
    {
        if(time_reached(debounce_do_serial))
        {
            mensagem_serial(&ssd);
            isSerial=false;
        }
    }else if(isSerial==false)
    {
        isSerial=true;
        mensagem_caracter(&ssd);
    }
}


void init_pio_routine()
{
    //inicia o PIO
    //sem struct
    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
   status_init_pio = set_sys_clock_khz(128000, false);

    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (status_init_pio){ printf("clock set to %ld\n", clock_get_hz(clk_sys));}

    //configurações da PIO
    uint deslocamento = pio_add_program(pio_controlador, &pio_matrix_program);
    state_machine= pio_claim_unused_sm(pio_controlador, true);
    pio_matrix_program_init(pio_controlador, state_machine, deslocamento, OUT_PIN);

}

int main()
{
    inicia_hardware();
    debounce_do_serial=delayed_by_ms(get_absolute_time(), 2000);
 

    while (true) 
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
        if(stdio_usb_connected())
        {
            //ler do terminal
            printf("\n\nleitura do terminal\n\n");
            if(scanf("%c",leitura_UART)==1)//identifica se foi ligo algo
                {
                    printf("\n\nlido: '%c'\n", leitura_UART);
                    if(leitura_UART<=48&&leitura_UART>=57)
                    {
                        numero_display=leitura_UART;
                        atualiza_matrix();
                    }else if((leitura_UART>=65&&leitura_UART<=90)||(leitura_UART>=97&&leitura_UART<=122))
                    {
                        mensagem_caracter(&ssd);
                    }
                }
        }sleep_ms(40);

        //sobe_um botao a
        if(sobe_um==1)
        {
            liga_verde(1);
        }else if(sobe_um==0)
        {
            liga_verde(0);
        }
        //desce_um botao b
        if(desce_um==1)
        {
            liga_azul(1);
        }else if(desce_um==0)
        {
            liga_azul(0);
        }
        //saida_teste bootloader
        if(saida_teste==1)
        {
            entrarModoBootloader();
        }

    }
}
