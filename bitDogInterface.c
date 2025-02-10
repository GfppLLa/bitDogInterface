// Bibliotecas de inclusão
#include <stdint.h>
#include "hardware/clocks.h" // Inclua o cabeçalho correto
#include <stdio.h>                // Para operações de entrada/saída
#include <stdint.h>
#include<stdbool.h>
#include <math.h>
#include "pico/stdlib.h"           // Biblioteca padrão da Raspberry Pi Pico
#include "hardware/i2c.h"          // Biblioteca para comunicação I2C
#include "pico/stdio_usb.h"
#include "hardware/uart.h"         // Biblioteca para comunicação UART
#include "hardware/timer.h"        // Biblioteca para uso de timers
#include "pico/bootrom.h"          // Biblioteca para manipulação do bootrom (bootloader)
#include "hardware/pwm.h"          // Biblioteca para controle de PWM
#include "pico/time.h"             // Biblioteca para manipulação de tempo
#include "pio_matrix.pio.h"
#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"

#include "include/libraries/ssd1306.h"      // Biblioteca do display SSD1306
//
#include "include/libraries/numbers_n_array.h"  // Biblioteca auxiliar de números e arrays
#include "include/libraries/font.h"        // Biblioteca de fontes
#include "include/libraries/func_generals.h"  // Funções gerais auxiliares

// Definições de macros
#define LED_RED 13            // Definição do pino do LED vermelho
#define LED_GREEN 11          // Definição do pino do LED verde
#define LED_BLUE 12           // Definição do pino do LED azul
#define BOTAO_A 5             // Definição do pino do botão A
#define BOTAO_B 6             // Definição do pino do botão B
#define BOTAO_C 22            // Definição do pino do botão C
#define OUT_PIN 7             // Pino para o controle do PIO (matriz 5x5)
#define BUZZER_A_PIN 10       // Pino para o buzzer A
#define BUZZER_B_PIN 21       // Pino para o buzzer B
#define NUM_PIXELS 25         // Número total de pixels da matriz
#define DEBOUNCING_TIME_US 220 // Tempo de debounce para os botões, em microssegundos
#define NUMLEDS 3             // Número de LEDs (vermelho, verde, azul)
#define I2C_PORT i2c1         // Definição do barramento I2C
#define I2C_SDA 14            // Pino SDA para comunicação I2C
#define I2C_SCL 15            // Pino SCL para comunicação I2C
#define endereco 0x3C         // Endereço do display I2C (SSD1306)
#define WIDTH 128 
#define HEIGHT 64

// Declaração de variáveis globais

volatile _Atomic uint estado;                // Estado atual do sistema (0 - vermelho, 1 - amarelo, 2 - verde)
static uint8_t leds[3] = {LED_RED, LED_GREEN, LED_BLUE};  // Vetor para os pinos dos LEDs (vermelho, verde, azul)
volatile _Atomic uint8_t outroContador = 0;  // Contador auxiliar
volatile uint32_t hora_presente;             // Armazena a hora atual
static volatile uint32_t ultimo_pressionamento = 0;  // Hora do último pressionamento de botão
struct repeating_timer timer;                // Timer repetitivo
volatile _Atomic uint saida_teste = 0;      // Flag para indicar saída de teste (modo bootloader)
volatile bool isRunning = false;             // Flag indicando se o sistema está em execução

// Variáveis de controle dos botões
volatile _Atomic uint numero_display = 0;    // Número a ser exibido no display
volatile _Atomic uint cor_atual = 1;         // Cor atual (1 - vermelho, 2 - verde, 3 - azul)
volatile _Atomic uint brilho = 1;            // Nível de brilho (1 a 4)
volatile _Atomic uint tempo = 5;             // Tempo de exibição (1 a 4)
volatile _Atomic uint var_teste = 0;         // Variável de teste auxiliar
volatile _Atomic uint sobe_um = 0;           // Flag para o botão A (sobe)
volatile _Atomic uint desce_um = 0;          // Flag para o botão B (desce)
int randon = 0;                              // Variável auxiliar para valores aleatórios
static bool randonTwo = 0;                   // Outra variável auxiliar para aleatoriedade
volatile _Atomic char leitura_UART;          // Variável para leitura de dados via UART
absolute_time_t debounce_do_serial;          // Tempo para debounce do serial

