#ifndef data_process_h
#define data_process_h

#include "usb.h"
#include "crc.h"
#include "string.h"
#include "opt4060.h"

#define usb_tx_header 0x5a

typedef struct __attribute__((packed))
{
    uint8_t header;
    float opt4060_data[4]; 
    uint16_t crc_sum;
}usb_tx;

void data_process_init(void);
void data_process_loop(void);

#endif
