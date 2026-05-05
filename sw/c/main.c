#include "PWM.h"
#include "led_key_onPynq.h"
#include "pic.h"
#include "sleep.h"
#include "xaxivdma.h"
#include "xgpiops.h"
#include "xil_cache.h"
#include "xparameters.h"

#define SPI_SCLK 64
#define SPI_MOSI 65
#define SPI_CS 66

#define LCD_RGB_ORIENTATION 0

#define RGB888_RED 0x0000FF
#define RGB888_GREEN 0x00FF00
#define RGB888_BLUE 0xFF0000
#define RGB888_WHITE 0xFFFFFF

#define PWM_BASEADDR XPAR_PWM_0_PWM_AXI_BASEADDR

int run_triple_frame_buffer(XAxiVdma *InstancePtr, int DeviceId, int hsize,
                            int vsize, int buf_base_addr, int number_frame_count,
                            int enable_frm_cnt_intr);
unsigned int srcBuffer = (XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x1000000);

void spi_delay(uint16_t cnt) {
        for (volatile int k = 0; k < cnt; k++)
                ;
}

void spi_send(XGpioPs *Gpio, uint8_t *buf, int len) {
        XGpioPs_WritePin(Gpio, SPI_SCLK, 0);
        spi_delay(10000);
        XGpioPs_WritePin(Gpio, SPI_CS, 0);
        spi_delay(10000);
        for (uint8_t j = 0; j < len; j++) {
                uint8_t element = buf[j];
                for (uint8_t i = 0; i < 8; i++) {
                        if (element & 0x80) {
                                XGpioPs_WritePin(Gpio, SPI_MOSI, 1);
                        } else {
                                XGpioPs_WritePin(Gpio, SPI_MOSI, 0);
                        }
                        spi_delay(10000);
                        element <<= 1;
                        XGpioPs_WritePin(Gpio, SPI_SCLK, 1);
                        spi_delay(10000);
                        XGpioPs_WritePin(Gpio, SPI_SCLK, 0);
                        spi_delay(10000);
                }
        }
        XGpioPs_WritePin(Gpio, SPI_CS, 1);
        spi_delay(10000);
}

void spi_send_cmd(XGpioPs *Gpio, uint8_t cmd) {
        uint8_t send_buf[2] = {0x70, cmd};
        spi_send(Gpio, send_buf, sizeof(send_buf));
}

void spi_send_data(XGpioPs *Gpio, uint8_t data) {
        uint8_t send_buf[2] = {0x72, data};
        spi_send(Gpio, send_buf, sizeof(send_buf));
}

