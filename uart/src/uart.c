#include <stdlib.h>
#include <string.h>

#include "uart.h"
#include "uart_priv.h"

struct uart *
uart_init(int id)
{
    struct uart *u;

    u = malloc(sizeof(struct uart));
    if (u == NULL)
        return NULL;

    memset(u, 0, sizeof(struct uart));
    u->id = id;
    return u;
}

int
uart_set_baudrate(struct uart *uart, int baudrate)
{
    if (uart == NULL)
        return -UART_ERR;

    switch (baudrate)
    {
        /* TODO: support more rates */
        case 9600:
        case 115200:
            uart->baudrate = baudrate;
            break;
        default:
            return -UART_ERR;
    }

    return UART_OK;
}

int
uart_set_parity(struct uart *uart, int parity)
{
    if (uart == NULL)
        return -UART_ERR;

    if (parity < 0 || parity > 2)
        return -UART_ERR;

    uart->parity = parity;
    return UART_OK;
}

int
uart_set_stop_bits(struct uart *uart, int stop_bits)
{
    if (uart == NULL)
        return -UART_ERR;

    if (stop_bits < 1 || stop_bits > 2)
        return -UART_ERR;

    uart->stop_bits = stop_bits;
    return UART_OK;
}

int
uart_set_rx_cb(struct uart *uart, void (*rx_cb)(int *priv, char data))
{
    if (uart == NULL)
        return -UART_ERR;

    if (rx_cb == NULL)
        return -UART_ERR;

    uart->rx_cb = rx_cb;
    return UART_OK;
}

int
uart_set_tx_cb(struct uart *uart, void (*tx_cb)(int *priv, int nbytes))
{
    if (uart == NULL)
        return -UART_ERR;

    if (tx_cb == NULL)
        return -UART_ERR;

    uart->tx_cb = tx_cb;
    return UART_OK;
}

int
uart_send_data(struct uart *uart, char *buf, int size)
{
    if (uart == NULL)
        return -UART_ERR;

    if (buf == NULL || size == 0)
        return UART_ERR;

    uart->buf = buf;
    uart->cnt = size;

    if (uart->enabled)
        return uart_start_tx(uart);

    return UART_OK;
}

int
uart_enable(struct uart *uart, int new_state)
{
    if (uart == NULL)
        return -UART_ERR;

    if (new_state == 0)
        uart->enabled = 0;
    else
        uart->enabled = 1;

    return uart_enable_hw(uart);
}

int
uart_deinit(struct uart *uart)
{
    if (uart == NULL)
        return -UART_ERR;

    free(uart);

    return UART_OK;
}

int
uart_push(struct uart *uart, char data)
{
    uart->rx_cb(&uart->id, data);
    return UART_OK;
}
