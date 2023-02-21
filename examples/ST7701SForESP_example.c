#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "Vernon_ST7701S.h"
#include "lvgl.h"
#include "lv_demos.h"

#define LCD_PIXEL_CLOCK_HZ     (18 * 1000 * 1000)
#define LCD_BK_LIGHT_ON_LEVEL  1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL

// SPI PIN Define
#define SPI_SDA 20
#define SPI_SCL 19
#define SPI_CS  4

// RGB Pin Define
// #define PIN_NUM_BK_LIGHT       38
// #define PIN_NUM_HSYNC          16
// #define PIN_NUM_VSYNC          17
// #define PIN_NUM_DE             18
// #define PIN_NUM_PCLK           21
// #define PIN_NUM_DATA0          4    // B0
// #define PIN_NUM_DATA1          5    // B1
// #define PIN_NUM_DATA2          6    // B2
// #define PIN_NUM_DATA3          7    // B3
// #define PIN_NUM_DATA4          15   // B4
// #define PIN_NUM_DATA5          8    // G0
// #define PIN_NUM_DATA6          20   // G1
// #define PIN_NUM_DATA7          3    // G2
// #define PIN_NUM_DATA8          46   // G3
// #define PIN_NUM_DATA9          9    // G4
// #define PIN_NUM_DATA10         10   // G5
// #define PIN_NUM_DATA11         11   // R0
// #define PIN_NUM_DATA12         12   // R1
// #define PIN_NUM_DATA13         13   // R2
// #define PIN_NUM_DATA14         14   // R3
// #define PIN_NUM_DATA15         0    // R4

// RGB Pin Define
#define PIN_NUM_BK_LIGHT       -1
#define PIN_NUM_HSYNC          46
#define PIN_NUM_VSYNC          3
#define PIN_NUM_DE             17
#define PIN_NUM_PCLK           9
#define PIN_NUM_DATA0          10    // B0
#define PIN_NUM_DATA1          11   // B1
#define PIN_NUM_DATA2          12    // B2
#define PIN_NUM_DATA3          13    // B3
#define PIN_NUM_DATA4          14   // B4
#define PIN_NUM_DATA5          21    // G0
#define PIN_NUM_DATA6          47   // G1
#define PIN_NUM_DATA7          48    // G2
#define PIN_NUM_DATA8          45   // G3
#define PIN_NUM_DATA9          38    // G4
#define PIN_NUM_DATA10         39   // G5
#define PIN_NUM_DATA11         40   // R0
#define PIN_NUM_DATA12         41   // R1
#define PIN_NUM_DATA13         42   // R2
#define PIN_NUM_DATA14         2   // R3
#define PIN_NUM_DATA15         1    // R4
    
#define PIN_NUM_DISP_EN        -1

// The pixel number in horizontal and vertical
#define LCD_H_RES              480
#define LCD_V_RES              480

// RGB Libraries Define
#define LCD_NUM_FB              1

// LVGL TICK PERIOD DEFINE
#define LVGL_TICK_PERIOD_MS    2

// we use two semaphores to sync the VSYNC event and the LVGL task, to avoid potential tearing effect
SemaphoreHandle_t sem_vsync_end;
SemaphoreHandle_t sem_gui_ready;

//LVGL display buffer and display driver
static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
static lv_disp_drv_t disp_drv;      // contains callback functions

// 创建ST7701S实例
Vernon_ST7701S VernonSt7701S;


static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) disp_drv->user_data;

    xSemaphoreGive(sem_gui_ready);
    xSemaphoreTake(sem_vsync_end, portMAX_DELAY);

    // pass the draw buffer to the driver
    esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_p);

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

static void increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

static bool on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *event_data, void *user_data)
{
    BaseType_t high_task_awoken = pdFALSE;
    if (xSemaphoreTakeFromISR(sem_gui_ready, &high_task_awoken) == pdTRUE) {
        xSemaphoreGiveFromISR(sem_vsync_end, &high_task_awoken);
    }
    return high_task_awoken == pdTRUE;
}

void lv_port_disp_init(esp_lcd_panel_handle_t panel_handle)
{
    void *buf1 = NULL;
    void *buf2 = NULL;
    // Allocate separate LVGL draw buffers from PSRAM
    buf1 = heap_caps_malloc(LCD_H_RES * 100 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf1);
    buf2 = heap_caps_malloc(LCD_H_RES * 100 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf2);
    // initialize LVGL draw buffers
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, LCD_H_RES * 100);
    
    // Register display driver to LVGL
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_H_RES;
    disp_drv.ver_res = LCD_V_RES;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
}

void app_main(void)
{
    sem_vsync_end = xSemaphoreCreateBinary();
    assert(sem_vsync_end);
    sem_gui_ready = xSemaphoreCreateBinary();
    assert(sem_gui_ready);


    /**RGB面板SPI配置**/
    ST7701S_spi_init(&VernonSt7701S, SPI_SDA, SPI_SCL, SPI_CS, SPI3_HOST);
    ST7701S_screen_init(&VernonSt7701S, 2);


    /**RGB面板初始化**/
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_rgb_panel_config_t panel_config = {
        .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
        .psram_trans_align = 64,
        .num_fbs = LCD_NUM_FB,
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .disp_gpio_num = PIN_NUM_DISP_EN,
        .pclk_gpio_num = PIN_NUM_PCLK,
        .vsync_gpio_num = PIN_NUM_VSYNC,
        .hsync_gpio_num = PIN_NUM_HSYNC,
        .de_gpio_num = PIN_NUM_DE,
        .data_gpio_nums = {
            PIN_NUM_DATA0,
            PIN_NUM_DATA1,
            PIN_NUM_DATA2,
            PIN_NUM_DATA3,
            PIN_NUM_DATA4,
            PIN_NUM_DATA5,
            PIN_NUM_DATA6,
            PIN_NUM_DATA7,
            PIN_NUM_DATA8,
            PIN_NUM_DATA9,
            PIN_NUM_DATA10,
            PIN_NUM_DATA11,
            PIN_NUM_DATA12,
            PIN_NUM_DATA13,
            PIN_NUM_DATA14,
            PIN_NUM_DATA15,
        },
        .timings = {
            .pclk_hz = LCD_PIXEL_CLOCK_HZ,
            .h_res = LCD_H_RES,
            .v_res = LCD_V_RES,
            // The following parameters should refer to LCD spec
            .hsync_back_porch = 50,
            .hsync_front_porch = 10,
            .hsync_pulse_width = 8,
            .vsync_back_porch = 20,
            .vsync_front_porch = 10,
            .vsync_pulse_width = 8,
            .flags.pclk_active_neg = false,
        },
        .flags.fb_in_psram = true, // allocate frame buffer in PSRAM
    };
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
    //注册时间回调函数
    esp_lcd_rgb_panel_event_callbacks_t cbs = {
        .on_vsync = on_vsync_event,
    };
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &cbs, &disp_drv));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

#if PIN_NUM_BK_LIGHT > 0
    /**背光开启**/
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << PIN_NUM_BK_LIGHT
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(PIN_NUM_BK_LIGHT, LCD_BK_LIGHT_ON_LEVEL);
#endif
    

    /**LVGL初始化，此项请在RGB初始化之后**/
    lv_init();
    lv_port_disp_init(panel_handle);
    //Timer初始化
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));


    /**LVGL示例**/
    lv_demo_widgets();


    while (1) {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(10));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        lv_timer_handler();
    }
}
