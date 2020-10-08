#ifndef UART_H
#define UART_H
/**
 * @addtogroup uart
 * @{
 */

/**
 * @brief error codes
 */
enum {
    UART_OK,
    UART_ERR,
};

/**
 * @brief internal data structure
 */
struct uart;

/**
 * @brief initialize internal data
 * @param id HW instance id
 * @return opaque pointer to internal data or NULL
 */
struct uart *uart_init(int id);

/**
 * @brief set baudrate
 * @param uart pointer to internal data
 * @param baudrate baudrate to set, e.g. 115200
 * @return 0 on success
 */
int uart_set_baudrate(struct uart *uart, int baudrate);

/**
 * @brief set parity
 * @param uart pointer to internal data
 * @param parity 0 - none, 1 - odd, 2 - even
 * @return 0 on success
 */
int uart_set_parity(struct uart *uart, int parity);

/**
 * @brief set number of stop bits
 * @param uart pointer to internal data
 * @param stop_bits 1 or 2
 * @return 0 on success
 */
int uart_set_stop_bits(struct uart *uart, int stop_bits);

/**
 * @brief set callback function for RX event
 * @param uart pointer to internal data
 * @param rx_cb pointer to callback function
 * @return 0 on success
 */
int uart_set_rx_cb(struct uart *uart, void (*rx_cb)(int *priv, char data));

/**
 * @brief set callback function for TX complete event
 * @param uart pointer to internal data
 * @param tx_cb pointer to callback function
 * @return 0 on success
 */
int uart_set_tx_cb(struct uart *uart, void (*tx_cb)(int *priv, int nbytes));

/**
 * @brief send buffer of given size
 * @param uart pointer to internal data
 * @param buf pointer to char buffer
 * @param size size of data in bytes
 * @return 0 on success
 */
int uart_send_data(struct uart *uart, char *buf, int size);

/**
 * @brief enable or disable UART HW
 * @param uart pointer to internal data
 * @param new_state 0 - disable, 1 - enable
 * @return 0 on success
 */
int uart_enable(struct uart *uart, int new_state);

/**
 * @brief free internal resources
 * @param uart pointer to internal data
 * @return 0 on success
 */
int uart_deinit(struct uart *uart);


/**
 * @brief simulate RX
 * @param uart pointer to internal data
 * @param data received char
 * @return 0 on success
 */
int uart_push(struct uart *uart, char data);

/* @} */
#endif /* UART_H */