#include "uart_echo_freertos.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "string.h"
#include "semphr.h"
#include "event_groups.h"
#include "string.h"

static handler_uart_t *h;
xTaskHandle uart_tx_task_handler;
xTaskHandle uart_rx_task_handler;

xQueueHandle Queue_rx;

uint8_t	rx_data;

void uart_tx_task( void * handle ) {
	int i = 222;
	while(1) {
		if ((xQueueSend(Queue_rx, &i, portMAX_DELAY)) == pdPASS)
		{
			char *str = " Successfully sent the to the queue\nLeaving SENDER1_Task\n\n\n";
			HAL_UART_Transmit(h->huart, (uint8_t *)str, strlen (str), HAL_MAX_DELAY);
		}
		vTaskDelay(2000);
	}
}

void uart_rx_task( void * handle ) {
	int received = 0;
	while(1) {
		/**** RECEIVE FROM QUEUE *****/
		if (xQueueReceive(Queue_rx, &received, portMAX_DELAY) == pdTRUE)
		{
			HAL_UART_Transmit(h->huart, (uint8_t *)"Successfully RECEIVED\n\n", 23, HAL_MAX_DELAY);
		}
		vTaskDelay(1000);
	}
}

void task_create(handler_uart_t *handle) {

	h = handle;

	Queue_rx = xQueueCreate( 10, sizeof( uint8_t ) );
	if (Queue_rx == 0)
	{
		HAL_UART_Transmit(h->huart, (uint8_t *)"Unable to create queue\n\n", 24, HAL_MAX_DELAY);
	}
	xTaskCreate( uart_tx_task, "transmitter", h->task_stack_size, &handle, 2, &uart_tx_task_handler);
	xTaskCreate( uart_rx_task, "receiver", 	  h->task_stack_size, &handle, 1, &uart_rx_task_handler);

	HAL_UART_Receive_IT(h->huart, &rx_data, 1);

	vTaskStartScheduler();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(h->huart, &rx_data, 1);
	int ToSend = 123456789;
	if (rx_data == 'r')
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		if (xQueueSendToFrontFromISR(Queue_rx, &ToSend, &xHigherPriorityTaskWoken) == pdPASS)
		{
			HAL_UART_Transmit(h->huart, (uint8_t *)"\n\nSent from ISR\n\n", 17, 500);
		}
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}


