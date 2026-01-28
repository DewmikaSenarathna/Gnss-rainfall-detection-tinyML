#include "dht22.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim1;


static GPIO_TypeDef *DHT_PORT;
static uint16_t DHT_PIN;



static void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

void DHT22_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    DHT_PORT = GPIOx;
    DHT_PIN  = GPIO_Pin;
}

static void DHT_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);
}

static void DHT_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);
}

uint8_t DHT22_Read(float *temperature, float *humidity)
{
    uint8_t data[5] = {0};

    DHT_Output();
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_SET);
    delay_us(30);
    DHT_Input();

    delay_us(40);
    if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) return 0;
    delay_us(80);
    if (!HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) return 0;
    delay_us(40);

    for (int i = 0; i < 40; i++)
    {
        while (!HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN));
        delay_us(40);
        if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))
            data[i/8] |= (1 << (7 - (i % 8)));
        while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN));
    }

    if ((data[0]+data[1]+data[2]+data[3]) != data[4]) return 0;

    *humidity = ((data[0]<<8) | data[1]) / 10.0f;
    *temperature = ((data[2]<<8) | data[3]) / 10.0f;

    return 1;
}
