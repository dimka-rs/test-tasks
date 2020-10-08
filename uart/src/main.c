#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE (128)

char path_in[] = "/tmp/uart.in";
char path_out[] = "/tmp/uart.out";

char tx_buf1[BUF_SIZE];
char tx_buf2[BUF_SIZE];
char *tx_bufp = tx_buf1;
int tx_off = 0;
struct uart *u1, *u2;
volatile int tx_done = 1;
int fd_out;

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
    int fd_in;
    char data;
    ssize_t nbytes;

    mkfifo(path_in, 0666);
    mkfifo(path_out, 0666);

    init_uarts();

    fd_in = open(path_in, O_RDONLY);
    fd_out = open(path_out, O_WRONLY);

    while(1)
    {
        nbytes = read(fd_in, &data, 1);
        if (nbytes == 1 && data != '.')
        {
            uart_push(u1, data);
        }
    }

    close(fd_out);
    close(fd_in);
    deinit_uarts();
    return 0;
}