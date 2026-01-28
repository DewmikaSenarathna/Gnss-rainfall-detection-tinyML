#ifndef __SSD1306_H
#define __SSD1306_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

/* OLED I2C address */
#define SSD1306_I2C_ADDR   (0x3C << 1)

/* OLED resolution */
#define SSD1306_WIDTH     128
#define SSD1306_HEIGHT     32   // 0.91" OLED is usually 128x32

/* Color definitions */
typedef enum {
    Black = 0x00,
    White = 0x01
} SSD1306_COLOR;

/* Font structure */
typedef struct {
    const uint8_t Width;
    const uint8_t Height;
    const uint16_t *data;
} FontDef;

/* Public functions */
void ssd1306_Init(void);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen(void);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
void ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);

#endif
