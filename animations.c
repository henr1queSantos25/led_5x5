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

// Função para criar a animação de um olho
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