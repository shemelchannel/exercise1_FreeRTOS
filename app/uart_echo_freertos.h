#define QUEUE_LENGTH	10
#define ITEM_SIZE		sizeof( uint8_t )

typedef struct handler_uart_s {
	StaticQueue_t 		*xStaticQueue;
	uint8_t				ucQueueStorageArea[ QUEUE_LENGTH * ITEM_SIZE];
	UART_HandleTypeDef	*huart;
} handler_uart_t;

void task_create_uart_tx(handler_uart_t *handle);

