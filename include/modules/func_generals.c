#include "include/libraries/func_generals.h"
#include "include/libraries/numbers_n_array.h"
static uint8_t leds[3] = {13, 11, 12};
PIO pio_controlador = pio0;
bool status_init_pio = false;
uint state_machine =0;



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
    tocar_tom_buzzer(2000, 300);
   /*  for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], true); //
    } */
    sleep_ms(150);
    tocar_tom_buzzer(1500, 250);
    /* for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], false); //
    } */
    sleep_ms(150);
    tocar_tom_buzzer(1000, 250);
   /*  for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], true); //
    } */
    sleep_ms(200);
    tocar_tom_buzzer(800, 300);
    sleep_ms(250);
    tocar_tom_buzzer(600, 400);
   /*  for (uint8_t i = 0; i < NUMLEDS; i++)
    {
        gpio_put(leds[i], false); //
    } */
    sleep_ms(300);
    tocar_tom_buzzer(400, 500);
}
void tocar_tom_buzzer(uint16_t frequency, uint32_t duration_ms) {
   if (frequency == 0) {
        sleep_ms(duration_ms); // Silêncio (pausa)
        return;
    }

    gpio_set_function(BUZZER_B_PIN, GPIO_FUNC_PWM); // Configura pino do buzzer para PWM
    uint sliceNum = pwm_gpio_to_slice_num(BUZZER_B_PIN);

    uint clockDiv = 4; // Ajuste fino para manter precisão
    pwm_set_clkdiv(sliceNum, clockDiv);

    uint wrapValue = (125000000 / clockDiv) / frequency; // Cálculo mais preciso do período
    pwm_set_wrap(sliceNum, wrapValue);
    pwm_set_gpio_level(BUZZER_B_PIN, wrapValue / 2); // Duty cycle 50% (onda quadrada estável)
    
    pwm_set_enabled(sliceNum, true); // Ativa o PWM
    sleep_ms(duration_ms); // Mantém o som pelo tempo especificado

    // Suaviza a transição desligando PWM gradualmente
    for (int i = wrapValue / 2; i > 0; i -= wrapValue / 20) {
        pwm_set_gpio_level(BUZZER_B_PIN, i);
        sleep_ms(2);
    }

    pwm_set_enabled(sliceNum, false); // Desliga o PWM
    gpio_set_function(BUZZER_B_PIN, GPIO_FUNC_SIO);
    gpio_put(BUZZER_B_PIN, 0);

    sleep_ms(20); // Pequena pausa para evitar sobreposição de notas
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
void atualiza_matrix()
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
