# ST7701SForESP
ST7701S ESP系列驱动，基于ESP-IDF5.0，ESP32S3编写。

本库只负责SPI的配置，SPI设置屏幕信息两方面。由于RGB库和图形库的配置无法解耦，具体使用的图形库需要自行配置添加，**本示例默认绑定LVGL，使用IDF5示例`rgb_panel` **进行测试

SPI的指令，地址配置信息来源较多，其中有从Arduino_GFX库中移植，在此感谢陈亮大佬。

本库支持SPI配置和IO拓展IC配置屏幕，使用面向对象思想编程，支持多设备多实例，但不建议。

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

#### 2.1 克隆项目

```bash
//以下方式二选一即可，链接Github无法访问自行更换Gitee
git clone https://gitee.com/TaXue_TianXing/ST7701SForESP.git //直接克隆
git submodule add https://gitee.com/TaXue_TianXing/ST7701SForESP.git //作为模块克隆
```

#### 2.2 调用示例

```C
Vernon_ST7701S_handle vernon_st7701s = ST7701S_newObject(SPI_SDA, SPI_SCL, SPI_CS, SPI3_HOST, SPI_METHOD);
ST7701S_screen_init(vernon_st7701s, 2); //后方数字表示使用的配置下标
ST7701S_delObject(vernon_st7701s);
```

#### 2.3 加入Cmake

如果在components文件夹下作为模组使用，那么只需要命令行中

```bash
idf.py reconfigure
```

如果在其他文件夹下，那么需要设置CmkaeLists.txt加入编译列表，之后再执行上面的命令。

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
.flags.pclk_active_neg = true
```

改为

```c
.flags.pclk_active_neg = false
```

即可。截图如下：

<img src="https://taxue-alfred-1253400076.cos.ap-beijing.myqcloud.com/image-20230517163259824.png" alt="image-20230517163259824" style="zoom:50%;" />

<img src="https://taxue-alfred-1253400076.cos.ap-beijing.myqcloud.com/image-20230517163249587.png" alt="image-20230517163249587" style="zoom:50%;" />

## 3. 如何移植

修改`ST7701S_newObject`函数即可，即只修改初始化部分以及动态分配函数部分，**应该注意，再分配之后，应当为分配的内存区域全部置0**。之后初始化SPI使其可以发送1 + 8 = 9 bit的数据方式

## 4. 参考库

[https://github.com/moononournation/Arduino_GFX](https://github.com/moononournation/Arduino_GFX)

[https://www.bilibili.com/video/BV1hB4y197YN/](https://www.bilibili.com/video/BV1hB4y197YN/)

