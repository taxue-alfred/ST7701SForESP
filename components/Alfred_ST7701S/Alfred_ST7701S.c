//
// Created by taxue on 2023/1/21.
//

#include "Alfred_ST7701S.h"

#define SPI_WriteComm(cmd) ST7701S_WriteCommand(alfredSt7701S, cmd)
#define SPI_WriteData(data) ST7701S_WriteData(alfredSt7701S, data)
#define Delay(ms) vTaskDelay(ms / portTICK_PERIOD_MS)

/**
 * @brief 三线SPI初始化
 * @param alfredSt7701S 类实例指针
 * @param SDA SDA引脚
 * @param SCL SCL引脚
 * @param CS  CS引脚
 * @param spi_select SPI控制器, SPI3_HOST OR SPI4_HOST in ESP32S3
*/
void ST7701S_spi_init(Alfred_ST7701S *alfredSt7701S, int SDA, int SCL, int CS, char spi_select)
{
    alfredSt7701S->spi_io_config_t.miso_io_num = -1;
    alfredSt7701S->spi_io_config_t.mosi_io_num = SDA;
    alfredSt7701S->spi_io_config_t.sclk_io_num = SCL;
    alfredSt7701S->spi_io_config_t.quadwp_io_num = -1;
    alfredSt7701S->spi_io_config_t.quadhd_io_num = -1;
    // 默认值，启用DMA应设置为0
    alfredSt7701S->spi_io_config_t.max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE;
    // 不使用DMA最后赋值0
    ESP_ERROR_CHECK(spi_bus_initialize(spi_select, &alfredSt7701S->spi_io_config_t,
                                       0));

    alfredSt7701S->st7701s_protocol_config_t.command_bits = 1;
    alfredSt7701S->st7701s_protocol_config_t.address_bits = 8;
    alfredSt7701S->st7701s_protocol_config_t.clock_speed_hz = 40000000;
    /**
     * < 时钟极性以及时钟相位设置(CPOL, CPHA):
         - 0: (0, 0)
         - 1: (0, 1)
         - 2: (1, 0)
         - 3: (1, 1)
         **/
    alfredSt7701S->st7701s_protocol_config_t.mode = 0;
    alfredSt7701S->st7701s_protocol_config_t.spics_io_num = CS;
    alfredSt7701S->st7701s_protocol_config_t.queue_size = 1;
    ESP_ERROR_CHECK(spi_bus_add_device(spi_select, &alfredSt7701S->st7701s_protocol_config_t,
                                       &alfredSt7701S->spi_device));
}

