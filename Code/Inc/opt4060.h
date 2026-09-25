#ifndef opt4060_h
#define opt4060_h

#include "i2c.h"

#define OPT4060_ADDR    (0x44 << 1) // ADDR接GND，7位地址0x48

#define OPT4060_CTRL_REG    0x0A    // 控制寄存器

#define OPT4060_RANGE_AUTO      (0xC << 10)  // Auto-Range
#define OPT4060_CONV_200MS      (0x9 << 6)   // 200ms转换时间
#define OPT4060_MODE_CONTINUOUS (0x3 << 4)   // 连续测量模式

#define OPT4060_RED_MSB     0x00    // 红色通道MSB
#define OPT4060_GREEN_MSB   0x02    // 绿色通道MSB
#define OPT4060_BLUE_MSB    0x04    // 蓝色通道MSB
#define OPT4060_WHITE_MSB   0x06    // 白色通道MSB

#define SCALE_RED    2.4f
#define SCALE_GREEN  1.0f
#define SCALE_BLUE   1.3f
#define SCALE_WHITE  1.0f
#define SCALE_LUX    2.15e-3f

typedef struct
{
    struct
    {
        uint32_t r;
        uint32_t g;
        uint32_t b;
        uint32_t w;
    } adc_data;
    struct
    {
        float r;
        float g;
        float b;
        float w;
    } intensity;
    float lux;
} opt4060;

extern opt4060 opt4060_data;

void opt4060_init(void);
void opt4060_loop(void);

#endif
