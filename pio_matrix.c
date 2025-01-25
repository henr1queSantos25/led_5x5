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

// pino do led 5xr
#define OUT_PIN 7

extern void pico_keypad_init(void);
extern char pico_keypad_get_key(void);

// todos os LEDs deverão ser ligados na cor branca, no nível de intensidade de 20% da luminosidade máxima.
double leds_brancos[25] = {0.2, 0.2, 0.2, 0.2, 0.2,
                           0.2, 0.2, 0.2, 0.2, 0.2,
                           0.2, 0.2, 0.2, 0.2, 0.2,
                           0.2, 0.2, 0.2, 0.2, 0.2,
                           0.2, 0.2, 0.2, 0.2, 0.2};

// configuração para apagar todos os leds com intensidade 0 
double leds_apagados[25] = {0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0};

// vetor para criar imagem na matriz de led - 2
double desenho2[25] = {1.0, 0.0, 0.0, 0.0, 1.0,
                       0.0, 1.0, 0.0, 1.0, 0.0,
                       0.0, 0.0, 1.0, 0.0, 0.0,
                       0.0, 1.0, 0.0, 1.0, 0.0,
                       1.0, 0.0, 0.0, 0.0, 1.0};

// imprimir valor binário
void imprimir_binario(int num)
{
    int i;
    for (i = 31; i >= 0; i--)
    {
        (num & (1 << i)) ? printf("1") : printf("0");
    }
}

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
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b, char caracter_press)
{
    if (caracter_press == '#')
    {
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(desenho[24 - i], desenho[24 - i], desenho[24 - i]);
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
    else if (caracter_press == 'A')
    {
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(desenho[24 - i], desenho[24 - i], desenho[24 - i]);
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
}

int main()
{
    PIO pio = pio0;
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;

    // coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);
    pico_keypad_init();
    stdio_init_all();
    char last_key = 0;

    printf("iniciando a transmissão PIO");
    if (ok)
        printf("clock set to %ld\n", clock_get_hz(clk_sys));

    // configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    while (true)
    {
        char caracter_press = pico_keypad_get_key();

        if (caracter_press == '#' && caracter_press != last_key)
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            desenho_pio(leds_brancos, valor_led, pio, sm, r, g, b, caracter_press);
        }
        else if (caracter_press == 'C' && caracter_press != last_key)
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            desenho_pio(NULL, valor_led, pio, sm, 0.8, 0.0, 0.0, caracter_press);
        }
        else if (caracter_press == 'A' && caracter_press != last_key)
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            desenho_pio(leds_apagados, valor_led, pio, sm, r, g, b, caracter_press);
        }
        else if (caracter_press == 'B' && caracter_press != last_key) // Se caso para tecla 'B'
        {
            printf("\nTecla pressionada: %c\n", caracter_press);
            last_key = caracter_press;
            desenho_pio(NULL, valor_led, pio, sm, 0.0, 0.0, 1.0, caracter_press); // Chamada para acender LEDs azuis
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
        printf("\nFrequência de clock %ld\r\n", clock_get_hz(clk_sys));
    }
}
