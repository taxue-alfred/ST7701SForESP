//
// Created by taxue on 2023/1/21.
//

#ifndef RGB_PANEL_ALFRED_ST7701S_H
#define RGB_PANEL_ALFRED_ST7701S_H

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/task.h"

//类结构体
typedef struct{
    spi_device_handle_t spi_device;
    spi_bus_config_t spi_io_config_t;
    spi_device_interface_config_t st7701s_protocol_config_t;
}Alfred_ST7701S;

//三线SPI初始化，9Bit
void ST7701S_spi_init(Alfred_ST7701S * alfredSt7701S, int SDA, int SCL, int CS, char spi_select);

//屏幕初始化
void ST7701S_screen_init(Alfred_ST7701S *alfredSt7701S, unsigned char type);

//SPI写指令
void ST7701S_WriteCommand(Alfred_ST7701S * alfredSt7701S, uint8_t cmd);

//SPI写地址
void ST7701S_WriteData(Alfred_ST7701S * alfredSt7701S, uint8_t data);

#endif //RGB_PANEL_ALFRED_ST7701S_H
