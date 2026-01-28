#include "ssd1306.h"
#include "fonts.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

/* Screen buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/* Cursor position */
static uint8_t SSD1306_CurrentX = 0;
static uint8_t SSD1306_CurrentY = 0;

/* Write command */
static void SSD1306_WriteCommand(uint8_t cmd)
{
    uint8_t data[2];
    data[0] = 0x00;
    data[1] = cmd;
    HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, data, 2, HAL_MAX_DELAY);
}

/* Initialize OLED */
void ssd1306_Init(void)
{
    HAL_Delay(100);

    SSD1306_WriteCommand(0xAE); // Display OFF
    SSD1306_WriteCommand(0x20); // Memory addressing mode
    SSD1306_WriteCommand(0x00); // Horizontal addressing
    SSD1306_WriteCommand(0xB0); // Page start
    SSD1306_WriteCommand(0xC8); // COM scan direction
    SSD1306_WriteCommand(0x00); // Low column
    SSD1306_WriteCommand(0x10); // High column
    SSD1306_WriteCommand(0x40); // Start line
    SSD1306_WriteCommand(0x81); // Contrast
    SSD1306_WriteCommand(0x7F);
    SSD1306_WriteCommand(0xA1); // Segment remap
    SSD1306_WriteCommand(0xA6); // Normal display
    SSD1306_WriteCommand(0xA8); // Multiplex
    SSD1306_WriteCommand(0x1F); // 32 rows
    SSD1306_WriteCommand(0xA4); // Display RAM
    SSD1306_WriteCommand(0xD3); // Offset
    SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0xD5); // Clock
    SSD1306_WriteCommand(0x80);
    SSD1306_WriteCommand(0xD9); // Pre-charge
    SSD1306_WriteCommand(0xF1);
    SSD1306_WriteCommand(0xDA); // COM pins
    SSD1306_WriteCommand(0x02);
    SSD1306_WriteCommand(0xDB); // VCOMH
    SSD1306_WriteCommand(0x20);
    SSD1306_WriteCommand(0x8D); // Charge pump
    SSD1306_WriteCommand(0x14);
    SSD1306_WriteCommand(0xAF); // Display ON

    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
}

/* Fill screen */
void ssd1306_Fill(SSD1306_COLOR color)
{
    memset(SSD1306_Buffer,
           (color == Black) ? 0x00 : 0xFF,
           sizeof(SSD1306_Buffer));
}

/* Update screen */
void ssd1306_UpdateScreen(void)
{
    for (uint8_t page = 0; page < 4; page++)
    {
        SSD1306_WriteCommand(0xB0 + page);
        SSD1306_WriteCommand(0x00);
        SSD1306_WriteCommand(0x10);

        uint8_t data[SSD1306_WIDTH + 1];
        data[0] = 0x40;

        memcpy(&data[1],
               &SSD1306_Buffer[SSD1306_WIDTH * page],
               SSD1306_WIDTH);

        HAL_I2C_Master_Transmit(&hi2c1,
                                SSD1306_I2C_ADDR,
                                data,
                                SSD1306_WIDTH + 1,
                                HAL_MAX_DELAY);
    }
}

/* Set cursor */
void ssd1306_SetCursor(uint8_t x, uint8_t y)
{
    SSD1306_CurrentX = x;
    SSD1306_CurrentY = y;
}

/* Write character */
void ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
    uint32_t i, b, j;

    if ((SSD1306_WIDTH  <= (SSD1306_CurrentX + Font.Width)) ||
        (SSD1306_HEIGHT <= (SSD1306_CurrentY + Font.Height)))
        return;

    for (i = 0; i < Font.Height; i++)
    {
        b = Font.data[(ch - 32) * Font.Height + i];

        for (j = 0; j < Font.Width; j++)
        {
            if ((b << j) & 0x8000)
            {
                SSD1306_Buffer[
                    SSD1306_CurrentX + j +
                    (SSD1306_CurrentY + i) / 8 * SSD1306_WIDTH] |=
                    1 << ((SSD1306_CurrentY + i) % 8);
            }
            else if (color == Black)
            {
                SSD1306_Buffer[
                    SSD1306_CurrentX + j +
                    (SSD1306_CurrentY + i) / 8 * SSD1306_WIDTH] &=
                    ~(1 << ((SSD1306_CurrentY + i) % 8));
            }
        }
    }

    SSD1306_CurrentX += Font.Width;
}

/* Write string */
void ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color)
{
    while (*str)
    {
        ssd1306_WriteChar(*str, Font, color);
        str++;
    }
}
