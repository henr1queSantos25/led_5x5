#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// arquivo .pio
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25

// pino do led 5x5
#define OUT_PIN 7


extern void pico_keypad_init(void);
extern char pico_keypad_get_key(void);

// FUNÇÕES DAS ANIMAÇÕES
extern void animacao_olho(PIO pio, uint sm);
extern void coracao_batendo(uint32_t valor_led, PIO pio, uint sm, int repeticoes, int delay_ms);
extern void tetrix(PIO pio, uint sm);
extern void seta(PIO pio, uint sm);
extern void carinha(PIO pio, uint sm);
extern void cobra_animacao(uint32_t valor_led, PIO pio, uint sm, int repeticoes, int delay_ms);

extern void espiral_expansiva(PIO pio, uint sm);

// rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// rotina para acionar a matrix de leds - ws2812b
void desenho_pio(uint32_t valor_led, PIO pio, uint sm, char caracter_press)
{
    if (caracter_press == 'A')
    {
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, 0.0, 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }

    else if (caracter_press == 'B') // Novo caso para tecla 'B'
    {

        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(1.0, 0.0, 0.0); // LEDs na cor azul com 100% de intensidade
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }

    else if (caracter_press == 'C') // Caso para tecla 'C'
    {
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, 0.8, 0.0); // LEDs na cor verde com 80% de intensidade
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }

    else if (caracter_press == 'D')
    {
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, 0.0, 0.5);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }

    else if (caracter_press == '#')
    {
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.2, 0.2, 0.2);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }
}

// Função para reboot
void reboot()
{
    reset_usb_boot(0, 0);
}

int main()
{
    PIO pio = pio0;
    uint32_t valor_led;

    pico_keypad_init();
    stdio_init_all();
    char last_key = 0;

    // configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    while (true)
    {
        char caracter_press = pico_keypad_get_key();

        if (caracter_press == 'A' && caracter_press != last_key)
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            desenho_pio(valor_led, pio, sm, caracter_press);
        }

        else if (caracter_press == 'B' && caracter_press != last_key) // Se caso para tecla 'B'
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            desenho_pio(valor_led, pio, sm, caracter_press); // Chamada para acender LEDs azuis
        }

        else if (caracter_press == 'C' && caracter_press != last_key)
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            desenho_pio(valor_led, pio, sm, caracter_press);
        }

        else if (caracter_press == 'D' && caracter_press != last_key)
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            desenho_pio(valor_led, pio, sm, caracter_press);
        }

        else if (caracter_press == '1' && caracter_press != last_key) // Se caso para tecla '1'
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            coracao_batendo(valor_led, pio, sm, 3, 150);
        }

        else if (caracter_press == '2' && caracter_press != last_key) // Se caso para tecla '2'
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            animacao_olho(pio, sm); // Chamada para acender LEDs azuis
        }

        else if (caracter_press == '3' && caracter_press != last_key) // Se caso para tecla '3'
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            tetrix(pio, sm); // função para gerar animação do leds
        }
        
        else if (caracter_press == '4' && caracter_press != last_key) // Se caso para tecla '4'
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            seta(pio, sm); // função para gerar animação do leds
        }


        else if (caracter_press == '5' && caracter_press != last_key) // Se caso para tecla '5'
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            carinha(pio, sm); 
        }

        else if (caracter_press == '6' && caracter_press != last_key) // Se caso para tecla '6'
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            cobra_animacao(valor_led, pio, sm, 3, 150);
        }
      
        else if (caracter_press == '7' && caracter_press != last_key) // Se caso para tecla '7'
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            espiral_expansiva(pio, sm); // função para gerar animação do leds
        }

        else if (caracter_press == '#' && caracter_press != last_key)
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            desenho_pio(valor_led, pio, sm, caracter_press);
        }

        else if (caracter_press == '*' && caracter_press != last_key)
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            reboot();
        }

        else if (!caracter_press)
        {
            last_key = 0;
        }
      
        else if (caracter_press && caracter_press != last_key)
        {
            last_key = caracter_press;
            printf("\nTecla pressionada: %c\n", caracter_press);
        }

        sleep_ms(500);
    }
}
