/******************************************************************************/
#include "main.h"
/******************************************************************************/


void main()
{
    uint8_t AppFlashSts = 0;

    DigitalWrite(PB0,LOW );
    DigitalWrite(PB1, HIGH);
    Delay_ms(100);

    I2C_Init(I2C_2, 500);
    UART_init(UART_0, 115200);
    ssd1306_init();

    ssd1306_clear_row(7);
    ssd1306_setcursor(7,0);
    ssd1306_Print_String("Running Bootloader");
    //Delay_ms(2000);


    AppFlashSts = downloadNflashApps();


    fun_ptr  jump_to_apps = (void(*)(void)) (*((unsigned int*)(APPS_RESET_HANDLER_ADDR)));
    unsigned int apps_stack_ptr = APPS_STACK_PTR_ADDR;

    if(verify_apps())
    {

        SCB->VTOR = APPS_FLASH_START_ADDR;  // change in Interrupt Vector Table address to Apps IntVtrTble
        __set_MSP(apps_stack_ptr);
        jump_to_apps();
    }
    else
    {

        while(1)
            DigitalWrite(PF1,HIGH);
    }
}



/**************************************************************************************/
unsigned char verify_apps(void)
{
    if((*((unsigned int*)(APPS_RESET_HANDLER_ADDR))) != APPS_INVALID)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/**************************************************************************************/
void Delay_ms(unsigned int delay_value)
{
    uint32_t i = 0;
    uint32_t j = 0;
    for(i=0 ; i<delay_value ; i++)
    {
        for(j=0 ; j < 3180 ; j++);
    }
}

