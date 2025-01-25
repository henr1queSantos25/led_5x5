#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

uint rows[4] = {16, 17, 18, 19};
uint columns[4] = {20, 21, 22, 26};

char KEY_MAP[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'};

uint all_columns_mask = 0x0;
uint column_mask[4];

void pico_keypad_init(void)
{
    for (int i = 0; i < 4; i++)
    {
        gpio_init(columns[i]);
        gpio_pull_up(columns[i]);
        gpio_set_dir(columns[i], GPIO_IN);
        all_columns_mask |= (1 << columns[i]);
        column_mask[i] = (1 << columns[i]);

        gpio_init(rows[i]);
        gpio_set_dir(rows[i], GPIO_OUT);
        gpio_put(rows[i], 1);
    }
}

char pico_keypad_get_key(void)
{
    for (int row = 0; row < 4; row++)
    {
        gpio_put(rows[row], 0);
        busy_wait_us(100);

        uint32_t cols = gpio_get_all() & all_columns_mask;

        gpio_put(rows[row], 1);

        if (cols != all_columns_mask)
        {
            for (int col = 0; col < 4; col++)
            {
                if ((cols & column_mask[col]) == 0)
                {
                    return KEY_MAP[row * 4 + col];
                }
            }
        }
    }

    return 0;
}