// PIO e controle de estado da máquina
PIO pio_controlador = pio0;                  // Definição do controlador PIO
bool status_init_pio = false;                // Flag para status de inicialização do PIO
uint state_machine = 0;                       // Variável para estado da máquina do PIO
bool isSerial = true;                        // Flag para controle da comunicação serial
ssd1306_t ssd;                               // Estrutura para o controle do display SSD1306
bool cor = true;                             // Flag para controle de cor no display


// Protótipos das funções

void tratar_botoes(uint gpio, uint32_t events);         // Função que trata as interrupções dos botões
void inicia_hardware();                                // Função que inicializa o hardware (PIO, GPIOs, I2C, etc)
bool verificador_serial();                             // Função para verificar a conexão serial
void init_pio_routine();                               // Função para inicializar o PIO e suas rotinas
void mensagem_serial(ssd1306_t *ssd);                  // Função para mostrar mensagem no display via UART
void mensagem_caracter(ssd1306_t *ssd);                // Função para mostrar um caractere no display
void atualiza_matrix(uint numero_display);                                // Função para atualizar a matriz do display
void liga_verde(int estado);                          // Função para controlar o LED verde
void liga_azul(int estado);                           // Função para controlar o LED azul
void entrarModoBootloader();    

void entrarModoBootloaderDois();
// Em func_generals.h:
void tocar_tom_buzzer(uint16_t frequency, uint32_t duration_ms);

void acende_led();
uint32_t matrix_rgbFlag(double number);

void ligar_todos_os_leds();
void desliga_tudo();
void debounce();
void boas_vindas();

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
void boas_vindas(ssd1306_t *ssd)
{
  ssd1306_draw_string(ssd,"insira:  ; ",4,15); 
  ssd1306_draw_string(ssd,"0 a 9 matriz",4,25);
  ssd1306_draw_string(ssd," A verde",4,35);
  ssd1306_draw_string(ssd,"B  vzul",4,45);
  ssd1306_send_data(ssd); //Atualiza o display
}

void mensagem_serial(ssd1306_t *ssd){//informar para iniciar o serial
  ssd1306_fill(ssd,false); //Limpa display
  ssd1306_rect(ssd,3,3,122,58,true,false); //Desenha retângulo
  ssd1306_draw_string(ssd,"iniciar serial",6,30);
  ssd1306_send_data(ssd);
}
void mensagem_caracter(ssd1306_t *ssd){//se iniciado repetir o ultimo char
  ssd1306_fill(ssd,!cor); //Limpa display
  ssd1306_rect(ssd,3,3,122,58,cor,!cor); //Desenha retângulo
  ssd1306_draw_string(ssd,"char inserido: ",8,30); 
  ssd1306_draw_char(ssd,leitura_UART,90,30); //Imprime o caracter lido
  ssd1306_send_data(ssd); //Atualiza o display
}
void inicia_hardware()
{
    //inicializa pio

    init_pio_routine();
    stdio_init_all();
   // stdio_usb_init();
    //inicializa leds 11 
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
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
//
    ssd1306_fill(&ssd, true);
    ssd1306_send_data(&ssd);
    sleep_ms(500);
//
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    //iniciar repeating timer para por em tela caso o seria n seja acionado
//    add_repeating_timer_ms(1000, verificador_serial, false, &timer);

}
bool verificador_serial()//roda a cada 1000 ms
{
   sleep_ms(50);

int caractereRecebido = stdio_getchar_timeout_us(0);

  sleep_ms(50); // Pequeno atraso antes da leitura

  

    if (caractereRecebido != PICO_ERROR_TIMEOUT) {
        mensagem_serial(&ssd);
        isSerial = false;
    } 
    else if (!isSerial) { 
        isSerial = true;
        mensagem_caracter(&ssd);
    }

    return true;

}


