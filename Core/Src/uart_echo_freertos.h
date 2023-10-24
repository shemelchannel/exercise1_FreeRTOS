#include "main.h"
#include "cmsis_os.h"
#include "queue.h"
#include "task.h"

typedef struct handler_uart_s {
	QueueHandle_t 		xQueue_tx, xQueue_rx;
	uint32_t			task_stack_size;
	UART_HandleTypeDef	*huart;
} handler_uart_t;

void task_create(handler_uart_t *handle);
void uart_tx_task( void * handle );
void uart_rx_task( void * handle );