void lcd_init(XGpioPs *Gpio) {
        // 發送lcd初始化spi命令
        usleep(1000000);
        spi_send_cmd(Gpio, 0xc0);  // Oscillator frequency control
        spi_send_data(Gpio, 0x00); // use PLCK as system clock
        spi_send_data(Gpio, 0x00);

        spi_send_cmd(Gpio, 0x20);
        spi_send_cmd(Gpio, 0x35);
        spi_send_data(Gpio, 0x00);
        spi_send_data(Gpio, 0xC8);

        spi_send_cmd(Gpio, 0xb1);
        spi_send_data(Gpio, 0x06);
        spi_send_data(Gpio, 0x0F);
        spi_send_data(Gpio, 0x0F);

        spi_send_cmd(Gpio, 0xb2);
        spi_send_data(Gpio, 0x00);
        spi_send_data(Gpio, 0xc8);

        spi_send_cmd(Gpio, 0xb3);
        spi_send_data(Gpio, 0x01);

        spi_send_cmd(Gpio, 0xb4);
        spi_send_data(Gpio, 0x04);

        spi_send_cmd(Gpio, 0xb5);
        spi_send_data(Gpio, 0x10);
        spi_send_data(Gpio, 0x30);
        spi_send_data(Gpio, 0x30);
        spi_send_data(Gpio, 0x00);
        spi_send_data(Gpio, 0x00);

        spi_send_cmd(Gpio, 0xb6);  //
        spi_send_data(Gpio, 0x0b); // 0b
        spi_send_data(Gpio, 0x0f);
        spi_send_data(Gpio, 0x3c);
        spi_send_data(Gpio, 0x13);
        spi_send_data(Gpio, 0x13);
        spi_send_data(Gpio, 0xe8);

        spi_send_cmd(Gpio, 0xb7);
        spi_send_data(Gpio, 0x46);
        spi_send_data(Gpio, 0x06);
        spi_send_data(Gpio, 0x0c);
        spi_send_data(Gpio, 0x00);
        spi_send_data(Gpio, 0x00);

        spi_send_cmd(Gpio, 0xc3); // Power Control 3
        spi_send_data(Gpio, 0x07);
        spi_send_data(Gpio, 0x03);
        spi_send_data(Gpio, 0x04);
        spi_send_data(Gpio, 0x04);
        spi_send_data(Gpio, 0x04);
        usleep(400000);

        spi_send_cmd(Gpio, 0xc4);  // Power Control 4
        spi_send_data(Gpio, 0x12); // 11
        spi_send_data(Gpio, 0x24); // 23
        spi_send_data(Gpio, 0x18); // 12   16
        spi_send_data(Gpio, 0x18); // 12   16
        spi_send_data(Gpio, 0x02); // 05
        spi_send_data(Gpio, 0x49); // 6d  49   //6A
        usleep(200000);

        spi_send_cmd(Gpio, 0xc5);  // Power Control 5
        spi_send_data(Gpio, 0x69); // 69
        usleep(100000);

        spi_send_cmd(Gpio, 0xc6);  // Power Control 6
        spi_send_data(Gpio, 0x41); // 41 40
        spi_send_data(Gpio, 0x63);
        usleep(10000);

        spi_send_cmd(Gpio, 0xd0); // Positive Gamma Curve for Red
        spi_send_data(Gpio, 0x01);
        spi_send_data(Gpio, 0x26);
        spi_send_data(Gpio, 0x71);
        spi_send_data(Gpio, 0x16);
        spi_send_data(Gpio, 0x04);
        spi_send_data(Gpio, 0x03);
        spi_send_data(Gpio, 0x51);
        spi_send_data(Gpio, 0x15);
        spi_send_data(Gpio, 0x04);

        spi_send_cmd(Gpio, 0xd1); // Negative Gamma Curve for Red
        spi_send_data(Gpio, 0x01);
        spi_send_data(Gpio, 0x26);
        spi_send_data(Gpio, 0x71);
        spi_send_data(Gpio, 0x16);
        spi_send_data(Gpio, 0x04);
        spi_send_data(Gpio, 0x03);
        spi_send_data(Gpio, 0x51);
        spi_send_data(Gpio, 0x15);
        spi_send_data(Gpio, 0x04);

        spi_send_cmd(Gpio, 0xd2); // Positive Gamma Curve for Green
        spi_send_data(Gpio, 0x01);
        spi_send_data(Gpio, 0x26);
        spi_send_data(Gpio, 0x71);
        spi_send_data(Gpio, 0x16);
        spi_send_data(Gpio, 0x04);
        spi_send_data(Gpio, 0x03);
        spi_send_data(Gpio, 0x51);
        spi_send_data(Gpio, 0x15);
        spi_send_data(Gpio, 0x04);

        spi_send_cmd(Gpio, 0xd3); // Negative Gamma Curve for Green
        spi_send_data(Gpio, 0x01);
        spi_send_data(Gpio, 0x26);
        spi_send_data(Gpio, 0x71);
        spi_send_data(Gpio, 0x16);
        spi_send_data(Gpio, 0x04);
        spi_send_data(Gpio, 0x03);
        spi_send_data(Gpio, 0x51);
        spi_send_data(Gpio, 0x15);
        spi_send_data(Gpio, 0x04);

        spi_send_cmd(Gpio, 0xd4); // Positive Gamma Curve for Blue
        spi_send_data(Gpio, 0x01);
        spi_send_data(Gpio, 0x26);
        spi_send_data(Gpio, 0x71);
        spi_send_data(Gpio, 0x16);
        spi_send_data(Gpio, 0x04);
        spi_send_data(Gpio, 0x03);
        spi_send_data(Gpio, 0x51);
        spi_send_data(Gpio, 0x15);
        spi_send_data(Gpio, 0x04);

        spi_send_cmd(Gpio, 0xd5); // Negative Gamma Curve for Blue
        spi_send_data(Gpio, 0x01);
        spi_send_data(Gpio, 0x26);
        spi_send_data(Gpio, 0x71);
        spi_send_data(Gpio, 0x16);
        spi_send_data(Gpio, 0x04);
        spi_send_data(Gpio, 0x03);
        spi_send_data(Gpio, 0x51);
        spi_send_data(Gpio, 0x15);
        spi_send_data(Gpio, 0x04);

        spi_send_cmd(Gpio, 0x11); // Sleep Out
        usleep(200000);

        spi_send_cmd(Gpio, 0x29); // Display On
        usleep(100000);

        spi_send_cmd(Gpio, 0x3a);
        spi_send_data(Gpio, 0x77); // 16/18/24bit

        spi_send_cmd(Gpio, 0x36);
#if LCD_RGB_ORIENTATION // 是否旋转90度
        spi_send_data(Gpio, 0x02);
#else
        spi_send_data(Gpio, 0x00);
#endif
}

void led_blink(XGpioPs *Gpio, uint32_t pin) {
        XGpioPs_WritePin(Gpio, pin, 0x1);
        usleep(500000);
        XGpioPs_WritePin(Gpio, pin, 0x0);
}