void init_pio_routine()
{
    //inicia o PIO
    //sem struct
    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
   status_init_pio = set_sys_clock_khz(128000, false);

  

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
    sleep_ms(50);
    boas_vindas(&ssd);
    sleep_ms(50);
    entrarModoBootloaderDois();
    
    debounce_do_serial=delayed_by_ms(get_absolute_time(), 2000);
 
    
    //mensagem_serial(&ssd);
    while (true) 
    {
        sleep_ms(1000);
        //mensagem_serial(&ssd);
        printf("\n\nleitura do terminal\n\n");
        int teste_usb_leitura=stdio_getchar_timeout_us(0);
        if(teste_usb_leitura!=PICO_ERROR_TIMEOUT)
        {
            sleep_ms(50);
            //ler do terminal
            leitura_UART=(char)teste_usb_leitura;
            
                    printf("\n\nlido: '%c'\n", leitura_UART);
                    if(leitura_UART<=48&&leitura_UART>=57)
                    {
                        numero_display=leitura_UART;
                        atualiza_matrix(numero_display);
                    }else if((leitura_UART>=65&&leitura_UART<=90)||(leitura_UART>=97&&leitura_UART<=122))
                    {
                        mensagem_caracter(&ssd);

                    }
                
        }sleep_ms(40);
        atualiza_matrix(numero_display);
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


////
//*************************************************** */




void entrarModoBootloader()
{
    desliga_tudo();
    ligar_todos_os_leds();
    desliga_tudo();
    ligar_todos_os_leds();
    sleep_ms(100);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], true); //
    }
    tocar_tom_buzzer(1000, 200);
    sleep_ms(100);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], false); //
    }
    tocar_tom_buzzer(1500, 200);
    sleep_ms(100);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], true); //
    }
    tocar_tom_buzzer(2000, 300);
    sleep_ms(100);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], false); //
    }
    tocar_tom_buzzer(1000, 200);
    sleep_ms(50);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], true); //
    }
    tocar_tom_buzzer(1500, 200);
    sleep_ms(50);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], false); //
    }
    tocar_tom_buzzer(2000, 400);
    reset_usb_boot(0, 0); // Reinicia no modo bootloader
}
void entrarModoBootloaderDois()
{
    ligar_todos_os_leds();
    desliga_tudo();
    sleep_ms(100);
    ligar_todos_os_leds();
    desliga_tudo();
    sleep_ms(100);
    ligar_todos_os_leds();
    sleep_ms(100);
    tocar_tom_buzzer(2000, 300);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], true); //
    } 
    sleep_ms(150);
    tocar_tom_buzzer(1500, 250);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], false); //
    } 
    sleep_ms(150);
    tocar_tom_buzzer(1000, 250);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], true); //
    } 
    sleep_ms(200);
    tocar_tom_buzzer(800, 300);
    sleep_ms(250);
    tocar_tom_buzzer(600, 400);
    for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], false); //
    } 
    sleep_ms(300);
    tocar_tom_buzzer(400, 500);
    desliga_tudo();
    sleep_ms(100);
    ligar_todos_os_leds();
    desliga_tudo();
    sleep_ms(100);
    ligar_todos_os_leds();
    sleep_ms(100);
}
void tocar_tom_buzzer(uint16_t frequency, uint32_t duration_ms) {
   gpio_set_function(BUZZER_B_PIN, GPIO_FUNC_PWM); // Configura pino do buzzer para PWM
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_B_PIN);

    pwm_set_wrap(slice_num, 125000000 / frequency); // Período do PWM
    pwm_set_gpio_level(BUZZER_B_PIN, (125000000 / frequency) / 2); // Duty cycle 50%
    pwm_set_enabled(slice_num, true); // Ativa o PWM

    sleep_ms(duration_ms); // Toca por tempo especificado

    pwm_set_enabled(slice_num, false); // Desliga o PWM
    gpio_set_function(BUZZER_B_PIN, GPIO_FUNC_SIO);
    gpio_put(BUZZER_B_PIN, 0);

}

