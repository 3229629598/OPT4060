#include "opt4060.h"

opt4060 opt4060_data;

// 读取并解析单个通道的 ADC 原始值
uint32_t OPT4060_ReadChannel(uint8_t msb_reg)
{
    uint8_t buf[2];
    // 读取 MSB 和 LSB 寄存器（连续地址）
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, OPT4060_ADDR, msb_reg, I2C_MEMADD_SIZE_8BIT, buf, 2, 100);
	
		if(ret != HAL_OK) return 0xFFFFFFFF;

    uint8_t exponent = (buf[0] >> 4) & 0x0F;              // 高4位指数
    uint32_t mantissa = ((uint32_t)(buf[0] & 0x0F) << 8)
                      | (uint32_t)(buf[1] & 0xFF);

    // 计算 ADC 码值：MANTISSA × 2^EXPONENT
    return mantissa << exponent;
}

void opt4060_init(void)
{
    uint16_t ctrl_val = 0;
    ctrl_val |= OPT4060_RANGE_AUTO;
    ctrl_val |= OPT4060_CONV_200MS;
    ctrl_val |= OPT4060_MODE_CONTINUOUS; 
    ctrl_val |= (1 << 3);   // 位 3: LATCH = 1 (中断锁存，默认)

    uint8_t buf[3];
    buf[0] = OPT4060_CTRL_REG;
    buf[1] = (ctrl_val >> 8) & 0xFF;    // 高字节
    buf[2] = ctrl_val & 0xFF;   // 低字节
	
    HAL_I2C_Master_Transmit(&hi2c1, OPT4060_ADDR, buf, 3, 100);
}

void opt4060_loop(void)
{
    uint32_t tmp;

    tmp = OPT4060_ReadChannel(OPT4060_RED_MSB);
		opt4060_data.adc_data.r = (tmp != 0xFFFFFFFF) ? tmp : opt4060_data.adc_data.r;
    tmp = OPT4060_ReadChannel(OPT4060_GREEN_MSB);
		opt4060_data.adc_data.g = (tmp != 0xFFFFFFFF) ? tmp : opt4060_data.adc_data.g;
    tmp = OPT4060_ReadChannel(OPT4060_BLUE_MSB);
		opt4060_data.adc_data.b = (tmp != 0xFFFFFFFF) ? tmp : opt4060_data.adc_data.b;
    tmp = OPT4060_ReadChannel(OPT4060_WHITE_MSB);
		opt4060_data.adc_data.w = (tmp != 0xFFFFFFFF) ? tmp : opt4060_data.adc_data.w;
	
    opt4060_data.intensity.r = opt4060_data.adc_data.r * SCALE_RED;
    opt4060_data.intensity.g = opt4060_data.adc_data.g * SCALE_GREEN;
    opt4060_data.intensity.b = opt4060_data.adc_data.b * SCALE_BLUE;
    opt4060_data.intensity.w = opt4060_data.adc_data.w * SCALE_WHITE;
    opt4060_data.lux = opt4060_data.adc_data.g * SCALE_LUX;
}
