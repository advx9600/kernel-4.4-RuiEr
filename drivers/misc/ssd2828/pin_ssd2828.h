#ifndef _PIN_SSD2828_H
#define _PIN_SSD2828_H

#define U32 unsigned int
#define U16 unsigned short
#define U8 unsigned char
#define uint16 U16

#define printf printk

// this need to reconfig
#define PIN_LCD_RESET		(PAD_GPIO_B +9) // 145
#define PIN_SSD2828_RESET	(PAD_GPIO_B +8)  // 146
#define PIN_SSD2828_SLEEP	(PAD_GPIO_B +11)  // 143

#define SPI_CLK			(PAD_GPIO_C +29)
#define SPI_CS			(PAD_GPIO_C +30)
#define SPI_MOSI		(PAD_GPIO_C +31)
#define SPI_MISO		(PAD_GPIO_D +0)
// end config

#endif
