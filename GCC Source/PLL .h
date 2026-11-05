#ifndef PLL_H
#define PLL_H

/////////////////////
//Defines for Si5351
/////////////////////
#define SI5351_ADDRESS 0xC0 // 0b11000000 for my module. Others may vary! The 0x60 did NOT work with my module!

//Set of Si5351A register addresses
#define CLK_ENABLE_CONTROL       3
#define PLLX_SRC				15
#define CLK0_CONTROL            16 
#define CLK1_CONTROL            17
#define CLK2_CONTROL            18
#define SYNTH_PLL_A             26
#define SYNTH_PLL_B             34
#define SYNTH_MS_0              42
#define SYNTH_MS_1              50
#define SYNTH_MS_2              58
#define SYNTH_MS_1_PHASE       166
#define PLL_RESET              177
#define XTAL_LOAD_CAP          183

//The unavoidable functional stuff
void wait_ms(int);

//  TWI Declarations: in i2c.c
//Some differences in init, try to test first
/*void twi_init(void);
void twi_start(void);
void twi_stop(void);
void twi_write(uint8_t u8data);
uint8_t twi_get_status(void);*/

//  SI5351 Declarations
void si5351_write(int, int);
void si5351_start(void);
void si5351_set_freq(int, unsigned long);
void si5351_set_freq_with_phase(unsigned long, unsigned long) 
int pll_init(long);
#endif