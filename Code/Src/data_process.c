#include "data_process.h"

usb_tx tx_data;
uint8_t tx_len;

void data_process_init()
{
    tx_len = sizeof(usb_tx);
    tx_data.header = usb_tx_header;
}

void data_process_loop()
{
    memcpy((uint8_t*)&tx_data.opt4060_data, (uint8_t*)&opt4060_data.intensity, sizeof(float) * 4);
    Append_CRC16_Check_Sum((uint8_t*)&tx_data, tx_len);
    TEMPLATE_Transmit((uint8_t*)&tx_data, tx_len);
}
