#include "main.h"
#include "cmsis_os.h"
#include "queue.h"

typedef struct handler_uart_s {
	QueueHandle_t 		xQueue_tx, xQueue_rx;
	UART_HandleTypeDef	*huart;
} handler_uart_t;

void task_create_uart_tx(handler_uart_t *handle);
void uart_tx_task( void * handle );
