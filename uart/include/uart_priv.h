#ifndef UART_PRIV_H
#define UART_PRIV_H

/**
 * @addtogroup uart_priv
 * @{
 */

/** @brief internal data */
struct uart
{
    /** @brief  HW instance id */
    int id;
    /** @brief  baudrate */
    int baudrate;
    /** @brief  parity */
    int parity;
    /** @brief  stop bits */
    int stop_bits;
    /** @brief  RX complete callback */
    void (*rx_cb)(int *priv, char data);
    /** @brief  TX complete callback */
    void (*tx_cb)(int *priv, int nbytes);
    /** @brief pointer to data not txed yet */
    char *buf;
    /** @brief size of data not txed yet */
    int cnt;
    /** @brief enable state */
    int enabled;
};

/**
 * @brief start transmittion
 * @param uart internal data
 * @return 0 on success
 */
int uart_start_tx(struct uart *uart);

/**
 * @brief configure and enable/disable HW
 * @param uart internal data
 * @return 0 on success
 */
int uart_enable_hw(struct uart *uart);

/* @} */
#endif /* UART_PRIV_H */