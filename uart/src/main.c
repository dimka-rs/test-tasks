#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE (16)

char tx_buf1[BUF_SIZE];
char tx_buf2[BUF_SIZE];
char *tx_bufp = tx_buf1;
int tx_off = 0;
struct uart *u1, *u2;
volatile int tx_done = 1;

static void
uart1_rx_cb(int *priv, char data)
{
    tx_bufp[tx_off] = data;
    tx_off++;

    if (tx_off >= BUF_SIZE)
    {
        if (tx_done == 1)
        {
            tx_done = 0;
            tx_off = 0;
            uart_send_data(u2, tx_bufp, BUF_SIZE);
            if (tx_bufp == tx_buf1)
            {
                tx_bufp = tx_buf2;
            }
            else
            {
                tx_bufp = tx_buf1;
            }
        }
        else
        {
            fprintf(stderr, "Current buffer is full and previous buffer "
                    "is not transmitted yet. Dropping data!\n");
        }
    }
}

static void
uart2_tx_cb(int *priv, int nbytes)
{
    tx_done = 1;
}

void
init_uarts()
{
    u1 = uart_init(1);
    uart_set_baudrate(u1, 9600);
    uart_set_parity(u1, 0);
    uart_set_stop_bits(u1, 1);
    uart_set_rx_cb(u1, uart1_rx_cb);
    uart_enable(u1, 1);

    u2 = uart_init(2);
    uart_set_baudrate(u2, 115200);
    uart_set_parity(u2, 0);
    uart_set_stop_bits(u2, 1);
    uart_set_tx_cb(u2, uart2_tx_cb);
    uart_enable(u2, 1);
}

void
deinit_uarts()
{
    uart_deinit(u1);
    uart_deinit(u2);
}

int main(int argc, char **argv)
{
    int c;

    system ("/bin/stty raw");
    init_uarts();

    while((c = getchar()) != '.')
    {
        uart_push(u1, (char)c);
    }

    deinit_uarts();
    system ("/bin/stty cooked");
    return 0;
}