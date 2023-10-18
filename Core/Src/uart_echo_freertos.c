static handler_uart_t *h;
static void uart_tx_task(void const *handle);

void task_create_uart_tx(handler_uart_t *handle) {
	h = handle;
	QueueHandle_t xQueue_tx;
	xQueue_tx = xQueueCreateStatic(  QUEUE_LENGTH,
								 	 ITEM_SIZE,
									 h->ucQueueStorageArea,
									 h->xStaticQueue );
	xTaskCreate(uart_tx_task,
			"Nasty_task",
			256,
			( void * ) handle,
			configMAX_PRIORITIES,
			handle);
}

static void uart_tx_task(void const *handle) {


	while(1) {
		//xQueueReceive(h->tx, buf,  portMAX_DELAY);
		//HAL_UART_Transmit(h->tx, buf,  sizeof(uint8_t), 10);
	}
}
