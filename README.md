# ST7701SForESP
ST7701S ESP系列驱动，基于ESP-IDF5.0，ESP32S3编写。

本库只负责SPI的配置，SPI设置屏幕信息两方面。由于RGB库和图形库的配置无法解耦，具体使用的图形库需要自行配置添加，**本示例默认绑定LVGL，使用IDF5示例`rgb_panel`**进行测试

SPI的指令，地址配置信息来源较多，其中有从Arduino_GFX库中移植，在此感谢陈亮大佬。

本库使用面向对象思想编程，支持多设备多实例，但不建议。

Github，Gitee同步更新，Gitee仅作为下载仓库，提交Issue和Pull request请到Github

[Github: https://github.com/taxue-alfred/ST7701SForESP](https://github.com/taxue-alfred/ST7701SForESP)

[Gitee: https://gitee.com/TaXue_TianXing/ST7701SForESP](https://gitee.com/TaXue_TianXing/ST7701SForESP)

## 1. 目录结构

```
├─examples
│      ST7701SForESP_example.c //rgb_panel主函数示例(需要克隆rgb_panel)
│
└─Vernon_ST7701S
    │  Vernon_ST7701S.c
    │
    └─includes
            Vernon_ST7701S.h
```

## 2. 使用方法

### 1. LVGL报错解决

format %lu expects argument of type 'long unsigned int' but argument 3 has type long long unsigned int

> 如果在编译过程中未出现此类报错，可以直接跳过

<img src="https://taxue-alfred-1253400076.cos.ap-beijing.myqcloud.com/image-20230122220934556.png" alt="image-20230122220934556" style="zoom: 33%;" />

此处进行强制类型转换即可，修改之后编译通过即可。

### 2. 嵌入项目

直接将components中的Alfred_ST7701S放入自己的components中，后执行`idf.py reconfigure`重构CMake，主函数中调用函数即可

### 3. menuconfig配置PSRAM

配置如下
<img src="https://taxue-alfred-1253400076.cos.ap-beijing.myqcloud.com/image-20230122223142648.png" alt="image-20230122223142648" style="zoom: 33%;" />

**这一步特别重要，不然会出现屏幕疯狂滚动的现象**

> 这里从CLion环境截图，menuconfig都一样

开启 `Cache fetch instruction from SPI RAM`和`Cache load read only data from SPI RAM`

<img src="https://taxue-alfred-1253400076.cos.ap-beijing.myqcloud.com/image-20230219180440612.png" alt="image-20230219180440612" style="zoom: 67%;" />

### 4. 解决颜色显示不纯的问题

**最明显的就是灰色不是纯灰色，偏黄色，而且字体是“虚”的**

原因是PCLK上跳沿下跳沿与数据时间不匹配问题。将

```c
.timings = {
    .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
    .h_res = EXAMPLE_LCD_H_RES,
    .v_res = EXAMPLE_LCD_V_RES,
    // The following parameters should refer to LCD spec
    .hsync_back_porch = 40,
    .hsync_front_porch = 20,
    .hsync_pulse_width = 1,
    .vsync_back_porch = 8,
    .vsync_front_porch = 4,
    .vsync_pulse_width = 1,
    .flags.pclk_active_neg = true,
}
```

改为

```c
.timings = {
    .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
    .h_res = EXAMPLE_LCD_H_RES,
    .v_res = EXAMPLE_LCD_V_RES,
    // The following parameters should refer to LCD spec
    .hsync_back_porch = 40,
    .hsync_front_porch = 20,
    .hsync_pulse_width = 1,
    .vsync_back_porch = 8,
    .vsync_front_porch = 4,
    .vsync_pulse_width = 1,
    .flags.pclk_active_neg = false,
},
```

即可。截图如下：

<img src="https://taxue-alfred-1253400076.cos.ap-beijing.myqcloud.com/image-20230517163259824.png" alt="image-20230517163259824" style="zoom:50%;" />

<img src="https://taxue-alfred-1253400076.cos.ap-beijing.myqcloud.com/image-20230517163249587.png" alt="image-20230517163249587" style="zoom:50%;" />

## 3. 如何移植

修改`ST7701S_spi_init()`函数即可，即只修改初始化部分，初始化SPI使其可以发送1 + 8 = 9 bit的数据方式

## 4. 参考库

[https://github.com/moononournation/Arduino_GFX](https://github.com/moononournation/Arduino_GFX)

[https://www.bilibili.com/video/BV1hB4y197YN/](https://www.bilibili.com/video/BV1hB4y197YN/)

