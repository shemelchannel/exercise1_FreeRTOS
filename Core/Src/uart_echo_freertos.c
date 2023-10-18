#include "uart_echo_freertos.h"
#include "cmsis_os.h"
#include "queue.h"
#include "task.h"

static handler_uart_t *h;

void uart_tx_task( void * handle ) {


	while(1) {
		//xQueueReceive(h->tx, buf,  portMAX_DELAY);
		//HAL_UART_Transmit(h->tx, buf,  sizeof(uint8_t), 10);
		HAL_UART_Transmit(h->huart, (uint8_t*)"Hello world!\n", 13, 1000);
		vTaskDelay(1000);
	}
}

void task_create_uart_tx(handler_uart_t *handle) {

	h = handle;
	TaskHandle_t xHandle = NULL;

	h->xQueue_tx = xQueueCreate( 10, sizeof( uint8_t ) );
	h->xQueue_rx = xQueueCreate( 10, sizeof( uint8_t ) );
	xTaskCreate( uart_tx_task, "NAME", 256, &handle, 1, &xHandle);

	vTaskStartScheduler();
}
