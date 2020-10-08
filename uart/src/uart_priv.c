#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uart.h"
#include "uart_priv.h"

int uart_start_tx(struct uart *uart)
{
    char *p = uart->buf;

    while (uart->cnt--)
    {
        putchar(*p);
        p++;
    }

    if (uart->tx_cb != NULL)
        uart->tx_cb(&uart->id, uart->cnt + 1);

    return UART_OK;
}

int
uart_enable_hw(struct uart *uart)
{
    /* TODO: check if settings are correct */
    return UART_OK;
}

