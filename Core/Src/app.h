#include "main.h"

typedef struct GPIO_APP_s {
	GPIO_TypeDef *Port;
	uint16_t     Pin;
}GPIO_APP_t;

typedef struct APP_handle_s {
	GPIO_APP_t LEDsForBlink[8];
	GPIO_APP_t ToggleBuf[6];
	float frequency;
	uint8_t ButtonState;
}APP_handle_t;


void APP_init(APP_handle_t *handle);
void APP_start(void);