/**
 * @brief 屏幕初始化
 * @param alfredSt7701S 类实例指针
 * @param type 选择初始化类型 [1-x]
 * @note 以下类型来自不同地方，对应命令功能未知
*/
void ST7701S_screen_init(Alfred_ST7701S *alfredSt7701S, unsigned char type)
{
    if (type == 1){
        //从商家提供的资料复制

        SPI_WriteComm(0x11); //sleep out í?3??ˉ???￡ê?
        Delay(10);
    //--------------------------Bank0 Setting-----------------------------------//
    //----------------------Display Control setting-----------------------------//
    //--------------------------Bank1 Setting-----------------------------------//
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x10);

        SPI_WriteComm(0xC0);
        SPI_WriteData(0x3B);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xC1);
        SPI_WriteData(0x0D);
        SPI_WriteData(0x02);

        SPI_WriteComm(0xC2);
        SPI_WriteData(0x31);
        SPI_WriteData(0x05);
    //--------------------------Gamma Cluster Setting--------------------------//
        SPI_WriteComm(0xB0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x18);
        SPI_WriteData(0x0E);
        SPI_WriteData(0x11);
        SPI_WriteData(0x06);
        SPI_WriteData(0x07);
        SPI_WriteData(0x08);
        SPI_WriteData(0x07);
        SPI_WriteData(0x22);
        SPI_WriteData(0x04);
        SPI_WriteData(0x12);
        SPI_WriteData(0x0F);
        SPI_WriteData(0xAA);
        SPI_WriteData(0x31);
        SPI_WriteData(0x18);

        SPI_WriteComm(0xB1);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x19);
        SPI_WriteData(0x0E);
        SPI_WriteData(0x12);
        SPI_WriteData(0x07);
        SPI_WriteData(0x08);
        SPI_WriteData(0x08);
        SPI_WriteData(0x08);
        SPI_WriteData(0x22);
        SPI_WriteData(0x04);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);
        SPI_WriteData(0xA9);
        SPI_WriteData(0x32);
        SPI_WriteData(0x18);

    //-------------------------Bank1 Setting---------------------------------//
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);

        SPI_WriteComm(0xb0);
        SPI_WriteData(0x60);
    //--------------------------Vcom Setting--------------------------------//
        SPI_WriteComm(0xb1);
        SPI_WriteData(0x32);
    //-----------------------End Vcom Setting------------------------------//
        SPI_WriteComm(0xb2);
        SPI_WriteData(0x07);

        SPI_WriteComm(0xb3);
        SPI_WriteData(0x80);

        SPI_WriteComm(0xb5);
        SPI_WriteData(0x49);

        SPI_WriteComm(0xb7);
        SPI_WriteData(0x85);

        SPI_WriteComm(0xb8);
        SPI_WriteData(0x21);

        SPI_WriteComm(0xC1);
        SPI_WriteData(0x78);

        SPI_WriteComm(0xC2);
        SPI_WriteData(0x78);

        // SPI_WriteComm(0xD0);
        // SPI_WriteData(0x88);
    //-----------End Power Control Registers Initial ------------------//
        // DelayNms(100);
    //----------------GIP Setting--------------------------------------//
        SPI_WriteComm(0xE0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x1B);
        SPI_WriteData(0x02);

        SPI_WriteComm(0xE1);
        SPI_WriteData(0x08);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x07);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);

        SPI_WriteComm(0xE2);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);
        SPI_WriteData(0xED);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0xEC);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xE3);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);

        SPI_WriteComm(0xE4);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);

        SPI_WriteComm(0xE5);
        SPI_WriteData(0x0A);
        SPI_WriteData(0xE9);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0C);
        SPI_WriteData(0xEB);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0E);
        SPI_WriteData(0xED);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x10);
        SPI_WriteData(0xEF);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);

        SPI_WriteComm(0xE6);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);

        SPI_WriteComm(0xE7);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);

        SPI_WriteComm(0xE8);
        SPI_WriteData(0x09);
        SPI_WriteData(0xE8);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0B);
        SPI_WriteData(0xEA);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0D);
        SPI_WriteData(0xEC);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0F);
        SPI_WriteData(0xEE);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);

        SPI_WriteComm(0xEB);
        SPI_WriteData(0x02);
        SPI_WriteData(0x00);
        SPI_WriteData(0xE4);
        SPI_WriteData(0xE4);
        SPI_WriteData(0x88);
        SPI_WriteData(0x00);
        SPI_WriteData(0x40);

        SPI_WriteComm(0xEC);
        SPI_WriteData(0x3c);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xED); //ok
        SPI_WriteData(0xAB);
        SPI_WriteData(0x89);
        SPI_WriteData(0x76);
        SPI_WriteData(0x54);
        SPI_WriteData(0x02);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0x20);
        SPI_WriteData(0x45);
        SPI_WriteData(0x67);
        SPI_WriteData(0x98);
        SPI_WriteData(0xBA);

        SPI_WriteComm(0x35); SPI_WriteData(0x00);    // Sleep-Out

        SPI_WriteComm(0x36); SPI_WriteData(0x00);//BGR
        // SPI_WriteComm(0x36); SPI_WriteData(0x08); //RGB

        // SPI_WriteComm(0x3A); SPI_WriteData(0x77); //24bit
        // SPI_WriteComm(0x20);//display inversion off
        SPI_WriteComm(0x3A); SPI_WriteData(0x55);//16bit
        // SPI_WriteComm(0x3A); SPI_WriteData(0x66);//18bit
        // SPI_WriteComm(0x22);//All pixel off
        
        // SPI_WriteCmd(0x21);    //Normal Black
        // SPI_WriteComm(0x11);//sleep out í?3??ˉ???￡ê?

    //--------------------------Bank3 Setting--------------------------------//
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x13);

        SPI_WriteComm(0xE5);
        SPI_WriteData(0xE4);

        // Delay(10);
        SPI_WriteComm(0x29);

    }else if(type == 2){
        //从Arduino GFX库中移植

        SPI_WriteComm(0x01); //reset command

        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x10);

        SPI_WriteComm(0xC0);
        SPI_WriteData(0x3B);
        SPI_WriteData(0x00);
        
        SPI_WriteComm(0xC1);
        SPI_WriteData(0x0D);
        SPI_WriteData(0x02);

        SPI_WriteComm(0xC2);
        SPI_WriteData(0x31);
        SPI_WriteData(0x05);

        SPI_WriteComm(0xCD);
        SPI_WriteData(0x08);

        SPI_WriteComm(0xB0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x18);
        SPI_WriteData(0x0E);
        SPI_WriteData(0x11);
        SPI_WriteData(0x06);
        SPI_WriteData(0x07);
        SPI_WriteData(0x08);
        SPI_WriteData(0x07);
        SPI_WriteData(0x22);
        SPI_WriteData(0x04);
        SPI_WriteData(0x12);
        SPI_WriteData(0x0F);
        SPI_WriteData(0xAA);
        SPI_WriteData(0x31);
        SPI_WriteData(0x18);

        SPI_WriteComm(0xB1);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x19);
        SPI_WriteData(0x0E);
        SPI_WriteData(0x12);
        SPI_WriteData(0x07);
        SPI_WriteData(0x08);
        SPI_WriteData(0x08);
        SPI_WriteData(0x08);
        SPI_WriteData(0x22);
        SPI_WriteData(0x04);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);
        SPI_WriteData(0xA9);
        SPI_WriteData(0x32);
        SPI_WriteData(0x18);

        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);

        SPI_WriteComm(0xB0);
        SPI_WriteData(0x60);

        SPI_WriteComm(0xB1);
        SPI_WriteData(0x32);

        SPI_WriteComm(0xB2);
        SPI_WriteData(0x07);

        SPI_WriteComm(0xB3);
        SPI_WriteData(0x80);

        SPI_WriteComm(0xB5);
        SPI_WriteData(0x49);

        SPI_WriteComm(0xB7);
        SPI_WriteData(0x85);

        SPI_WriteComm(0xB8);
        SPI_WriteData(0x21);

        SPI_WriteComm(0xC1);
        SPI_WriteData(0x78);

        SPI_WriteComm(0xC2);
        SPI_WriteData(0x78);

        SPI_WriteComm(0xE0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x1B);
        SPI_WriteData(0x02);
        
        SPI_WriteComm(0xE1);
        SPI_WriteData(0x08);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x07);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);

        SPI_WriteComm(0xE2);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);
        SPI_WriteData(0xED);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0xEC);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xE3);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);

        SPI_WriteComm(0xE4);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);
        
        SPI_WriteComm(0xE5);
        SPI_WriteData(0x0A);
        SPI_WriteData(0xE9);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0C);
        SPI_WriteData(0xEB);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0E);
        SPI_WriteData(0xED);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x10);
        SPI_WriteData(0xEF);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);

        SPI_WriteComm(0xE6);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);

        SPI_WriteComm(0xE7);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);

        SPI_WriteComm(0xE8);
        SPI_WriteData(0x09);
        SPI_WriteData(0xE8);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0B);
        SPI_WriteData(0xEA);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0D);
        SPI_WriteData(0xEC);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0F);
        SPI_WriteData(0xEE);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);

        SPI_WriteComm(0xEB);
        SPI_WriteData(0x02);
        SPI_WriteData(0x00);
        SPI_WriteData(0xE4);
        SPI_WriteData(0xE4);
        SPI_WriteData(0x88);
        SPI_WriteData(0x00);
        SPI_WriteData(0x40);
        
        SPI_WriteComm(0xEC);
        SPI_WriteData(0x3C);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xED);
        SPI_WriteData(0xAB);
        SPI_WriteData(0x89);
        SPI_WriteData(0x76);
        SPI_WriteData(0x54);
        SPI_WriteData(0x02);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0x20);
        SPI_WriteData(0x45);
        SPI_WriteData(0x67);
        SPI_WriteData(0x98);
        SPI_WriteData(0xBA);

        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x13);

        SPI_WriteComm(0xE5);
        SPI_WriteData(0xE4);

        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);

        SPI_WriteComm(0x3A);
        SPI_WriteData(0x60);

        SPI_WriteComm(0x11);

        Delay(120);

        SPI_WriteComm(0x29);

        //after batchOperation
        SPI_WriteComm(0x21);
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x10);
        SPI_WriteComm(0xC7);
        SPI_WriteData(0x00);
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteComm(0x36);
        SPI_WriteData(0x00); //BGR
        // SPI_WriteData(0x08); //RGB

    }else if(type == 3){
        //网络查询复制, https://www.bilibili.com/video/BV1hB4y197YN/

        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x13);
        SPI_WriteComm(0xEF);
        SPI_WriteData(0x08);
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x10);
        SPI_WriteComm(0xC0);
        SPI_WriteData(0x3B);
        SPI_WriteData(0x00);
        SPI_WriteComm(0xC1);
        SPI_WriteData(0x0D);
        SPI_WriteData(0x02);
        SPI_WriteComm(0xC2);
        SPI_WriteData(0x21);
        SPI_WriteData(0x08);

        SPI_WriteComm(0xCD);
        SPI_WriteData(0x08);//18-bit/pixel: MDT=0:D[21:16]=R,D[13:8]=G,D[5:0]=B(CDH=00) ;
                            //              MDT=1:D[17:12]=R,D[11:6]=G,D[5:0]=B(CDH=08) ;

        SPI_WriteComm(0xB0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x18);
        SPI_WriteData(0x0E);
        SPI_WriteData(0x11);
        SPI_WriteData(0x06);
        SPI_WriteData(0x07);
        SPI_WriteData(0x08);
        SPI_WriteData(0x07);
        SPI_WriteData(0x22);
        SPI_WriteData(0x04);
        SPI_WriteData(0x12);
        SPI_WriteData(0x0F);
        SPI_WriteData(0xAA);
        SPI_WriteData(0x31);
        SPI_WriteData(0x18);
        SPI_WriteComm(0xB1);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x19);
        SPI_WriteData(0x0E);
        SPI_WriteData(0x12);
        SPI_WriteData(0x07);
        SPI_WriteData(0x08);
        SPI_WriteData(0x08);
        SPI_WriteData(0x08);
        SPI_WriteData(0x22);
        SPI_WriteData(0x04);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);
        SPI_WriteData(0xA9);
        SPI_WriteData(0x32);
        SPI_WriteData(0x18);
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteComm(0xB0);
        SPI_WriteData(0x60);
        SPI_WriteComm(0xB1);
        SPI_WriteData(0x30);
        SPI_WriteComm(0xB2);
        SPI_WriteData(0x87);
        SPI_WriteComm(0xB3);
        SPI_WriteData(0x80);
        SPI_WriteComm(0xB5);
        SPI_WriteData(0x49);
        SPI_WriteComm(0xB7);
        SPI_WriteData(0x85);
        SPI_WriteComm(0xB8);
        SPI_WriteData(0x21);
        SPI_WriteComm(0xC1);
        SPI_WriteData(0x78);
        SPI_WriteComm(0xC2);
        SPI_WriteData(0x78);
        Delay(2);
        SPI_WriteComm(0xE0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x1B);
        SPI_WriteData(0x02);
        SPI_WriteComm(0xE1);
        SPI_WriteData(0x08);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x07);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);
        SPI_WriteComm(0xE2);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);
        SPI_WriteData(0xED);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0xEC);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteComm(0xE3);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);
        SPI_WriteComm(0xE4);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);
        SPI_WriteComm(0xE5);
        SPI_WriteData(0x0A);
        SPI_WriteData(0xE9);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0C);
        SPI_WriteData(0xEB);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0E);
        SPI_WriteData(0xED);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x10);
        SPI_WriteData(0xEF);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteComm(0xE6);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x11);
        SPI_WriteComm(0xE7);
        SPI_WriteData(0x44);
        SPI_WriteData(0x44);
        SPI_WriteComm(0xE8);
        SPI_WriteData(0x09);
        SPI_WriteData(0xE8);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0B);
        SPI_WriteData(0xEA);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0D);
        SPI_WriteData(0xEC);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x0F);
        SPI_WriteData(0xEE);
        SPI_WriteData(0xD8);
        SPI_WriteData(0xA0);
        SPI_WriteComm(0xEB);
        SPI_WriteData(0x02);
        SPI_WriteData(0x00);
        SPI_WriteData(0xE4);
        SPI_WriteData(0xE4);
        SPI_WriteData(0x88);
        SPI_WriteData(0x00);
        SPI_WriteData(0x40);
        SPI_WriteComm(0xEC);
        SPI_WriteData(0x3C);
        SPI_WriteData(0x00);
        SPI_WriteComm(0xED);
        SPI_WriteData(0xAB);
        SPI_WriteData(0x89);
        SPI_WriteData(0x76);
        SPI_WriteData(0x54);
        SPI_WriteData(0x02);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0xFF);
        SPI_WriteData(0x20);
        SPI_WriteData(0x45);
        SPI_WriteData(0x67);
        SPI_WriteData(0x98);
        SPI_WriteData(0xBA);
        SPI_WriteComm(0xEF);
        SPI_WriteData(0x10);
        SPI_WriteData(0x0D);
        SPI_WriteData(0x04);
        SPI_WriteData(0x08);
        SPI_WriteData(0x3F);
        SPI_WriteData(0x1F);
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);

        SPI_WriteComm(0x3A);
        SPI_WriteData(0x50);    //55/50=16bit(RGB565);66=18bit(RGB666);77或默认不写3AH是=24bit(RGB888)

        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x13);
        SPI_WriteComm(0xE8);
        SPI_WriteData(0x00);
        SPI_WriteData(0x0E);
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteComm(0x11);
        Delay(12);
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x13);
        SPI_WriteComm(0xE8);
        SPI_WriteData(0x00);
        SPI_WriteData(0x0C);
        Delay(1);
        SPI_WriteComm(0xE8);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);

        SPI_WriteComm(0x36);
        SPI_WriteData(0x00);

        SPI_WriteComm(0x20);

        SPI_WriteComm(0x29);
        Delay(2);

    }else if(type == 4){
        //网络查询复制, https://www.bilibili.com/video/BV1hB4y197YN/

        SPI_WriteComm(0x11); 
        Delay(1);
            
        SPI_WriteComm (0xFF);
        SPI_WriteData (0x77);
        SPI_WriteData (0x01);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x10);

        SPI_WriteComm (0xC0);
        SPI_WriteData (0x3B);
        SPI_WriteData (0x00);

        SPI_WriteComm (0xC1);
        SPI_WriteData (0x0D);
        SPI_WriteData (0x02);

        SPI_WriteComm (0xC2);
        SPI_WriteData (0x31);
        SPI_WriteData (0x05);

        SPI_WriteComm (0xB0);
        SPI_WriteData (0x00);
        SPI_WriteData (0x11);
        SPI_WriteData (0x18);
        SPI_WriteData (0x0E);
        SPI_WriteData (0x11);
        SPI_WriteData (0x06);
        SPI_WriteData (0x07);
        SPI_WriteData (0x08);
        SPI_WriteData (0x07);
        SPI_WriteData (0x22);
        SPI_WriteData (0x04);
        SPI_WriteData (0x12);
        SPI_WriteData (0x0F);
        SPI_WriteData (0xAA);
        SPI_WriteData (0x31);
        SPI_WriteData (0x18);

        SPI_WriteComm (0xB1);
        SPI_WriteData (0x00);
        SPI_WriteData (0x11);
        SPI_WriteData (0x19);
        SPI_WriteData (0x0E);
        SPI_WriteData (0x12);
        SPI_WriteData (0x07);
        SPI_WriteData (0x08);
        SPI_WriteData (0x08);
        SPI_WriteData (0x08);
        SPI_WriteData (0x22);
        SPI_WriteData (0x04);
        SPI_WriteData (0x11);
        SPI_WriteData (0x11);
        SPI_WriteData (0xA9);
        SPI_WriteData (0x32);
        SPI_WriteData (0x18);

        SPI_WriteComm (0xFF);
        SPI_WriteData (0x77);
        SPI_WriteData (0x01);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x11);

        SPI_WriteComm (0xB0);
        SPI_WriteData (0x60);

        SPI_WriteComm (0xB1);
        SPI_WriteData (0x30);

        SPI_WriteComm (0xB2);
        SPI_WriteData (0x87);

        SPI_WriteComm (0xB3);
        SPI_WriteData (0x80);

        SPI_WriteComm (0xB5);
        SPI_WriteData (0x49);

        SPI_WriteComm (0xB7);
        SPI_WriteData (0x85);

        SPI_WriteComm (0xB8);
        SPI_WriteData (0x21);

        SPI_WriteComm (0xC1);
        SPI_WriteData (0x78);

        SPI_WriteComm (0xC2);
        SPI_WriteData (0x78);
        Delay(2);
        SPI_WriteComm (0xE0);
        SPI_WriteData (0x00);
        SPI_WriteData (0x1B);
        SPI_WriteData (0x02);

        SPI_WriteComm (0xE1);
        SPI_WriteData (0x08);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x07);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x44);
        SPI_WriteData (0x44);

        SPI_WriteComm (0xE2);
        SPI_WriteData (0x11);
        SPI_WriteData (0x11);
        SPI_WriteData (0x44);
        SPI_WriteData (0x44);
        SPI_WriteData (0xED);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0xEC);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);

        SPI_WriteComm (0xE3);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x11);
        SPI_WriteData (0x11);

        SPI_WriteComm (0xE4);
        SPI_WriteData (0x44);
        SPI_WriteData (0x44);

        SPI_WriteComm (0xE5);
        SPI_WriteData (0x0A);
        SPI_WriteData (0xE9);
        SPI_WriteData (0xD8);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x0C);
        SPI_WriteData (0xEB);
        SPI_WriteData (0xD8);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x0E);
        SPI_WriteData (0xED);
        SPI_WriteData (0xD8);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x10);
        SPI_WriteData (0xEF);
        SPI_WriteData (0xD8);
        SPI_WriteData (0xA0);

        SPI_WriteComm (0xE6);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x11);
        SPI_WriteData (0x11);

        SPI_WriteComm (0xE7);
        SPI_WriteData (0x44);
        SPI_WriteData (0x44);

        SPI_WriteComm (0xE8);
        SPI_WriteData (0x09);
        SPI_WriteData (0xE8);
        SPI_WriteData (0xD8);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x0B);
        SPI_WriteData (0xEA);
        SPI_WriteData (0xD8);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x0D);
        SPI_WriteData (0xEC);
        SPI_WriteData (0xD8);
        SPI_WriteData (0xA0);
        SPI_WriteData (0x0F);
        SPI_WriteData (0xEE);
        SPI_WriteData (0xD8);
        SPI_WriteData (0xA0);

        SPI_WriteComm (0xEB);
        SPI_WriteData (0x02);
        SPI_WriteData (0x00);
        SPI_WriteData (0xE4);
        SPI_WriteData (0xE4);
        SPI_WriteData (0x88);
        SPI_WriteData (0x00);
        SPI_WriteData (0x40);

        SPI_WriteComm (0xEC);
        SPI_WriteData (0x3C);
        SPI_WriteData (0x00);

        SPI_WriteComm (0xED);
        SPI_WriteData (0xAB);
        SPI_WriteData (0x89);
        SPI_WriteData (0x76);
        SPI_WriteData (0x54);
        SPI_WriteData (0x02);
        SPI_WriteData (0xFF);
        SPI_WriteData (0xFF);
        SPI_WriteData (0xFF);
        SPI_WriteData (0xFF);
        SPI_WriteData (0xFF);
        SPI_WriteData (0xFF);
        SPI_WriteData (0x20);
        SPI_WriteData (0x45);
        SPI_WriteData (0x67);
        SPI_WriteData (0x98);
        SPI_WriteData (0xBA);

        SPI_WriteComm (0xFF);
        SPI_WriteData (0x77);
        SPI_WriteData (0x01);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x10);

        // SPI_WriteComm(0xCD);
        // SPI_WriteData(0x0a); 
                
        SPI_WriteComm(0x36); 
        SPI_WriteData(0x00); //RGB

        SPI_WriteComm (0xE5);
        SPI_WriteData (0xE4);

        // SPI_WriteComm(0x3A); SPI_WriteData(0x66); //24bit
        // SPI_WriteComm(0x20);//display inversion off
        SPI_WriteComm(0x3A); SPI_WriteData(0x55);//16bit
        // SPI_WriteComm(0x3A); SPI_WriteData(0x66);//18bit``````````````````````````````````````````````````````````````````````																																																																																																								
        // SPI_WriteComm(0x22);//All pixel off


        SPI_WriteComm (0x2a);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x01);
        SPI_WriteData (0xdf);

        SPI_WriteComm (0x2b);
        SPI_WriteData (0x00);
        SPI_WriteData (0x00);
        SPI_WriteData (0x03);
        SPI_WriteData (0x55);


        SPI_WriteComm (0x29);

    }else if(type == 5){
        // 2.1寸圆屏厂商提供驱动
        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x10);

        SPI_WriteComm(0xC0);
        SPI_WriteData(0x3B);//Scan line	
        SPI_WriteData(0x00);

        SPI_WriteComm(0xC1);
        SPI_WriteData(0x0B);	//VBP
        SPI_WriteData(0x02);

        SPI_WriteComm(0xC2);
        SPI_WriteData(0x07);
        SPI_WriteData(0x02);

        SPI_WriteComm(0xCC);
        SPI_WriteData(0x10);

        //SPI_WriteComm(0xCD);//RGB format
        //SPI_WriteData(0x08);        //用565时屏蔽    666打开

        SPI_WriteComm(0xB0); // IPS   
        SPI_WriteData(0x00); // 255 
        SPI_WriteData(0x11); // 251    
        SPI_WriteData(0x16); // 247  down
        SPI_WriteData(0x0e); // 239    
        SPI_WriteData(0x11); // 231    
        SPI_WriteData(0x06); // 203    
        SPI_WriteData(0x05); // 175 
        SPI_WriteData(0x09); // 147    
        SPI_WriteData(0x08); // 108    
        SPI_WriteData(0x21); // 80  
        SPI_WriteData(0x06); // 52   
        SPI_WriteData(0x13); // 24    
        SPI_WriteData(0x10); // 16    
        SPI_WriteData(0x29); // 8    down
        SPI_WriteData(0x31); // 4    
        SPI_WriteData(0x18); // 0   

        SPI_WriteComm(0xB1);//  IPS	   
        SPI_WriteData(0x00);//  255 
        SPI_WriteData(0x11);//  251
        SPI_WriteData(0x16);//  247   down
        SPI_WriteData(0x0e);//  239
        SPI_WriteData(0x11);//  231
        SPI_WriteData(0x07);//  203    
        SPI_WriteData(0x05);//  175
        SPI_WriteData(0x09);//  147  
        SPI_WriteData(0x09);//  108  
        SPI_WriteData(0x21);//  80 
        SPI_WriteData(0x05);//  52   
        SPI_WriteData(0x13);//  24 
        SPI_WriteData(0x11);//  16 
        SPI_WriteData(0x2a);//  8  down 
        SPI_WriteData(0x31);//  4  
        SPI_WriteData(0x18);//  0  

        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);

        SPI_WriteComm(0xB0);  //VOP  3.5375+ *x 0.0125
        SPI_WriteData(0x6d);  //5D
        
        SPI_WriteComm(0xB1); 	//VCOM amplitude setting  
        SPI_WriteData(0x37);  //
        
        SPI_WriteComm(0xB2); 	//VGH Voltage setting  
        SPI_WriteData(0x81);	//12V

        SPI_WriteComm(0xB3);
        SPI_WriteData(0x80);

        SPI_WriteComm(0xB5); 	//VGL Voltage setting  
        SPI_WriteData(0x43);	//-8.3V

        SPI_WriteComm(0xB7);
        SPI_WriteData(0x85);

        SPI_WriteComm(0xB8);
        SPI_WriteData(0x20);

        SPI_WriteComm(0xC1);
        SPI_WriteData(0x78);

        SPI_WriteComm(0xC2);
        SPI_WriteData(0x78);

        SPI_WriteComm(0xD0);
        SPI_WriteData(0x88);

        SPI_WriteComm(0xE0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x02);

        SPI_WriteComm(0xE1);
        SPI_WriteData(0x03);	
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);
        SPI_WriteData(0x04);	
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);
        SPI_WriteData(0x20);
        SPI_WriteData(0x20);

        SPI_WriteComm(0xE2);
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	  
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);

        SPI_WriteComm(0xE3);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xE4);
        SPI_WriteData(0x22);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xE5);		
        SPI_WriteData(0x05);	
        SPI_WriteData(0xEC);	
        SPI_WriteData(0xA0);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x07);	
        SPI_WriteData(0xEE);	
        SPI_WriteData(0xA0);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xE6);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x11);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xE7);
        SPI_WriteData(0x22);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xE8);		
        SPI_WriteData(0x06);	
        SPI_WriteData(0xED);	
        SPI_WriteData(0xA0);
        SPI_WriteData(0xA0);
        SPI_WriteData(0x08);	
        SPI_WriteData(0xEF);	
        SPI_WriteData(0xA0); 
        SPI_WriteData(0xA0);
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);	
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);

        SPI_WriteComm(0xEB);
        SPI_WriteData(0x00); 	
        SPI_WriteData(0x00);
        SPI_WriteData(0x40);
        SPI_WriteData(0x40);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);  

        SPI_WriteComm(0xED);  
        SPI_WriteData(0xFF); 
        SPI_WriteData(0xFF);  
        SPI_WriteData(0xFF); 	
        SPI_WriteData(0xBA); 		
        SPI_WriteData(0x0A); 	
        SPI_WriteData(0xBF); 	
        SPI_WriteData(0x45); 	
        SPI_WriteData(0xFF); 
        SPI_WriteData(0xFF);  
        SPI_WriteData(0x54); 	
        SPI_WriteData(0xFB); 	
        SPI_WriteData(0xA0); 	
        SPI_WriteData(0xAB); 	
        SPI_WriteData(0xFF); 
        SPI_WriteData(0xFF); 
        SPI_WriteData(0xFF); 

        SPI_WriteComm(0xEF);
        SPI_WriteData(0x10); 
        SPI_WriteData(0x0D); 
        SPI_WriteData(0x04); 
        SPI_WriteData(0x08); 
        SPI_WriteData(0x3F); 
        SPI_WriteData(0x1F);

        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x13);

        SPI_WriteComm(0xEF);
        SPI_WriteData(0x08);

        SPI_WriteComm(0xFF);
        SPI_WriteData(0x77);
        SPI_WriteData(0x01);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);
        SPI_WriteData(0x00);

        SPI_WriteComm(0x11);

        Delay(120);

        SPI_WriteComm(0x29);

        SPI_WriteComm(0x36);
        SPI_WriteData(0x08);

        SPI_WriteComm(0x3A);
        SPI_WriteData(0x70);  //55/50=16bit(RGB565);66=18bit(RGB666);77或默认不写3AH是=24bit(RGB888)
    }
}

/**
 * @brief SPI写指令
 * @param alfredSt7701S 类实例指针
 * @param cmd 指令
*/
void ST7701S_WriteCommand(Alfred_ST7701S *alfredSt7701S, uint8_t cmd)
{
    spi_transaction_t spi_tran = {
        .rxlength = 0,
        .length = 0,
        .cmd = 0,
        .addr = cmd,
    };
    spi_device_transmit(alfredSt7701S->spi_device, &spi_tran);
}

/**
 * @brief SPI写地址
 * @param alfredSt7701S 类实例指针
 * @param cmd 地址
*/
void ST7701S_WriteData(Alfred_ST7701S *alfredSt7701S, uint8_t data)
{
    spi_transaction_t spi_tran = {
        .rxlength = 0,
        .length = 0,
        .cmd = 1,
        .addr = data,
    };
    spi_device_transmit(alfredSt7701S->spi_device, &spi_tran);
}