void reset_lcd(XGpioPs *Gpio) {
        XGpioPs_WritePin(Gpio, 63, 0x0);
        usleep(200000);
        XGpioPs_WritePin(Gpio, 63, 0x1);
        usleep(200000);
}

void bl_on(uint32_t period_clocks) {
        // XGpioPs_WritePin(Gpio, 62, 0x1);
        PWM_Set_Period(PWM_BASEADDR, period_clocks);
        PWM_Set_Duty(PWM_BASEADDR, period_clocks / 2, 0);
        PWM_Enable(PWM_BASEADDR);
}

void fill_color_buffer(uint32_t buffer_addr, uint32_t color) {
        uint8_t *ptr = (uint8_t *)buffer_addr;
        for (int i = 0; i < (800 * 480); i++) {
                ptr[i * 3 + 0] = (color >> 16) & 0xFF; // R
                ptr[i * 3 + 1] = (color >> 8) & 0xFF;  // G
                ptr[i * 3 + 2] = (color) & 0xFF;       // B
        }
        // 重要：寫入後必須沖刷快取 (Cache Flush)，確保 VDMA 能從 DDR 讀到最新數據
        Xil_DCacheFlushRange(buffer_addr, 480 * 800 * 3);
}

void Pic_Display_Vertical(uint8_t *frame, const uint8_t *Pic) {
        uint16_t src_w, src_h;
        uint16_t x_s, y_s;
        uint32_t target_idx;
        uint32_t src_idx;

        // 從圖片頭部讀取原始寬高
        src_w = (Pic[2] << 8) | Pic[3];
        src_h = (Pic[4] << 8) | Pic[5];

        // 遍歷原始圖片的每一個像素
        for (y_s = 0; y_s < src_h; y_s++) {
                for (x_s = 0; x_s < src_w; x_s++) {
                        // 1. 計算原始圖片索引 (跳過 8 位元組文件頭)
                        src_idx = 8 + (y_s * src_w + x_s) * 3;

                        uint16_t x_t = (480 - 1) - y_s;
                        uint16_t y_t = x_s;

                        // 2. 順時針旋轉 90 度並靠右上對齊
                        // 目標 x = (螢幕寬度 - 圖片旋轉後的寬度) + 當前 y
                        // 目標 y = 當前 x
                        if (x_t < 480 && y_t < 800) {
                                // 3. 計算目標 DDR 索引 (注意：垂直解析度下每一行是 480 像素)
                                target_idx = (y_t * 480 + x_t) * 3;

                                // 4. 寫入像素數據[cite: 8]
                                frame[target_idx] = Pic[src_idx];         // R
                                frame[target_idx + 1] = Pic[src_idx + 1]; // G
                                frame[target_idx + 2] = Pic[src_idx + 2]; // B
                        }
                }
        }

        // 刷新 Cache，確保 VDMA 能看到最新數據[cite: 8]
        Xil_DCacheFlush();
}

int main() {
        XAxiVdma Vdma_Inst;
        XGpioPs Gpio;

        emio_init(&Gpio);
        reset_lcd(&Gpio);
        lcd_init(&Gpio);
        uint32_t period_clocks = 50000;
        bl_on(period_clocks);

        memset((uint8_t *)srcBuffer, 0x00, 480 * 800 * 3);
        Pic_Display_Vertical((uint8_t *)srcBuffer, gImage_pic);
        run_triple_frame_buffer(&Vdma_Inst, 0, 480, 800, srcBuffer, 0, 0);
        uint8_t key_val;
        uint32_t current_duty = period_clocks;

        while (1) {
                key_val = key_scan(&Gpio);
                switch (key_val) {
                case KEY0:
                        led_blink(&Gpio, LED0);
                        current_duty = current_duty / 2;
                        if (current_duty < 100)
                                current_duty = 100;
                        PWM_Set_Duty(PWM_BASEADDR, current_duty, 0);
                        xil_printf("Current Duty: %d%%\r\n", (current_duty * 100) / period_clocks);
                        break;
                case KEY1:
                        led_blink(&Gpio, LED1);
                        current_duty = current_duty * 2;
                        PWM_Set_Duty(PWM_BASEADDR, current_duty, 0);
                        xil_printf("Current Duty: %d%%\r\n", (current_duty * 100) / period_clocks);
                        break;
                case KEY2:
                        led_blink(&Gpio, LED2);
                        fill_color_buffer(srcBuffer, RGB888_RED);
                        break;
                case KEY3:
                        led_blink(&Gpio, LED3);
                        fill_color_buffer(srcBuffer, RGB888_BLUE);
                        break;
                default:
                        break;
                }
                usleep(1000);
        };
        return 0;
}
