#include "ansiMorseCodeDB.h"
#include "main.h"
#include "cmsis_os.h"

#define MORSE_CODE_DOT_LENGTH 500
#define MORSE_CODE_DASH_LENGTH 1500
#define MORSE_CODE_PAUSE_LENGTH 500
#define MORSE_CODE_SPACE_LENGTH 2000

#define DELAY_MS_FUNCTION vTaskDelay

// #define DebugPin_Pin GPIO_PIN_13
// #define DebugPin_GPIO_Port GPIOC
// freeRTOS_WriteMorseCode(DebugPin_GPIO_Port, DebugPin_Pin, "Hello World!", GPIO_PIN_SET); 使用开漏输出的Pin 高电平时关闭 低电平时开启 输出Hello World!的摩斯码文本

void freeRTOS_WriteMorseCode(GPIO_TypeDef* port, uint16_t pin, const char* text, GPIO_PinState closePinState)
{
    HAL_GPIO_WritePin(port, pin, closePinState);
    unsigned int NowPtr = 0;
    while (text[NowPtr] != '\0')
    {
        unsigned char nowChar = text[NowPtr];
        unsigned char length = AnsiMorseCodeDB[nowChar * 2];
        unsigned char code = AnsiMorseCodeDB[nowChar * 2 + 1];
        while (length > 0)
        {
            char isDash = (code & 0x80);
            if (isDash)
            {
                HAL_GPIO_WritePin(port, pin, !closePinState);
                DELAY_MS_FUNCTION(MORSE_CODE_DASH_LENGTH);
                HAL_GPIO_WritePin(port, pin, closePinState);
            }
            else
            {
                HAL_GPIO_WritePin(port, pin, !closePinState);
                DELAY_MS_FUNCTION(MORSE_CODE_DOT_LENGTH);
                HAL_GPIO_WritePin(port, pin, closePinState);
            }
            code <<= 1;
            length--;
            vTaskDelay(MORSE_CODE_PAUSE_LENGTH);
        }
        vTaskDelay(MORSE_CODE_SPACE_LENGTH);
        NowPtr++;
    }

}