void liga_verde(int estado)
{
    gpio_put(leds[1], estado);

    return;
}
void liga_azul(int estado) // usar função separada pra ligar amarelo para n bagunçar
{
    gpio_put(leds[2], estado); // liga vermelho e verde
    return;
}
//gera_rgb
uint32_t matrix_rgbFlag(double number)
{
     // Certifica-se de que 'number' esteja no intervalo [0, 1]
    number = fmax(0.0, fmin(1.0, number));
    double brilhoLeds=0;
    if(var_teste==1){
         brilhoLeds=1;
         randon=4;
    }else{
     brilhoLeds=0.25*brilho;
        randon=(rand()%4)+1;

    }


    // Definir as variáveis de cor RGB
    unsigned char R = 0, G = 0, B = 0;
if(randon>=1&&randon<=3){
    // Ajuste de cor baseado na variável 'cor_atual'
    if (randon == 1) { // Vermelho
        R = (unsigned char)(number *brilhoLeds* 255.0); // R é o máximo, G e B são 0
    } else if (randon == 2) { // Verde
        G = (unsigned char)(number *brilhoLeds*  255.0); // G é o máximo, R e B são 0
    } else if (randon == 3) { // Azul
        B = (unsigned char)(number *brilhoLeds*  255.0); // B é o máximo, R e G são 0
    }
}else if(randon==4)//se or and for 4 o led fica branco
{
        R = (unsigned char)(number *brilhoLeds* 255.0); // R é o máximo, G e B são 0
        G = (unsigned char)(number *brilhoLeds*  255.0); // G é o máximo, R e B são 0
        B = (unsigned char)(number *brilhoLeds*  255.0); // B é o máximo, R e G são 0
    
}
if( randonTwo==true)//se or and for 4 o led fica branco
{
        R = (unsigned char)(number *brilhoLeds* 255.0); // R é o máximo, G e B são 0
        G = (unsigned char)(number *brilhoLeds*  255.0); // G é o máximo, R e B são 0
        B = (unsigned char)(number *brilhoLeds*  255.0); // B é o máximo, R e G são 0
    
}
randon=0;
    // Retorna o valor RGB no formato ARGB (Alpha, Red, Green, Blue)
    return (G << 24) | (R << 16) | (B << 8) | 0xFF;
}

//debounce()
void debounce()
{
    sleep_ms(50);
}
//atualiza_matrix
void atualiza_matrix(uint numero_display)
{
 uint32_t valor_led = 0;

    tocar_tom_buzzer(1000, 200); 
    sleep_ms(20);              

 
        for (int16_t i = 0; i < NUM_PIXELS; i++) //laço pixels ou colnas
        {

            valor_led = matrix_rgbFlag(numbers[numero_display][24-i]);
          
            pio_sm_put_blocking(pio_controlador, state_machine, valor_led);
            sleep_us(50);
   
    }
        sleep_ms(50);
           
    
}

void ligar_todos_os_leds()
{
    uint32_t valor_led = 0;
    double ligar_todos_os_leds_20[25] =   {1.0, 1.0, 1.0, 1.0, 1.0,
                                        1.0, 1.0, 1.0, 1.0, 1.0, 
                                        1.0, 1.0, 1.0, 1.0, 1.0,
                                        1.0, 1.0, 1.0, 1.0, 1.0,
                                        1.0, 1.0, 1.0, 1.0, 1.0};

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgbFlag(ligar_todos_os_leds_20[i]);
        pio_sm_put_blocking(pio_controlador, state_machine, valor_led);
    }
}
void desliga_tudo()
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgbFlag(0.0);
        pio_sm_put_blocking(pio_controlador, state_machine, valor_led);

    }
  
}
