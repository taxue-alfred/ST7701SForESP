# ST7701SForESP
ST7701S ESP系列驱动，基于ESP-IDF5.0，ESP32S3编写。

本库只负责SPI的配置，SPI设置屏幕两方面。由于RGB库和图形库的配置无法解耦，具体使用的RGB库，图形库需要自行配置添加。

本库使用面向对象思想编程，支持多设备多实例，但不建议

## 1. 目录结构

```
- components
	- Alfred_ST7701S 驱动文件
- main
	- ST7701SForESP.c 示例文件
```

## 2. 使用方法

### 1. LVGL报错解决

format %lu expects argument of type 'long unsigned int' but argument 3 has type long long unsigned int

> 如果在编译过程中未出现此类报错，可以直接跳过

![image-20230122220934556](https://taxue-alfred-1253400076.cos.ap-beijing.myqcloud.com/image-20230122220934556.png)

此处进行强制类型转换即可，修改之后编译通过即可。

### 2. 嵌入项目

直接将components中的Alfred_ST7701S放入自己的components中，后执行`idf.py reconfigure`重构CMake，主函数中调用函数即可

## 3. 如何移植

修改`ST7701S_spi_init()`函数即可，即只修改初始化部分，初始化SPI使其可以发送1 + 8 = 9 bit的数据方式

