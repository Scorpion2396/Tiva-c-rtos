/*
 * AppsDownloader.c
 *
 *  Created on: 16-May-2023
 *      Author: saksh
 */

/******************************************************************************/
#include "main.h"
#include <stdio.h>
/******************************************************************************/
#define MAX_APPS_PACKET_SIZE         1024  // no. of bytes
/******************************************************************************/

uint8_t downloadNflashApps()
{
    uint8_t temp1[4]   = {0};
    uint32_t rx_app_size , rx_cnt , apps_packet_size;
    uint8_t str1[64];
    uint8_t err = 1;
    uint32_t app_bin_size           = 0;
    uint8_t temp[MAX_APPS_PACKET_SIZE]  = {0};  //
    uint32_t no_of_blocks           = 0 ;
    uint32_t no_of_bytes_rem        = 0 ;
    uint32_t block_cnt              = 0;
    uint32_t long_cnt               = 0;
    uint32_t long_data_cnt          = 0;
    uint32_t Apps_start_Addr        = APPS_FLASH_START_ADDR;
    uint8_t rx_data                 = 0;


    if((DigitalRead(PF0, PU) == 1))
        return 0;

    ssd1306_clear_refresh();
    ssd1306_setcursor(7,0);
    ssd1306_Print_String("Flash Apps Mode");

    rx_data = UART_Receive();
    if(rx_data == 0xAA) // app flasher text byte to check if receiver is ready
        UART_Transmit(0x55);
    else
    {
        return 0;
    }

// receiving apps bin size in bytes as first 4 bytes
    for(rx_cnt = 0 ; rx_cnt < 4 ; rx_cnt++)
    {
        temp1[rx_cnt] = UART_Receive();
    }

    rx_app_size  = (uint32_t)((temp1[3]<<24) | (temp1[2]<<16) | (temp1[1]<<8) | (temp1[0]<<0));

    sprintf(str1,"AppSize=%ld\r\n", rx_app_size);
    ssd1306_setcursor(0,0);
    ssd1306_Print_String(str1);

//ack-back
    UART_Transmit(0x55);


// receiving apps packet size in bytes as second 4 bytes
    for(rx_cnt = 0 ; rx_cnt < 4 ; rx_cnt++)
    {
        temp1[rx_cnt] = UART_Receive();
    }

    apps_packet_size  = (uint32_t)((temp1[3]<<24) | (temp1[2]<<16) | (temp1[1]<<8) | (temp1[0]<<0));

    app_bin_size            = rx_app_size;
    no_of_blocks            = app_bin_size / apps_packet_size ;
    no_of_bytes_rem         = app_bin_size % apps_packet_size ;

//ack-back
    if(apps_packet_size <= MAX_APPS_PACKET_SIZE)
    {
        err = FlashErase(Apps_start_Addr, (rx_app_size/4));
        if(err == 0)
        {
            return err;
        }

        UART_Transmit(0x55);
    }


    for(block_cnt = 0 ; block_cnt < no_of_blocks ; block_cnt++)
    {
        for(long_data_cnt = 0 ; long_data_cnt < (apps_packet_size) ; long_data_cnt++)
        {
            temp[long_data_cnt] = UART_Receive();
        }

        err = FlashProgram_ByteArr(Apps_start_Addr, temp, apps_packet_size);
        if(err == 0)
            return err;

        Apps_start_Addr += apps_packet_size;


//        if(block_cnt == 43)
//        {
//            ssd1306_setcursor(2,0);
//            ssd1306_Print_String("Complete");
//        }

        sprintf(str1,"block received =%ld\r\n", block_cnt+1);
        ssd1306_setcursor(1,0);
        ssd1306_Print_String(str1);

        for(long_data_cnt = 0 ; long_data_cnt < (apps_packet_size) ; long_data_cnt++)
        {
            temp[long_data_cnt] = 0;
        }

        UART_Transmit(0x55);

    }



    for(long_cnt = 0 ; long_cnt < no_of_bytes_rem ; long_cnt++)
    {
        temp[long_cnt] = UART_Receive();
    }

    ssd1306_setcursor(2,0);
    ssd1306_Print_String("Complete!!!");

    err = FlashProgram_ByteArr(Apps_start_Addr, temp, no_of_bytes_rem);
    if(err == 0)
        return err;


    return 1;

}



