#ifndef bringup_h
#define bringup_h

#include "tim.h"
#include "config.h"
#include "opt4060.h"
#include "data_process.h"

void bringup_init(void);
void tim1_loop(void);
void main_loop(void);

#endif
