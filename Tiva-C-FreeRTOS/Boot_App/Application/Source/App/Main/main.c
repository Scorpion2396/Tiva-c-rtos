#include<stdio.h>
#include "TM4C123GH6PM.h"
#include "GPIO_Driver_Interface.h"
#include "Timer_Interface.h"
#include "ADC_Driver_Interface.h"
#include "Common_Datatype.h"
#include "Common_Macros.h"
//#include "LCD_Interface.h"
#include "I2C_Interface.h"
//#include "SPI_Interface.h"
//#include "MCP3208_Interface.h"
//#include "CAN_Interface.h"
#include "UART_Interface.h"
#include "timercnter_interface.h"
#include "DS3231_Interface.h"
#include "SSD1306_Interface.h"
#include "bsp.h"

/******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"


/******************************************************************************/
void toggleLed_PF1()
{

    while(1)
    {
        ssd1306_clear_row(2);
        ssd1306_setcursor(2,0);
        ssd1306_Print_String("Running PF1");


        DigitalWrite(PF1,HIGH);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        DigitalWrite(PF1,LOW);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

/******************************************************************************/
void toggleLed_PF2()
{

    while(1)
    {
        ssd1306_clear_row(2);
        ssd1306_setcursor(2,0);
        ssd1306_Print_String("Running PF2");


        DigitalWrite(PF2,HIGH);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        DigitalWrite(PF2,LOW);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


/******************************************************************************/
void main()
{
    DigitalWrite(PB0,LOW );
    DigitalWrite(PB1, HIGH);
    Delay_ms(100);

    I2C_Init(I2C_2, 500);
    ssd1306_init();

    ssd1306_clear_row(7);
    ssd1306_setcursor(7,0);
    ssd1306_Print_String("Running Apps");



    xTaskCreate(toggleLed_PF1,
                "Toggle_LED_PF1",
                512,
                NULL,
                1,
                NULL);

    xTaskCreate(toggleLed_PF2,
                "Toggle_LED_PF2",
                512,
                NULL,
                0,
                NULL);


    ssd1306_clear_row(1);
    ssd1306_setcursor(1,0);
    ssd1306_Print_String("Task Created");


    vTaskStartScheduler();

    while(1)
    {
        ;
    }
}




