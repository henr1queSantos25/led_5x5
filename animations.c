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

extern uint32_t matrix_rgb(double b, double r, double g);

// ########################################### ANIMAÇÃO DE UM OLHO #########################################
void animacao_olho(PIO pio, uint sm)
{
    // Definição dos frames iniciais para o olho
    double frames[21][25] = {
        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 1, 0, 0, 0,
         0, 0, 1, 1, 1,
         0, 1, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 1, 0,
         1, 1, 1, 0, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 1, 0, 0, 0,
         0, 0, 1, 1, 1,
         0, 1, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 1, 0,
         1, 1, 1, 0, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 1, 0, 0, 0,
         0, 0, 1, 1, 1,
         0, 1, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 1, 0,
         1, 1, 1, 0, 0,
         0, 0, 0, 1, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 0, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 0, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0}};

    uint32_t valor_led;

    for (int frame = 0; frame < 21; frame++)
    {
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, frames[frame][24 - i], 0.0); // LEDs vermelhos para o olho
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(400); // Tempo entre os frames
    }
}

// ########################################### CORAÇÃO PULSANDO #########################################

void coracao_batendo(uint32_t valor_led, PIO pio, uint sm, int repeticoes, int delay_ms)
{

    double frames[5][25] = {
        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.5, 0.0, 1.0,
         0.0, 1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.5, 0.5, 0.5, 1.0,
         0.0, 1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 1.0, 1.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0}};

    for (int rep = 0; rep < repeticoes; rep++)
    {
        for (int f = 0; f < 5; f++)
        {
            for (double brilho = 0.2; brilho <= 1.0; brilho += 0.2) // AUMENTA o brilho
            {
                for (int16_t i = 0; i < NUM_PIXELS; i++)
                {
                    double intensidade = frames[f][24 - i] * brilho;
                    valor_led = matrix_rgb(0, intensidade, 0);
                    pio_sm_put_blocking(pio, sm, valor_led);
                }
                sleep_ms(delay_ms / 2);
            }

            for (double brilho = 1.0; brilho >= 0.2; brilho -= 0.2) // DIMINUI o brilho
            {
                for (int16_t i = 0; i < NUM_PIXELS; i++)
                {
                    double intensidade = frames[f][24 - i] * brilho;
                    valor_led = matrix_rgb(0, intensidade, 0);
                    pio_sm_put_blocking(pio, sm, valor_led);
                }
                sleep_ms(delay_ms / 2);
            }
        }
    }
}
