#include "led_key_onPynq.h"
#include "xparameters.h"

static const uint32_t KEY_PINS[] = {54, 55, 56, 57};
static const uint32_t LED_PINS[] = {58, 59, 60, 61};
#define KEY_COUNT (sizeof(KEY_PINS) / sizeof(KEY_PINS[0]))
#define LED_COUNT (sizeof(LED_PINS) / sizeof(LED_PINS[0]))

uint8_t emio_init(XGpioPs *Gpio) {
        XGpioPs_Config *gcfgPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
        uint8_t status = XGpioPs_CfgInitialize(Gpio, gcfgPtr, gcfgPtr->BaseAddr);
        if (status != XST_SUCCESS)
                return XST_FAILURE;
        // initial Led
        for (uint8_t i = 0; i < LED_COUNT; i++) {
                XGpioPs_SetDirectionPin(Gpio, LED_PINS[i], 1);
                XGpioPs_SetOutputEnablePin(Gpio, LED_PINS[i], 1);
        }
        // initial Key
        for (uint8_t k = 0; k < KEY_COUNT; k++) {
                XGpioPs_SetDirectionPin(Gpio, KEY_PINS[k], 0);
        }
        // initial bl_ctr, rst, spi_sclk, spi_mosi, spi_cs
        for (uint8_t i = 62; i < 67; i++) {
                XGpioPs_SetDirectionPin(Gpio, i, 1);
                XGpioPs_SetOutputEnablePin(Gpio, i, 1);
        }
        
        return XST_SUCCESS;
}

// return pressed button pin
uint8_t key_scan(XGpioPs *Gpio) {
        static uint8_t key_old[4] = {1, 1, 1, 1};
        for (uint8_t i = 0; i < KEY_COUNT; i++) {
                uint8_t key_now = XGpioPs_ReadPin(Gpio, KEY_PINS[i]);
                if (key_now == 0 && key_old[i] == 1) {
                        key_old[i] = key_now;
                        return (i + 1);
                }
                key_old[i] = key_now;
        }
        return KEY_NONE;
};
