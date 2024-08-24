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
#include "queue.h"
/******************************************************************************/

static const uint8_t msg_queue_len = 5;

static QueueHandle_t msg_queue;


/******************************************************************************/
void queueReceive()
{
    uint32_t item;

    while(1)
    {
        if(xQueueReceive(msg_queue, (void*)&item, 0) == pdTRUE)
        {
            UART_print_int(item);
            UART_print("\n\r");
        }
        else
        {
            UART_print("Queue empty\n\r");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/******************************************************************************/
void queueSend()
{

    while(1)
    {
        static uint32_t num = 0;

        if(xQueueSend(msg_queue, (void*)&num, 10) != pdTRUE)
        {
            UART_print("Queue full\n\r");
        }

        num++;

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
    UART_init(UART_0, 9600);
    ssd1306_init();

    ssd1306_clear_row(7);
    ssd1306_setcursor(7,0);
    ssd1306_Print_String("Running Apps");

    UART_print("Running Apps\n\r");

    msg_queue = xQueueCreate(msg_queue_len, sizeof(int));


    xTaskCreate(queueReceive,
                "queueReceive",
                512,
                NULL,
                1,
                NULL);

    xTaskCreate(queueSend,
                "queueSend",
                512,
                NULL,
                0,
                NULL);


    ssd1306_clear_row(1);
    ssd1306_setcursor(1,0);
    ssd1306_Print_String("Task Created");
    UART_print("Task Created\n\r");


    vTaskStartScheduler();

    while(1)
    {
        ;
    }
}




