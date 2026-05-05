#ifndef SRC_LED_KEY_ONPYNQ_H_
#define SRC_LED_KEY_ONPYNQ_H_

#include "xgpiops.h"

#define KEY_NONE 0
#define KEY0 1
#define KEY1 2
#define KEY2 3
#define KEY3 4
#define LED0 58
#define LED1 59
#define LED2 60
#define LED3 61

uint8_t emio_init(XGpioPs *Gpio);
uint8_t key_scan(XGpioPs *Gpio);

#endif /* SRC_LED_KEY_ONPYNQ_H_ */
