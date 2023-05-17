//
// Created by taxue on 2023/1/21.
//

#ifndef RGB_PANEL_Vernon_ST7701S_H
#define RGB_PANEL_Vernon_ST7701S_H

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/task.h"

#define SPI_METHOD 1
#define IOEXPANDER_METHOD 0

//类结构体
typedef struct{
    char method_select;
    
    //SPI config_t
    spi_device_handle_t spi_device;
    spi_bus_config_t spi_io_config_t;
    spi_device_interface_config_t st7701s_protocol_config_t;

    //I2C config_t
}Vernon_ST7701S;

typedef Vernon_ST7701S * Vernon_ST7701S_handle;


/*Public Function*/

//创建新的对象
Vernon_ST7701S_handle ST7701S_newObject(int SDA, int SCL, int CS, char channel_select, char method_select);

//屏幕初始化
void ST7701S_screen_init(Vernon_ST7701S_handle VernonSt7701S_handle, unsigned char type);

//删除对象
void ST7701S_delObject(Vernon_ST7701S_handle VernonSt7701S_handle);


/*Private Function*/

//SPI写指令
void ST7701S_WriteCommand(Vernon_ST7701S_handle VernonSt7701S_handle, uint8_t cmd);

//SPI写地址
void ST7701S_WriteData(Vernon_ST7701S_handle VernonSt7701S_handle, uint8_t data);

#endif //RGB_PANEL_Vernon_ST7701S_H
