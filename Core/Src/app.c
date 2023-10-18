#include "math.h"
#include "app.h"
#include "main.h"

typedef struct state_s
{
	void (*processing)(struct state_s **current_state);
	struct state_s *next;
	struct state_s *back;
} state_t;

static APP_handle_t *h;
static void set_freq_proc(state_t **current_state);
static void run_proc(state_t **current_state);
static void stop_run_proc(state_t **current_state);
static void reset_proc (state_t **current_state);
void delay (uint32_t ticks);
void on_led (uint32_t led_state);
void off_led (uint32_t led_state);
uint32_t convert(uint32_t num);

uint32_t led_state = 1;
float f = 1;

static state_t* current_state;
static state_t set_freq_state;
static state_t run_state;
static state_t stop_run_state;
static state_t reset_state;

void APP_init(APP_handle_t *handle){

	h = handle;

	set_freq_state.processing = &set_freq_proc;
	set_freq_state.next		  = &run_state;
	set_freq_state.back		  = &stop_run_state;

	run_state.processing = &run_proc;
	run_state.next		 = &stop_run_state;
	run_state.back		 = &set_freq_state;

	stop_run_state.processing = &stop_run_proc;
	stop_run_state.next		  = &reset_state;
	stop_run_state.back		  = &run_state;

	reset_state.processing = &reset_proc;
	reset_state.next	   = &set_freq_state;
	reset_state.back	   = &stop_run_state;

	current_state = &(set_freq_state);
}

void APP_start(void) {
	current_state->processing( &current_state);
}

static void set_freq_proc(state_t **current_state){
	uint32_t bin_num = 0;
	uint32_t CODE[6];
	float f0 = 0.001;

	for (uint8_t i = 0; i < 6; i++)
	{
		CODE[i] = HAL_GPIO_ReadPin(h->ToggleBuf[i].Port, h->ToggleBuf[i].Pin);
		bin_num += CODE[i] * pow(10, i);
	}
	f = f0 * (1 + convert(bin_num));
	bin_num = 0;
	if (h->ButtonState == 0)
		*current_state = current_state[0]->next;
}

static void run_proc (state_t **current_state)
{
	uint8_t flag = 0;

	while (h->ButtonState == 0)
	{
		on_led(led_state);
		HAL_Delay(1 / f);
		if (h->ButtonState == 1)
		{
			*current_state = current_state[0]->next;
		}
		else
		{
			off_led(led_state);
			if (flag == 0)
			{
				led_state++;
				if (led_state == 8)
					flag  = 1;
			}
			else
			{
				led_state--;
				if (led_state == 1)
					flag  = 0;
			}
		}
	}
}

void stop_run_proc (state_t **current_state)
{
	if (h->ButtonState == 0) {
		*current_state = current_state[0]->next;
	}
}

void reset_proc (state_t **current_state)
{
	led_state = 0;
	off_led(led_state);
	if (h->ButtonState == 1) {
		*current_state = current_state[0]->next;
	}
}

uint32_t convert(uint32_t num)
{
	uint32_t dec = 0, i = 0, rem;
	while (num != 0)
  {
		rem = num % 10;
		num /= 10;
		dec += rem * pow(2, i);
		++i;
  }
	return dec;
}

void on_led (uint32_t led_state)
{
	switch(led_state) {
		case 1 :
			HAL_GPIO_WritePin(h->LEDsForBlink[0].Port, h->LEDsForBlink[0].Pin, GPIO_PIN_SET);
			break;
		case 2 :
			HAL_GPIO_WritePin(h->LEDsForBlink[1].Port, h->LEDsForBlink[1].Pin, GPIO_PIN_SET);
			break;
		case 3 :
			HAL_GPIO_WritePin(h->LEDsForBlink[2].Port, h->LEDsForBlink[2].Pin, GPIO_PIN_SET);
			break;
		case 4 :
			HAL_GPIO_WritePin(h->LEDsForBlink[3].Port, h->LEDsForBlink[3].Pin, GPIO_PIN_SET);
			break;
		case 5 :
			HAL_GPIO_WritePin(h->LEDsForBlink[4].Port, h->LEDsForBlink[4].Pin, GPIO_PIN_RESET);
			break;
		case 6 :
			HAL_GPIO_WritePin(h->LEDsForBlink[5].Port, h->LEDsForBlink[5].Pin, GPIO_PIN_RESET);
			break;
		case 7 :
			HAL_GPIO_WritePin(h->LEDsForBlink[6].Port, h->LEDsForBlink[6].Pin, GPIO_PIN_RESET);
			break;
		case 8 :
			HAL_GPIO_WritePin(h->LEDsForBlink[7].Port, h->LEDsForBlink[7].Pin, GPIO_PIN_RESET);
			break;
	}
}

void off_led (uint32_t led_state)
{
	switch(led_state) {
		case 1 :
			HAL_GPIO_WritePin(h->LEDsForBlink[0].Port, h->LEDsForBlink[0].Pin, GPIO_PIN_RESET);
			break;
		case 2 :
			HAL_GPIO_WritePin(h->LEDsForBlink[1].Port, h->LEDsForBlink[1].Pin, GPIO_PIN_RESET);
			break;
		case 3 :
			HAL_GPIO_WritePin(h->LEDsForBlink[2].Port, h->LEDsForBlink[2].Pin, GPIO_PIN_RESET);
			break;
		case 4 :
			HAL_GPIO_WritePin(h->LEDsForBlink[3].Port, h->LEDsForBlink[3].Pin, GPIO_PIN_RESET);
			break;
		case 5 :
			HAL_GPIO_WritePin(h->LEDsForBlink[4].Port, h->LEDsForBlink[4].Pin, GPIO_PIN_SET);
			break;
		case 6 :
			HAL_GPIO_WritePin(h->LEDsForBlink[5].Port, h->LEDsForBlink[5].Pin, GPIO_PIN_SET);
			break;
		case 7 :
			HAL_GPIO_WritePin(h->LEDsForBlink[6].Port, h->LEDsForBlink[6].Pin, GPIO_PIN_SET);
			break;
		case 8 :
			HAL_GPIO_WritePin(h->LEDsForBlink[7].Port, h->LEDsForBlink[7].Pin, GPIO_PIN_SET);
			break;
		default :
			HAL_GPIO_WritePin(h->LEDsForBlink[0].Port, h->LEDsForBlink[0].Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(h->LEDsForBlink[1].Port, h->LEDsForBlink[1].Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(h->LEDsForBlink[2].Port, h->LEDsForBlink[2].Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(h->LEDsForBlink[3].Port, h->LEDsForBlink[3].Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(h->LEDsForBlink[4].Port, h->LEDsForBlink[4].Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(h->LEDsForBlink[5].Port, h->LEDsForBlink[5].Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(h->LEDsForBlink[6].Port, h->LEDsForBlink[6].Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(h->LEDsForBlink[7].Port, h->LEDsForBlink[7].Pin, GPIO_PIN_SET);
			break;
	}
}

